/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "usb_conf.h"
#include "usb_lib.h"
#include "usb_tmc.h"
#include "bsp.h"
#include "debug.h"

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static boolean gTxFinished = TRUE;

tagTmcLayerInfo gTmcLayerInfo={
    {0},      //lastTmcBulkOutHeader
    {0},      //lastTmcBulkInHeader
    TMC_IDLE, //rxState
    TMC_IDLE, //txState
    TERR_NONE,//tmcLastError
    TRUE,     //rxFinished
    0,        //rxCount
    FALSE,    //txFinished
    0,        //txCount
    FALSE,    //hostRead
    FALSE     //hostWrite
};

/*
 *********************************************************
 *  TMC transfer layer buffer;
 *  used to package the user data into several tmc pkg
 *********************************************************
 */
UINT8 tmcLayerTxBuffer[EP_MAXPKGSIZE]={0};
UINT8 tmcLayerRxBuffer[USB_BUF_SIZE]={0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
 ***************************************************************
 * Bulk-Out Endpoint : Host -> device
 *
 * process received package of BULK-OUT endpoint from host.
 * ENDPOINT1;
 ***************************************************************
 */
void EP1_OUT_Callback(void)
{
    tagTmcBulkOutHeader *tmcBulkOutHeaderPtr=0;

    switch(gTmcLayerInfo.rxState)
    {
    case TMC_IDLE:
        //New transfer, clear rxReady.
        gTmcLayerInfo.rxFinished = FALSE;

        //Init the data count of the first transaction(not include header info), and save the newest pkg header.
        gTmcLayerInfo.rxCount  = (PMAToUserBufferCopy(tmcLayerRxBuffer,ENDP1_RXADDR,GetEPRxCount(ENDP1))-TMC_HEADER_SIZE);
        tmcBulkOutHeaderPtr    = (tagTmcBulkOutHeader*)tmcLayerRxBuffer;

        //DEBUG_MSG(TMC_DEBUG,"****%d\n",tmcBulkOutHeaderPtr->transferSize);

        //pkg check
        if(tmcBulkOutHeaderPtr->bTag == gTmcLayerInfo.lastTmcBulkOutHeader.bTag)
        {
            gTmcLayerInfo.tmcLastError = TERR_TAG;
            goto TMC_ERROR;
        }
        else if(tmcBulkOutHeaderPtr->bTag & tmcBulkOutHeaderPtr->bTagInverse)
        {
            gTmcLayerInfo.tmcLastError = TERR_TAGINV;
            goto TMC_ERROR;
        }
        else
        {
            gTmcLayerInfo.tmcLastError = TERR_NONE;
        }

        //save the newest tmc message Header;
        memcpy((UINT8*)&gTmcLayerInfo.lastTmcBulkOutHeader,tmcLayerRxBuffer,TMC_HEADER_SIZE);
        if(gTmcLayerInfo.rxCount < MAXHRADERDATASIZE)
        {
            gTmcLayerInfo.rxFinished = TRUE;
            //DEBUG_LOG(TMC_DEBUG,"%s:%d;Finished.\n--------\n",__func__,__LINE__);
        }
        //change the tmc state
        else if(tmcBulkOutHeaderPtr->transferSize <= MAXHRADERDATASIZE)//???????????
        {//all data have been completelly recevied.
            gTmcLayerInfo.rxFinished = TRUE;
            //DEBUG_LOG(TMC_DEBUG,"%s:%d;Finished.\n--------\n",__func__,__LINE__);
        }
        else
        {
            gTmcLayerInfo.rxState = TMC_RUN;
            DEBUG_LOG(TMC_DEBUG,"%s:%d\n",__func__,__LINE__);
        }
        break;
    case TMC_RUN:
        //DEBUG_LOG(TMC_DEBUG,"%s:%d\n",__func__,__LINE__);
        //now, the pkg is the continue of this transfer;
        if( (TMC_HEADER_SIZE + gTmcLayerInfo.rxCount + GetEPRxCount(ENDP1)) >= USB_BUF_SIZE )
        {//Overflow
            DEBUG_LOG(TMC_DEBUG,"%s:%d\n",__func__,__LINE__);
            gTmcLayerInfo.tmcLastError = TERR_MSGOVERFLOW;
            gTmcLayerInfo.rxState      = TMC_IDLE;
        }
        else
        {
            DEBUG_LOG(TMC_DEBUG,"%s:%d\n",__func__,__LINE__);
            //The continue transaction are all data package,not contain header info, so just add the length to rxCount.
            gTmcLayerInfo.rxCount += PMAToUserBufferCopy(tmcLayerRxBuffer + TMC_HEADER_SIZE + gTmcLayerInfo.rxCount,ENDP1_RXADDR,GetEPRxCount(ENDP1));
            if( gTmcLayerInfo.lastTmcBulkOutHeader.transferSize <= gTmcLayerInfo.rxCount )
            {//OK,data have been received finished.
                gTmcLayerInfo.rxFinished   = TRUE;
                gTmcLayerInfo.rxState      = TMC_IDLE;
                gTmcLayerInfo.tmcLastError = TERR_NONE;
            }
        }
        break;
    default:
        break;
    }
    if(tmcRxFinished()) tmcCmdAnalize();
TMC_ERROR:
    SetEPRxStatus(ENDP1, EP_RX_VALID);
}

/*
 ***************************************************************
 * Bulk-IN Endpoint
 *
 * PC的IN分组回调函数
 * 在成功发给PC分组后回调该函数
 ***************************************************************
 */
void EP2_IN_Callback(void)
{
    gTxFinished = TRUE;
}

/*
 ***************************************************************
 * RESET Callback 
 * This function will be called after usb connected successfully
 *  with host.
 * So, we need reset tmc state machine here.
 ***************************************************************
 */
void RESET_Callback(void)
{
    gTxFinished = TRUE;
    gTmcLayerInfo.txState = TMC_IDLE;
}

/*
 ***************************************************************
 * Send data from user layer;
 * User data will be divided into several part if 
 * its length is bigger then the maxpacksize;
 * And , only the first contains the tmcHeader information.
 *
 * bufLen:
 *    [0 ~ 1024] byte
 ***************************************************************
 */
UINT32 tmcSendDat(const void * const pBuffer,const UINT32 bufLen)
{
    UINT32 index=0,xmitLen=0,cpyIndex=0;
    UINT16 pkgCnt,remainCnt;
    tagCounter counter;
    UINT8  errId;
    const UINT8 * const pSrc=(UINT8*)pBuffer;

    if( 0 == bufLen )
    {
        DEBUG_MSG(TMC_DEBUG,"bufLen:%d\n",bufLen);
        goto END;
    }
    switch(gTmcLayerInfo.txState)
    {
    case TMC_IDLE: //first package
        gTmcLayerInfo.txFinished = FALSE;
        gTmcLayerInfo.lastTmcBulkInHeader.transferAttr = 0x1;    //last byte is the last byte of this transfer
        gTmcLayerInfo.lastTmcBulkInHeader.reserved1[0] = 0;
        gTmcLayerInfo.lastTmcBulkInHeader.reserved1[1] = 0;
        gTmcLayerInfo.lastTmcBulkInHeader.reserved1[2] = 0;

        StartCounter(&counter,50);//50ms
        while(FALSE == gTxFinished)
        {
            if(CounterArrived(&counter))
            {
                errId = 1;
                goto TIMEOUT;
            }
        }

        //copy the bulk-in header into tx buffer.
        memcpy(tmcLayerTxBuffer,(const void *)&gTmcLayerInfo.lastTmcBulkInHeader,TMC_HEADER_SIZE);

        xmitLen =gTmcLayerInfo.lastTmcBulkOutHeader.transferSize;//host expected byte
        if(xmitLen > bufLen) xmitLen = bufLen;//this will course host soft wait timeout.
        
        //copy user data into tx buffer.
        for(index=0;(index < xmitLen) && (index < MAXHRADERDATASIZE);index++,cpyIndex++)
            tmcLayerTxBuffer[TMC_HEADER_SIZE + cpyIndex] = pSrc[cpyIndex]; 
        gTxFinished = FALSE;
        UserToPMABufferCopy(tmcLayerTxBuffer,ENDP2,ENDP2_TXADDR,TMC_HEADER_SIZE + index);
        gTmcLayerInfo.txCount += index;
        SetEPTxValid(ENDP2);
    case TMC_RUN:  //send the remained package of this transfer
        pkgCnt    = (xmitLen - index) / EP_MAXPKGSIZE;
        remainCnt = (xmitLen - index) % EP_MAXPKGSIZE;

        while(pkgCnt-- > 0)
        {
            StartCounter(&counter,50);//50ms
            while(FALSE == gTxFinished)
            {
                if(CounterArrived(&counter))
                {
                    errId = 2;
                    goto TIMEOUT;
                }    
            }

            for(index=0;index<EP_MAXPKGSIZE;index++)
            {
                tmcLayerTxBuffer[index] = pSrc[cpyIndex++];
            }
            gTxFinished = FALSE;
            gTmcLayerInfo.txCount += UserToPMABufferCopy(tmcLayerTxBuffer,ENDP2,ENDP2_TXADDR,index);
            SetEPTxValid(ENDP2);
        }

        //send the last package of this transfer,
        if(remainCnt)
        {   //send the last pkg;(device is not required to send any alignment bytes)
            StartCounter(&counter,50);//50ms
            while(FALSE == gTxFinished)
            {
                if(CounterArrived(&counter))
                {
                    errId = 3;
                    goto TIMEOUT;
                }    
            }

            for(index=0;index<remainCnt;index++)
            {
                tmcLayerTxBuffer[index] = pSrc[cpyIndex++];
            }
            gTxFinished = FALSE;
            gTmcLayerInfo.txCount += UserToPMABufferCopy(tmcLayerTxBuffer,ENDP2,ENDP2_TXADDR,index);
            SetEPTxValid(ENDP2);
        }
        gTmcLayerInfo.txFinished = FALSE;
        gTmcLayerInfo.txState    = TMC_IDLE;
    }
    goto END;

TIMEOUT:
    DEBUG_MSG(TMC_ERROR_LOG,"Timeout ID=%d\n",errId);
END:
    DEBUG_MSG(TMC_DEBUG,"txCount:%d\n====\n",gTmcLayerInfo.txCount);
    return gTmcLayerInfo.txCount;
}

boolean tmcRxFinished(void)
{
    return gTmcLayerInfo.rxFinished;
}
void tmcRxFinishedClear(void)
{
    gTmcLayerInfo.rxFinished=FALSE;
    gTmcLayerInfo.rxCount   =0;
}

tagTmcError tmcLastError(void)
{
    return  gTmcLayerInfo.tmcLastError;
}
UINT32 tmcRxDataSize(void)
{
    return  gTmcLayerInfo.lastTmcBulkOutHeader.transferSize;
}
void *tmcRxDatPtr(void)
{
    return (void *)(tmcLayerRxBuffer+TMC_HEADER_SIZE);
}
void *tmcGetLastBulkOutHeader(void)
{
    return  (void*)&gTmcLayerInfo.lastTmcBulkOutHeader;
}
void *tmcGetTxBufPtr(void)
{
    return (void*)tmcLayerRxBuffer;
}

/*
 **********************************************************
 * Description:
 *   Get user data from TMC layer.
 *   
 * Parameters:
 *   buffer ----- where the user data will be stored.
 * 
 * Return:
 *   received count
 **********************************************************
 */
UINT32 tmcGetDat( void *buffer, UINT32 bufSize)
{   
    UINT8 *pSrc;
    UINT32 datSize=0;
    
    __disable_irq();//disable all interrupt 

    if( FALSE == tmcRxFinished())  goto ERROR;

    datSize = tmcRxDataSize();
    if(bufSize < datSize) goto ERROR;
    
    pSrc = (UINT8*)tmcRxDatPtr();
    memcpy(buffer,pSrc,datSize);

ERROR:
    tmcRxFinishedClear();
    __enable_irq();//NVIC_RESETPRIMASK();//resume interrupt
    return datSize;
}

