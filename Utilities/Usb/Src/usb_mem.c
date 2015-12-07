/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_mem.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Utility functions for memory transfers to/from PMA
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : UserToPMABufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf: pointer to user memory area.
*                  - wPMABufAddr: address into PMA.
*                  - wNBytes: no. of bytes to be copied.
* Output         : None.
* Return         : None	.
*******************************************************************************/
UINT16 UserToPMABufferCopy(const UINT8 *pbUsrBuf,UINT8 bEpNum,UINT16 wPMABufAddr, UINT16 wNBytes)
{
  UINT32 n = (wNBytes + 1) >> 1;   /* n = (wNBytes + 1) / 2 */
  UINT32 i, temp1, temp2;
  UINT16 *pdwVal;
  pdwVal = (UINT16 *)(wPMABufAddr * 2 + PMAAddr);
  for (i = n; i != 0; i--)
  {
    temp1 = (UINT16) * pbUsrBuf;
    pbUsrBuf++;
    temp2 = temp1 | (UINT16) * pbUsrBuf << 8;
    *pdwVal++ = temp2;
    pdwVal++;
    pbUsrBuf++;
  }
  SetEPTxCount(bEpNum, wNBytes);  //70 --> 84
  return wNBytes;
}
/*******************************************************************************
* Function Name  : PMAToUserBufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf    = pointer to user memory area.
*                  - wPMABufAddr = address into PMA.
*                  - wNBytes     = no. of bytes to be copied.
* Output         : None.
* Return         : None.
*******************************************************************************/
UINT16 PMAToUserBufferCopy(UINT8 *pbUsrBuf, UINT16 wPMABufAddr, UINT16 wNBytes)
{
     UINT32 n = (wNBytes + 1) >> 1;/* /2*/
     UINT32 i;
     UINT32 *pdwVal;
     pdwVal = (UINT32 *)(wPMABufAddr * 2 + PMAAddr);  //*2得到真正的地址
     for (i = n; i != 0; i--)
     {
         *(UINT16*)pbUsrBuf++ = *pdwVal++;
         pbUsrBuf++;
     }
     return wNBytes;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
