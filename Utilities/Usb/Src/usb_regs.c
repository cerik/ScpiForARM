/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_regs.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Interface functions to USB cell registers
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
* Function Name  : SetCNTR.
* Description    : Set the CNTR register value.
* Input          : wRegValue: new register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetCNTR(UINT16 wRegValue)
{
  _SetCNTR(wRegValue);
}

/*******************************************************************************
* Function Name  : GetCNTR.
* Description    : returns the CNTR register value.
* Input          : None.
* Output         : None.
* Return         : CNTR register Value.
*******************************************************************************/
UINT16 GetCNTR(void)
{
  return(_GetCNTR());
}

/*******************************************************************************
* Function Name  : SetISTR.
* Description    : Set the ISTR register value.
* Input          : wRegValue: new register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetISTR(UINT16 wRegValue)
{
  _SetISTR(wRegValue);
}

/*******************************************************************************
* Function Name  : GetISTR
* Description    : Returns the ISTR register value.
* Input          : None.
* Output         : None.
* Return         : ISTR register Value
*******************************************************************************/
UINT16 GetISTR(void)
{
  return(_GetISTR());
}

/*******************************************************************************
* Function Name  : GetFNR
* Description    : Returns the FNR register value.
* Input          : None.
* Output         : None.
* Return         : FNR register Value
*******************************************************************************/
UINT16 GetFNR(void)
{
  return(_GetFNR());
}

/*******************************************************************************
* Function Name  : SetDADDR
* Description    : Set the DADDR register value.
* Input          : wRegValue: new register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetDADDR(UINT16 wRegValue)
{
  _SetDADDR(wRegValue);
}

/*******************************************************************************
* Function Name  : GetDADDR
* Description    : Returns the DADDR register value.
* Input          : None.
* Output         : None.
* Return         : DADDR register Value
*******************************************************************************/
UINT16 GetDADDR(void)
{
  return(_GetDADDR());
}

/*******************************************************************************
* Function Name  : SetBTABLE
* Description    : Set the BTABLE.
* Input          : wRegValue: New register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetBTABLE(UINT16 wRegValue)
{
  _SetBTABLE(wRegValue);
}

/*******************************************************************************
* Function Name  : GetBTABLE.
* Description    : Returns the BTABLE register value.
* Input          : None. 
* Output         : None.
* Return         : BTABLE address.
*******************************************************************************/
UINT16 GetBTABLE(void)
{
  return(_GetBTABLE());
}

/*******************************************************************************
* Function Name  : SetENDPOINT
* Description    : Setthe Endpoint register value.
* Input          : bEpNum: Endpoint Number. 
*                  wRegValue.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetENDPOINT(UINT8 bEpNum, UINT16 wRegValue)
{
  _SetENDPOINT(bEpNum, wRegValue);
}

/*******************************************************************************
* Function Name  : GetENDPOINT
* Description    : Return the Endpoint register value.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint register value.
*******************************************************************************/
UINT16 GetENDPOINT(UINT8 bEpNum)
{
  return(_GetENDPOINT(bEpNum));
}

/*******************************************************************************
* Function Name  : SetEPType
* Description    : sets the type in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
*                  wType: type definition.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPType(UINT8 bEpNum, UINT16 wType)
{
  _SetEPType(bEpNum, wType);
}

/*******************************************************************************
* Function Name  : GetEPType
* Description    : Returns the endpoint type.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint Type
*******************************************************************************/
UINT16 GetEPType(UINT8 bEpNum)
{
  return(_GetEPType(bEpNum));
}

/*******************************************************************************
* Function Name  : SetEPTxStatus
* Description    : Set the status of Tx endpoint.
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxStatus(UINT8 bEpNum, UINT16 wState)
{
  _SetEPTxStatus(bEpNum, wState);
}

/*******************************************************************************
* Function Name  : SetEPRxStatus
* Description    : Set the status of Rx endpoint.
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxStatus(UINT8 bEpNum, UINT16 wState)
{
  _SetEPRxStatus(bEpNum, wState);
}

/*******************************************************************************
* Function Name  : SetDouBleBuffEPStall
* Description    : sets the status for Double Buffer Endpoint to STALL
* Input          : bEpNum: Endpoint Number. 
*                  bDir: Endpoint direction.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetDouBleBuffEPStall(UINT8 bEpNum, UINT8 bDir)
{
  UINT16 Endpoint_DTOG_Status;
  Endpoint_DTOG_Status = GetENDPOINT(bEpNum);
  if (bDir == EP_DBUF_OUT)
  { /* OUT double buffered endpoint */
    _SetENDPOINT(bEpNum, Endpoint_DTOG_Status & ~EPRX_DTOG1);
  }
  else if (bDir == EP_DBUF_IN)
  { /* IN double buffered endpoint */
    _SetENDPOINT(bEpNum, Endpoint_DTOG_Status & ~EPTX_DTOG1);
  }
}

