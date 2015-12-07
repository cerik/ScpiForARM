
#include <string.h>
#include "usb_tmc.h"
#include "debug.h"

/*
 ***************************************************** 
 * File Name    : protocol.c
 * Author       : CCZY & LYS
 * Version      : V6.1
 * Date         : 11/15/2011
 * Description  : Source file TMC protocol layer.
 *******************************************************
 */
void tmcCmdAnalize(void)
{
    if(TRUE == gTmcLayerInfo.rxFinished) //all data have been received.
    {
        switch(gTmcLayerInfo.lastTmcBulkOutHeader.msgId)
        {
        case DEV_DEP_MSG_OUT: //This is the host Write operation,not read operation
            gTmcLayerInfo.hostRead  = FALSE;
            gTmcLayerInfo.hostWrite = TRUE;

            gTmcLayerInfo.txCount   = 0;
            DEBUG_MSG(TMC_DEBUG,"\n");
            break;
        case REQUEST_DEV_DEP_MSG_IN: //This is the host read operation,not write operation.
            gTmcLayerInfo.hostRead  = TRUE;
            gTmcLayerInfo.hostWrite = FALSE;

            memcpy((UINT8*)&gTmcLayerInfo.lastTmcBulkInHeader,
                   (const void *)&gTmcLayerInfo.lastTmcBulkOutHeader,
                   TMC_HEADER_SIZE);
            DEBUG_MSG(TMC_DEBUG,"txCount:%d,%d,%d\n",gTmcLayerInfo.txCount,
                    gTmcLayerInfo.hostRead ,gTmcLayerInfo.hostWrite);
            //DEBUG_MSG(1,"REQ_MSG_IN:\n");
            //for(index=0;index<12;index++)
            //    printf("%X ",((UINT8*)&gTmcLayerInfo.lastTmcBulkOutHeader)[index]);
            //printf("\n");

            break;
        case VENDOR_SPECIFIC_OUT:
            
            break;
        default:
            gTmcLayerInfo.tmcLastError = TERR_MSGID;
            break;
        } 
    }
}

boolean tmcIsHostWrite(void)
{
    return gTmcLayerInfo.hostWrite;
}

boolean tmcIsHostRead(void)
{
    return gTmcLayerInfo.hostRead;
}

void tmcHostReadClear(void)
{
    gTmcLayerInfo.hostRead = FALSE;
}

void tmcHostWriteClear(void)
{
    gTmcLayerInfo.hostWrite = FALSE;
}

UINT32 tmcHostReqSize(void)
{
    return gTmcLayerInfo.lastTmcBulkInHeader.transferSize;
}
UINT32 tmcTxCount(void)
{
    return gTmcLayerInfo.txCount;
}