/*******************************************************************************
* Function Name  : GetEPTxStatus
* Description    : Returns the endpoint Tx status.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint TX Status
*******************************************************************************/
UINT16 GetEPTxStatus(UINT8 bEpNum)
{
  return(_GetEPTxStatus(bEpNum));
}

/*******************************************************************************
* Function Name  : GetEPRxStatus
* Description    : Returns the endpoint Rx status.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint RX Status
*******************************************************************************/
UINT16 GetEPRxStatus(UINT8 bEpNum)
{
  return(_GetEPRxStatus(bEpNum));
}

/*******************************************************************************
* Function Name  : SetEPTxValid
* Description    : Valid the endpoint Tx Status.
* Input          : bEpNum: Endpoint Number.  
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxValid(UINT8 bEpNum)
{
  _SetEPTxStatus(bEpNum, EP_TX_VALID);
}

/*******************************************************************************
* Function Name  : SetEPRxValid
* Description    : Valid the endpoint Rx Status.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxValid(UINT8 bEpNum)
{
  _SetEPRxStatus(bEpNum, EP_RX_VALID);
}

/*******************************************************************************
* Function Name  : SetEP_KIND
* Description    : Clear the EP_KIND bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEP_KIND(UINT8 bEpNum)
{
  _SetEP_KIND(bEpNum);
}

/*******************************************************************************
* Function Name  : ClearEP_KIND
* Description    : set the  EP_KIND bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearEP_KIND(UINT8 bEpNum)
{
  _ClearEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : Clear_Status_Out
* Description    : Clear the Status Out of the related Endpoint
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void Clear_Status_Out(UINT8 bEpNum)
{
  _ClearEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : Set_Status_Out
* Description    : Set the Status Out of the related Endpoint
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_Status_Out(UINT8 bEpNum)
{
  _SetEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : SetEPDoubleBuff
* Description    : Enable the double buffer feature for the endpoint. 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDoubleBuff(UINT8 bEpNum)
{
  _SetEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : ClearEPDoubleBuff
* Description    : Disable the double buffer feature for the endpoint. 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearEPDoubleBuff(UINT8 bEpNum)
{
  _ClearEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : GetTxStallStatus
* Description    : Returns the Stall status of the Tx endpoint.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Tx Stall status.
*******************************************************************************/
UINT16 GetTxStallStatus(UINT8 bEpNum)
{
  return(_GetTxStallStatus(bEpNum));
}
/*******************************************************************************
* Function Name  : GetRxStallStatus
* Description    : Returns the Stall status of the Rx endpoint. 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Rx Stall status.
*******************************************************************************/
UINT16 GetRxStallStatus(UINT8 bEpNum)
{
  return(_GetRxStallStatus(bEpNum));
}
/*******************************************************************************
* Function Name  : ClearEP_CTR_RX
* Description    : Clear the CTR_RX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearEP_CTR_RX(UINT8 bEpNum)
{
  _ClearEP_CTR_RX(bEpNum);
}
/*******************************************************************************
* Function Name  : ClearEP_CTR_TX
* Description    : Clear the CTR_TX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearEP_CTR_TX(UINT8 bEpNum)
{
  _ClearEP_CTR_TX(bEpNum);
}
/*******************************************************************************
* Function Name  : ToggleDTOG_RX
* Description    : Toggle the DTOG_RX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ToggleDTOG_RX(UINT8 bEpNum)
{
  _ToggleDTOG_RX(bEpNum);
}
/*******************************************************************************
* Function Name  : ToggleDTOG_TX
* Description    : Toggle the DTOG_TX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ToggleDTOG_TX(UINT8 bEpNum)
{
  _ToggleDTOG_TX(bEpNum);
}
/*******************************************************************************
* Function Name  : ClearDTOG_RX.
* Description    : Clear the DTOG_RX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearDTOG_RX(UINT8 bEpNum)
{
  _ClearDTOG_RX(bEpNum);
}
/*******************************************************************************
* Function Name  : ClearDTOG_TX.
* Description    : Clear the DTOG_TX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearDTOG_TX(UINT8 bEpNum)
{
  _ClearDTOG_TX(bEpNum);
}
/*******************************************************************************
* Function Name  : SetEPAddress
* Description    : Set the endpoint address.
* Input          : bEpNum: Endpoint Number.
*                  bAddr: New endpoint address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPAddress(UINT8 bEpNum, UINT8 bAddr)
{
  _SetEPAddress(bEpNum, bAddr);
}
/*******************************************************************************
* Function Name  : GetEPAddress
* Description    : Get the endpoint address.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint address.
*******************************************************************************/
UINT8 GetEPAddress(UINT8 bEpNum)
{
  return(_GetEPAddress(bEpNum));
}
/*******************************************************************************
* Function Name  : SetEPTxAddr
* Description    : Set the endpoint Tx buffer address.
* Input          : bEpNum: Endpoint Number.
*                  wAddr: new address. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxAddr(UINT8 bEpNum, UINT16 wAddr)
{
  _SetEPTxAddr(bEpNum, wAddr);
}
/*******************************************************************************
* Function Name  : SetEPRxAddr
* Description    : Set the endpoint Rx buffer address.
* Input          : bEpNum: Endpoint Number.
*                  wAddr: new address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxAddr(UINT8 bEpNum, UINT16 wAddr)
{
  _SetEPRxAddr(bEpNum, wAddr);
}
/*******************************************************************************
* Function Name  : GetEPTxAddr
* Description    : Returns the endpoint Tx buffer address.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Rx buffer address. 
*******************************************************************************/
UINT16 GetEPTxAddr(UINT8 bEpNum)
{
  return(_GetEPTxAddr(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPRxAddr.
* Description    : Returns the endpoint Rx buffer address.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Rx buffer address.
*******************************************************************************/
UINT16 GetEPRxAddr(UINT8 bEpNum)
{
  return(_GetEPRxAddr(bEpNum));
}
/*******************************************************************************
* Function Name  : SetEPTxCount.
* Description    : Set the Tx count.
* Input          : bEpNum: Endpoint Number.
*                  wCount: new count value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxCount(UINT8 bEpNum, UINT16 wCount)
{
  _SetEPTxCount(bEpNum, wCount);
}
/*******************************************************************************
* Function Name  : SetEPCountRxReg.
* Description    : Set the Count Rx Register value.
* Input          : *pdwReg: point to the register.
*                  wCount: the new register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPCountRxReg(UINT32 *pdwReg, UINT16 wCount)
{
  _SetEPCountRxReg(dwReg, wCount);
}
/*******************************************************************************
* Function Name  : SetEPRxCount
* Description    : Set the Rx count.
* Input          : bEpNum: Endpoint Number. 
*                  wCount: the new count value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxCount(UINT8 bEpNum, UINT16 wCount)
{
  _SetEPRxCount(bEpNum, wCount);
}
/*******************************************************************************
* Function Name  : GetEPTxCount
* Description    : Get the Tx count.
* Input          : bEpNum: Endpoint Number. 
* Output         : None
* Return         : Tx count value.
*******************************************************************************/
UINT16 GetEPTxCount(UINT8 bEpNum)
{
  return(_GetEPTxCount(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPRxCount
* Description    : Get the Rx count.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Rx count value.
*******************************************************************************/
UINT16 GetEPRxCount(UINT8 bEpNum)
{
  return(_GetEPRxCount(bEpNum));
}
/*******************************************************************************
* Function Name  : SetEPDblBuffAddr
* Description    : Set the addresses of the buffer 0 and 1.
* Input          : bEpNum: Endpoint Number.  
*                  wBuf0Addr: new address of buffer 0. 
*                  wBuf1Addr: new address of buffer 1.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuffAddr(UINT8 bEpNum, UINT16 wBuf0Addr, UINT16 wBuf1Addr)
{
  _SetEPDblBuffAddr(bEpNum, wBuf0Addr, wBuf1Addr);
}
/*******************************************************************************
* Function Name  : SetEPDblBuf0Addr
* Description    : Set the Buffer 1 address.
* Input          : bEpNum: Endpoint Number
*                  wBuf0Addr: new address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuf0Addr(UINT8 bEpNum, UINT16 wBuf0Addr)
{
  _SetEPDblBuf0Addr(bEpNum, wBuf0Addr);
}
/*******************************************************************************
* Function Name  : SetEPDblBuf1Addr
* Description    : Set the Buffer 1 address.
* Input          : bEpNum: Endpoint Number
*                  wBuf1Addr: new address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuf1Addr(UINT8 bEpNum, UINT16 wBuf1Addr)
{
  _SetEPDblBuf1Addr(bEpNum, wBuf1Addr);
}
/*******************************************************************************
* Function Name  : GetEPDblBuf0Addr
* Description    : Returns the address of the Buffer 0.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : None.
*******************************************************************************/
UINT16 GetEPDblBuf0Addr(UINT8 bEpNum)
{
  return(_GetEPDblBuf0Addr(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPDblBuf1Addr
* Description    : Returns the address of the Buffer 1.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : Address of the Buffer 1.
*******************************************************************************/
UINT16 GetEPDblBuf1Addr(UINT8 bEpNum)
{
  return(_GetEPDblBuf1Addr(bEpNum));
}
/*******************************************************************************
* Function Name  : SetEPDblBuffCount
* Description    : Set the number of bytes for a double Buffer 
*                  endpoint.
* Input          : bEpNum,bDir, wCount
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuffCount(UINT8 bEpNum, UINT8 bDir, UINT16 wCount)
{
  _SetEPDblBuffCount(bEpNum, bDir, wCount);
}
/*******************************************************************************
* Function Name  : SetEPDblBuf0Count
* Description    : Set the number of bytes in the buffer 0 of a double Buffer 
*                  endpoint.
* Input          : bEpNum, bDir,  wCount
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuf0Count(UINT8 bEpNum, UINT8 bDir, UINT16 wCount)
{
  _SetEPDblBuf0Count(bEpNum, bDir, wCount);
}
/*******************************************************************************
* Function Name  : SetEPDblBuf1Count
* Description    : Set the number of bytes in the buffer 0 of a double Buffer 
*                  endpoint.
* Input          : bEpNum,  bDir,  wCount
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuf1Count(UINT8 bEpNum, UINT8 bDir, UINT16 wCount)
{
  _SetEPDblBuf1Count(bEpNum, bDir, wCount);
}
/*******************************************************************************
* Function Name  : GetEPDblBuf0Count
* Description    : Returns the number of byte received in the buffer 0 of a double
*                  Buffer endpoint.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : Endpoint Buffer 0 count
*******************************************************************************/
UINT16 GetEPDblBuf0Count(UINT8 bEpNum)
{
  return(_GetEPDblBuf0Count(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPDblBuf1Count
* Description    : Returns the number of data received in the buffer 1 of a double
*                  Buffer endpoint.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : Endpoint Buffer 1 count.
*******************************************************************************/
UINT16 GetEPDblBuf1Count(UINT8 bEpNum)
{
  return(_GetEPDblBuf1Count(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPDblBufDir
* Description    : gets direction of the double buffered endpoint
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : EP_DBUF_OUT, EP_DBUF_IN,
*                  EP_DBUF_ERR if the endpoint counter not yet programmed.
*******************************************************************************/
EP_DBUF_DIR GetEPDblBufDir(UINT8 bEpNum)
{
  if ((UINT16)(*_pEPRxCount(bEpNum) & 0xFC00) != 0)
    return(EP_DBUF_OUT);
  else if (((UINT16)(*_pEPTxCount(bEpNum)) & 0x03FF) != 0)
    return(EP_DBUF_IN);
  else
    return(EP_DBUF_ERR);
}
/*******************************************************************************
* Function Name  : FreeUserBuffer
* Description    : free buffer used from the application realizing it to the line
                   toggles bit SW_BUF in the double buffered endpoint register
* Input          : bEpNum, bDir
* Output         : None.
* Return         : None.
*******************************************************************************/
void FreeUserBuffer(UINT8 bEpNum, UINT8 bDir)
{
  if (bDir == EP_DBUF_OUT)
  { /* OUT double buffered endpoint */
    _ToggleDTOG_TX(bEpNum);
  }
  else if (bDir == EP_DBUF_IN)
  { /* IN double buffered endpoint */
    _ToggleDTOG_RX(bEpNum);
  }
}

/*******************************************************************************
* Function Name  : ToWord
* Description    : merge two byte in a word.
* Input          : bh: byte high, bl: bytes low.
* Output         : None.
* Return         : resulted word.
*******************************************************************************/
UINT16 ToWord(UINT8 bh, UINT8 bl)
{
  UINT16 wRet;
  wRet = (UINT16)bl | ((UINT16)bh << 8);
  return(wRet);
}
/*******************************************************************************
* Function Name  : ByteSwap
* Description    : Swap two byte in a word.
* Input          : wSwW: word to Swap.
* Output         : None.
* Return         : resulted word.
*******************************************************************************/
UINT16 ByteSwap(UINT16 wSwW)
{
  UINT8 bTemp;
  UINT16 wRet;
  bTemp = (UINT8)(wSwW & 0xff);
  wRet =  (wSwW >> 8) | ((UINT16)bTemp << 8);
  return(wRet);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
