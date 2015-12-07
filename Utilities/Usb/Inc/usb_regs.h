/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_regs.h
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Interface prototype functions to USB cell registers
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_REGS_H
#define __USB_REGS_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _EP_DBUF_DIR
{
  /* double buffered endpoint direction */
  EP_DBUF_ERR,
  EP_DBUF_OUT,
  EP_DBUF_IN
}EP_DBUF_DIR;

/* endpoint buffer number */
enum EP_BUF_NUM
{
  EP_NOBUF,
  EP_BUF0,
  EP_BUF1
};

/* Exported constants --------------------------------------------------------*/
#define RegBase  (0x40005C00L)  /* USB_IP Peripheral Registers base address */
#define PMAAddr  (0x40006000L)  /* USB_IP Packet Memory Area base address   */

/******************************************************************************/
/*                         General registers                                  */
/******************************************************************************/

/* Control register */
#define CNTR    ((volatile unsigned *)(RegBase + 0x40))
/* Interrupt status register */
#define ISTR    ((volatile unsigned *)(RegBase + 0x44))
/* Frame number register */
#define FNR     ((volatile unsigned *)(RegBase + 0x48))
/* Device address register */
#define DADDR   ((volatile unsigned *)(RegBase + 0x4C))
/* Buffer Table address register */
#define BTABLE  ((volatile unsigned *)(RegBase + 0x50))
/******************************************************************************/
/*                         Endpoint registers                                 */
/******************************************************************************/
#define EP0REG  ((volatile unsigned *)(RegBase)) /* endpoint 0 register address */

/* endpoints enumeration */
#define ENDP0   ((UINT8)0)
#define ENDP1   ((UINT8)1)
#define ENDP2   ((UINT8)2)
#define ENDP3   ((UINT8)3)
#define ENDP4   ((UINT8)4)
#define ENDP5   ((UINT8)5)
#define ENDP6   ((UINT8)6)
#define ENDP7   ((UINT8)7)
/******************************************************************************/
/*                       ISTR interrupt events                                */
/******************************************************************************/
#define ISTR_CTR    (0x8000) /* Correct TRansfer (clear-only bit) */
#define ISTR_DOVR   (0x4000) /* PMA OVeR/underrun (clear-only bit) */
#define ISTR_ERR    (0x2000) /* ERRor (clear-only bit) */
#define ISTR_WKUP   (0x1000) /* WaKe UP (clear-only bit) */
#define ISTR_SUSP   (0x0800) /* SUSPend (clear-only bit) */
#define ISTR_RESET  (0x0400) /* RESET (clear-only bit) */
#define ISTR_SOF    (0x0200) /* Start Of Frame (clear-only bit) */
#define ISTR_ESOF   (0x0100) /* Expected Start Of Frame (clear-only bit) */


#define ISTR_DIR    (0x0010)  /* DIRection of transaction (read-only bit)  */
#define ISTR_EP_ID  (0x000F)  /* EndPoint IDentifier (read-only bit)  */

#define CLR_CTR    (~ISTR_CTR)   /* clear Correct TRansfer bit */
#define CLR_DOVR   (~ISTR_DOVR)  /* clear DMA OVeR/underrun bit*/
#define CLR_ERR    (~ISTR_ERR)   /* clear ERRor bit */
#define CLR_WKUP   (~ISTR_WKUP)  /* clear WaKe UP bit     */
#define CLR_SUSP   (~ISTR_SUSP)  /* clear SUSPend bit     */
#define CLR_RESET  (~ISTR_RESET) /* clear RESET bit      */
#define CLR_SOF    (~ISTR_SOF)   /* clear Start Of Frame bit   */
#define CLR_ESOF   (~ISTR_ESOF)  /* clear Expected Start Of Frame bit */

/******************************************************************************/
/*             CNTR control register bits definitions                         */
/******************************************************************************/
#define CNTR_CTRM   (0x8000) /* Correct TRansfer Mask */
#define CNTR_DOVRM  (0x4000) /* DMA OVeR/underrun Mask */
#define CNTR_ERRM   (0x2000) /* ERRor Mask */
#define CNTR_WKUPM  (0x1000) /* WaKe UP Mask */
#define CNTR_SUSPM  (0x0800) /* SUSPend Mask */
#define CNTR_RESETM (0x0400) /* RESET Mask   */
#define CNTR_SOFM   (0x0200) /* Start Of Frame Mask */
#define CNTR_ESOFM  (0x0100) /* Expected Start Of Frame Mask */


#define CNTR_RESUME (0x0010) /* RESUME request */
#define CNTR_FSUSP  (0x0008) /* Force SUSPend */
#define CNTR_LPMODE (0x0004) /* Low-power MODE */
#define CNTR_PDWN   (0x0002) /* Power DoWN */
#define CNTR_FRES   (0x0001) /* Force USB RESet */

/******************************************************************************/
/*                FNR Frame Number Register bit definitions                   */
/******************************************************************************/
#define FNR_RXDP (0x8000) /* status of D+ data line */
#define FNR_RXDM (0x4000) /* status of D- data line */
#define FNR_LCK  (0x2000) /* LoCKed */
#define FNR_LSOF (0x1800) /* Lost SOF */
#define FNR_FN   (0x07FF) /* Frame Number */
/******************************************************************************/
/*               DADDR Device ADDRess bit definitions                         */
/******************************************************************************/
#define DADDR_EF (0x80)
#define DADDR_ADD (0x7F)
/******************************************************************************/
/*                            Endpoint register                               */
/******************************************************************************/
/* bit positions */
#define EP_CTR_RX      (0x8000) /* EndPoint Correct TRansfer RX */
#define EP_DTOG_RX     (0x4000) /* EndPoint Data TOGGLE RX */
#define EPRX_STAT      (0x3000) /* EndPoint RX STATus bit field */
#define EP_SETUP       (0x0800) /* EndPoint SETUP */
#define EP_T_FIELD     (0x0600) /* EndPoint TYPE */
#define EP_KIND        (0x0100) /* EndPoint KIND */
#define EP_CTR_TX      (0x0080) /* EndPoint Correct TRansfer TX */
#define EP_DTOG_TX     (0x0040) /* EndPoint Data TOGGLE TX */
#define EPTX_STAT      (0x0030) /* EndPoint TX STATus bit field */
#define EPADDR_FIELD   (0x000F) /* EndPoint ADDRess FIELD */

/* EndPoint REGister MASK (no toggle fields) */
#define EPREG_MASK     (EP_CTR_RX|EP_SETUP|EP_T_FIELD|EP_KIND|EP_CTR_TX|EPADDR_FIELD)

/* EP_TYPE[1:0] EndPoint TYPE */
#define EP_TYPE_MASK   (0x0600) /* EndPoint TYPE Mask */
#define EP_BULK        (0x0000) /* EndPoint BULK */
#define EP_CONTROL     (0x0200) /* EndPoint CONTROL */
#define EP_ISOCHRONOUS (0x0400) /* EndPoint ISOCHRONOUS */
#define EP_INTERRUPT   (0x0600) /* EndPoint INTERRUPT */
#define EP_T_MASK      (~EP_T_FIELD & EPREG_MASK)


/* EP_KIND EndPoint KIND */
#define EPKIND_MASK    (~EP_KIND & EPREG_MASK)

/* STAT_TX[1:0] STATus for TX transfer */
#define EP_TX_DIS      (0x0000) /* EndPoint TX DISabled */
#define EP_TX_STALL    (0x0010) /* EndPoint TX STALLed */
#define EP_TX_NAK      (0x0020) /* EndPoint TX NAKed */
#define EP_TX_VALID    (0x0030) /* EndPoint TX VALID */
#define EPTX_DTOG1     (0x0010) /* EndPoint TX Data TOGgle bit1 */
#define EPTX_DTOG2     (0x0020) /* EndPoint TX Data TOGgle bit2 */
#define EPTX_DTOGMASK  (EPTX_STAT|EPREG_MASK)

/* STAT_RX[1:0] STATus for RX transfer */
#define EP_RX_DIS      (0x0000) /* EndPoint RX DISabled */
#define EP_RX_STALL    (0x1000) /* EndPoint RX STALLed */
#define EP_RX_NAK      (0x2000) /* EndPoint RX NAKed */
#define EP_RX_VALID    (0x3000) /* EndPoint RX VALID */
#define EPRX_DTOG1     (0x1000) /* EndPoint RX Data TOGgle bit1 */
#define EPRX_DTOG2     (0x2000) /* EndPoint RX Data TOGgle bit1 */
#define EPRX_DTOGMASK  (EPRX_STAT|EPREG_MASK)
/* Exported macro ------------------------------------------------------------*/
/* SetCNTR */
#define _SetCNTR(wRegValue)  (*CNTR   = (UINT16)wRegValue)

/* SetISTR */
#define _SetISTR(wRegValue)  (*ISTR   = (UINT16)wRegValue)

/* SetDADDR */
#define _SetDADDR(wRegValue) (*DADDR  = (UINT16)wRegValue)

/* SetBTABLE */
#define _SetBTABLE(wRegValue)(*BTABLE = (UINT16)(wRegValue & 0xFFF8))

/* GetCNTR */
#define _GetCNTR()   ((UINT16) *CNTR)

/* GetISTR */
#define _GetISTR()   ((UINT16) *ISTR)

/* GetFNR */
#define _GetFNR()    ((UINT16) *FNR)

/* GetDADDR */
#define _GetDADDR()  ((UINT16) *DADDR)

/* GetBTABLE */
#define _GetBTABLE() ((UINT16) *BTABLE)

/* SetENDPOINT Register */
#define _SetENDPOINT(bEpNum,wRegValue)  (*(EP0REG + bEpNum)= \
    (UINT16)wRegValue)

/* GetENDPOINT Register */
#define _GetENDPOINT(bEpNum)     ((UINT16)(*(EP0REG + bEpNum)))

/*******************************************************************************
* Macro Name     : SetEPType
* Description    : sets the type in the endpoint register(bits EP_TYPE[1:0])
* Input          : bEpNum: Endpoint Number. 
*                  wType
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPType(bEpNum,wType) (_SetENDPOINT(bEpNum,\
                                 ((_GetENDPOINT(bEpNum) & EP_T_MASK) | wType)))

/*******************************************************************************
* Macro Name     : GetEPType
* Description    : gets the type in the endpoint register(bits EP_TYPE[1:0]) 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint Type
*******************************************************************************/
#define _GetEPType(bEpNum) (_GetENDPOINT(bEpNum) & EP_T_FIELD)

/*******************************************************************************
* Macro Name     : SetEPTxStatus
* Description    : sets the status for tx transfer (bits STAT_TX[1:0]).
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxStatus(bEpNum,wState) {\
    register UINT16 _wRegVal;       \
    _wRegVal = _GetENDPOINT(bEpNum) & EPTX_DTOGMASK;\
    /* toggle first bit ? */     \
    if((EPTX_DTOG1 & wState)!= 0)      \
      _wRegVal ^= EPTX_DTOG1;        \
    /* toggle second bit ?  */         \
    if((EPTX_DTOG2 & wState)!= 0)      \
      _wRegVal ^= EPTX_DTOG2;        \
    _SetENDPOINT(bEpNum, _wRegVal);    \
  } /* _SetEPTxStatus */

/*******************************************************************************
* Macro Name     : SetEPRxStatus
* Description    : sets the status for rx transfer (bits STAT_TX[1:0])
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPRxStatus(bEpNum,wState) {\
    register UINT16 _wRegVal;   \
    _wRegVal = _GetENDPOINT(bEpNum) & EPRX_DTOGMASK;\
    /* toggle first bit ? */  \
    if((EPRX_DTOG1 & wState)!= 0) \
      _wRegVal ^= EPRX_DTOG1;  \
    /* toggle second bit ? */  \
    if((EPRX_DTOG2 & wState)!= 0) \
      _wRegVal ^= EPRX_DTOG2;  \
    _SetENDPOINT(bEpNum, _wRegVal); \
  } /* _SetEPRxStatus */
/*******************************************************************************
* Macro Name     : GetEPTxStatus / GetEPRxStatus 
* Description    : gets the status for tx/rx transfer (bits STAT_TX[1:0]
*                  /STAT_RX[1:0])
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : status .
*******************************************************************************/
#define _GetEPTxStatus(bEpNum) ((UINT16)_GetENDPOINT(bEpNum) & EPTX_STAT)

#define _GetEPRxStatus(bEpNum) ((UINT16)_GetENDPOINT(bEpNum) & EPRX_STAT)

/*******************************************************************************
* Macro Name     : SetEPTxValid / SetEPRxValid 
* Description    : sets directly the VALID tx/rx-status into the enpoint register
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxValid(bEpNum)     (_SetEPTxStatus(bEpNum, EP_TX_VALID))

#define _SetEPRxValid(bEpNum)     (_SetEPRxStatus(bEpNum, EP_RX_VALID))

/*******************************************************************************
* Macro Name     : GetTxStallStatus / GetRxStallStatus.
* Description    : checks stall condition in an endpoint.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : TRUE = endpoint in stall condition.
*******************************************************************************/
#define _GetTxStallStatus(bEpNum) (_GetEPTxStatus(bEpNum) \
                                   == EP_TX_STALL)
#define _GetRxStallStatus(bEpNum) (_GetEPRxStatus(bEpNum) \
                                   == EP_RX_STALL)

/*******************************************************************************
* Macro Name     : SetEP_KIND / ClearEP_KIND.
* Description    : set & clear EP_KIND bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEP_KIND(bEpNum)    (_SetENDPOINT(bEpNum, \
                                (_GetENDPOINT(bEpNum) | EP_KIND) & EPREG_MASK))
#define _ClearEP_KIND(bEpNum)  (_SetENDPOINT(bEpNum, \
                                (_GetENDPOINT(bEpNum) & EPKIND_MASK)))

/*******************************************************************************
* Macro Name     : Set_Status_Out / Clear_Status_Out.
* Description    : Sets/clears directly STATUS_OUT bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _Set_Status_Out(bEpNum)    _SetEP_KIND(bEpNum)
#define _Clear_Status_Out(bEpNum)  _ClearEP_KIND(bEpNum)

/*******************************************************************************
* Macro Name     : SetEPDoubleBuff / ClearEPDoubleBuff.
* Description    : Sets/clears directly EP_KIND bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDoubleBuff(bEpNum)   _SetEP_KIND(bEpNum)
#define _ClearEPDoubleBuff(bEpNum) _ClearEP_KIND(bEpNum)

/*******************************************************************************
* Macro Name     : ClearEP_CTR_RX / ClearEP_CTR_TX.
* Description    : Clears bit CTR_RX / CTR_TX in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _ClearEP_CTR_RX(bEpNum)   (_SetENDPOINT(bEpNum,\
                                   _GetENDPOINT(bEpNum) & 0x7FFF & EPREG_MASK))
#define _ClearEP_CTR_TX(bEpNum)   (_SetENDPOINT(bEpNum,\
                                   _GetENDPOINT(bEpNum) & 0xFF7F & EPREG_MASK))

/*******************************************************************************
* Macro Name     : ToggleDTOG_RX / ToggleDTOG_TX .
* Description    : Toggles DTOG_RX / DTOG_TX bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _ToggleDTOG_RX(bEpNum)    (_SetENDPOINT(bEpNum, \
                                   EP_DTOG_RX | _GetENDPOINT(bEpNum) & EPREG_MASK))
#define _ToggleDTOG_TX(bEpNum)    (_SetENDPOINT(bEpNum, \
                                   EP_DTOG_TX | _GetENDPOINT(bEpNum) & EPREG_MASK))

/*******************************************************************************
* Macro Name     : ClearDTOG_RX / ClearDTOG_TX.
* Description    : Clears DTOG_RX / DTOG_TX bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _ClearDTOG_RX(bEpNum)  if((_GetENDPOINT(bEpNum) & EP_DTOG_RX) != 0)\
    _ToggleDTOG_RX(bEpNum)
#define _ClearDTOG_TX(bEpNum)  if((_GetENDPOINT(bEpNum) & EP_DTOG_TX) != 0)\
    _ToggleDTOG_TX(bEpNum)
/*******************************************************************************
* Macro Name     : SetEPAddress.
* Description    : Sets address in an endpoint register.
* Input          : bEpNum: Endpoint Number.
*                  bAddr: Address. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPAddress(bEpNum,bAddr) _SetENDPOINT(bEpNum,\
    _GetENDPOINT(bEpNum) & EPREG_MASK | bAddr)

/*******************************************************************************
* Macro Name     : GetEPAddress.
* Description    : Gets address in an endpoint register.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _GetEPAddress(bEpNum) ((UINT8)(_GetENDPOINT(bEpNum) & EPADDR_FIELD))

#define _pEPTxAddr(bEpNum) ((UINT32 *)((_GetBTABLE()+bEpNum*8  )*2 + PMAAddr))
#define _pEPTxCount(bEpNum) ((UINT32 *)((_GetBTABLE()+bEpNum*8+2)*2 + PMAAddr))
#define _pEPRxAddr(bEpNum) ((UINT32 *)((_GetBTABLE()+bEpNum*8+4)*2 + PMAAddr))
#define _pEPRxCount(bEpNum) ((UINT32 *)((_GetBTABLE()+bEpNum*8+6)*2 + PMAAddr))

/*******************************************************************************
* Macro Name     : SetEPTxAddr / SetEPRxAddr.
* Description    : sets address of the tx/rx buffer.
* Input          : bEpNum: Endpoint Number.
*                  wAddr: address to be set (must be word aligned).
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxAddr(bEpNum,wAddr) (*_pEPTxAddr(bEpNum) = ((wAddr >> 1) << 1))
#define _SetEPRxAddr(bEpNum,wAddr) (*_pEPRxAddr(bEpNum) = ((wAddr >> 1) << 1))

/*******************************************************************************
* Macro Name     : GetEPTxAddr / GetEPRxAddr.
* Description    : Gets address of the tx/rx buffer.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : address of the buffer.
*******************************************************************************/
#define _GetEPTxAddr(bEpNum) ((UINT16)*_pEPTxAddr(bEpNum))
#define _GetEPRxAddr(bEpNum) ((UINT16)*_pEPRxAddr(bEpNum))

/*******************************************************************************
* Macro Name     : SetEPCountRxReg.
* Description    : Sets counter of rx buffer with no. of blocks.
* Input          : pdwReg: pointer to counter.
*                  wCount: Counter.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _BlocksOf32(dwReg,wCount,wNBlocks) {\
    wNBlocks = wCount >> 5;\
    if((wCount & 0x1f) == 0)\
      wNBlocks--;\
    *pdwReg = (UINT32)((wNBlocks << 10) | 0x8000);\
  }/* _BlocksOf32 */

#define _BlocksOf2(dwReg,wCount,wNBlocks) {\
    wNBlocks = wCount >> 1;\
    if((wCount & 0x1) != 0)\
      wNBlocks++;\
    *pdwReg = (UINT32)(wNBlocks << 10);\
  }/* _BlocksOf2 */

#define _SetEPCountRxReg(dwReg,wCount)  {\
    UINT16 wNBlocks;\
    if(wCount > 62){_BlocksOf32(dwReg,wCount,wNBlocks);}\
    else {_BlocksOf2(dwReg,wCount,wNBlocks);}\
  }/* _SetEPCountRxReg */



#define _SetEPRxDblBuf0Count(bEpNum,wCount) {\
    UINT32 *pdwReg = _pEPTxCount(bEpNum); \
    _SetEPCountRxReg(pdwReg, wCount);\
  }
/*******************************************************************************
* Macro Name     : SetEPTxCount / SetEPRxCount.
* Description    : sets counter for the tx/rx buffer.
* Input          : bEpNum: endpoint number.
*                  wCount: Counter value.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxCount(bEpNum,wCount) (*_pEPTxCount(bEpNum) = wCount)
#define _SetEPRxCount(bEpNum,wCount) {\
    UINT32 *pdwReg = _pEPRxCount(bEpNum); \
    _SetEPCountRxReg(pdwReg, wCount);\
  }
/*******************************************************************************
* Macro Name     : GetEPTxCount / GetEPRxCount.
* Description    : gets counter of the tx buffer.
* Input          : bEpNum: endpoint number.
* Output         : None.
* Return         : Counter value.
*******************************************************************************/
#define _GetEPTxCount(bEpNum)((UINT16)(*_pEPTxCount(bEpNum)) & 0x3ff)
#define _GetEPRxCount(bEpNum)((UINT16)(*_pEPRxCount(bEpNum)) & 0x3ff)

/*******************************************************************************
* Macro Name     : SetEPDblBuf0Addr / SetEPDblBuf1Addr.
* Description    : Sets buffer 0/1 address in a double buffer endpoint.
* Input          : bEpNum: endpoint number.
*                : wBuf0Addr: buffer 0 address.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDblBuf0Addr(bEpNum,wBuf0Addr) {_SetEPTxAddr(bEpNum, wBuf0Addr);}
#define _SetEPDblBuf1Addr(bEpNum,wBuf1Addr) {_SetEPRxAddr(bEpNum, wBuf1Addr);}

/*******************************************************************************
* Macro Name     : SetEPDblBuffAddr.
* Description    : Sets addresses in a double buffer endpoint.
* Input          : bEpNum: endpoint number.
*                : wBuf0Addr: buffer 0 address.
*                : wBuf1Addr = buffer 1 address.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDblBuffAddr(bEpNum,wBuf0Addr,wBuf1Addr) { \
    _SetEPDblBuf0Addr(bEpNum, wBuf0Addr);\
    _SetEPDblBuf1Addr(bEpNum, wBuf1Addr);\
  } /* _SetEPDblBuffAddr */

/*******************************************************************************
* Macro Name     : GetEPDblBuf0Addr / GetEPDblBuf1Addr.
* Description    : Gets buffer 0/1 address of a double buffer endpoint.
* Input          : bEpNum: endpoint number.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _GetEPDblBuf0Addr(bEpNum) (_GetEPTxAddr(bEpNum))
#define _GetEPDblBuf1Addr(bEpNum) (_GetEPRxAddr(bEpNum))

/*******************************************************************************
* Macro Name     : SetEPDblBuffCount / SetEPDblBuf0Count / SetEPDblBuf1Count.
* Description    : Gets buffer 0/1 address of a double buffer endpoint.
* Input          : bEpNum: endpoint number.
*                : bDir: endpoint dir  EP_DBUF_OUT = OUT 
*                                      EP_DBUF_IN  = IN 
*                : wCount: Counter value    
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDblBuf0Count(bEpNum, bDir, wCount)  { \
    if(bDir == EP_DBUF_OUT)\
      /* OUT endpoint */ \
    {_SetEPRxDblBuf0Count(bEpNum,wCount);} \
    else if(bDir == EP_DBUF_IN)\
      /* IN endpoint */ \
      *_pEPTxCount(bEpNum) = (UINT32)wCount;  \
  } /* SetEPDblBuf0Count*/

#define _SetEPDblBuf1Count(bEpNum, bDir, wCount)  { \
    if(bDir == EP_DBUF_OUT)\
      /* OUT endpoint */ \
    {_SetEPRxCount(bEpNum,wCount);}\
    else if(bDir == EP_DBUF_IN)\
      /* IN endpoint */\
      *_pEPRxCount(bEpNum) = (UINT32)wCount; \
  } /* SetEPDblBuf1Count */

#define _SetEPDblBuffCount(bEpNum, bDir, wCount) {\
    _SetEPDblBuf0Count(bEpNum, bDir, wCount); \
    _SetEPDblBuf1Count(bEpNum, bDir, wCount); \
  } /* _SetEPDblBuffCount  */

/*******************************************************************************
* Macro Name     : GetEPDblBuf0Count / GetEPDblBuf1Count.
* Description    : Gets buffer 0/1 rx/tx counter for double buffering.
* Input          : bEpNum: endpoint number.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _GetEPDblBuf0Count(bEpNum) (_GetEPTxCount(bEpNum))
#define _GetEPDblBuf1Count(bEpNum) (_GetEPRxCount(bEpNum))


/* External variables --------------------------------------------------------*/
extern volatile UINT16 wIstr;  /* ISTR register last read value */

/* Exported functions ------------------------------------------------------- */
void SetCNTR(UINT16 /*wRegValue*/);
void SetISTR(UINT16 /*wRegValue*/);
void SetDADDR(UINT16 /*wRegValue*/);
void SetBTABLE(UINT16 /*wRegValue*/);
void SetBTABLE(UINT16 /*wRegValue*/);
UINT16 GetCNTR(void);
UINT16 GetISTR(void);
UINT16 GetFNR(void);
UINT16 GetDADDR(void);
UINT16 GetBTABLE(void);
void SetENDPOINT(UINT8 /*bEpNum*/, UINT16 /*wRegValue*/);
UINT16 GetENDPOINT(UINT8 /*bEpNum*/);
void SetEPType(UINT8 /*bEpNum*/, UINT16 /*wType*/);
UINT16 GetEPType(UINT8 /*bEpNum*/);
void SetEPTxStatus(UINT8 /*bEpNum*/, UINT16 /*wState*/);
void SetEPRxStatus(UINT8 /*bEpNum*/, UINT16 /*wState*/);
void SetDouBleBuffEPStall(UINT8 /*bEpNum*/, UINT8 bDir);
UINT16 GetEPTxStatus(UINT8 /*bEpNum*/);
UINT16 GetEPRxStatus(UINT8 /*bEpNum*/);
void SetEPTxValid(UINT8 /*bEpNum*/);
void SetEPRxValid(UINT8 /*bEpNum*/);
UINT16 GetTxStallStatus(UINT8 /*bEpNum*/);
UINT16 GetRxStallStatus(UINT8 /*bEpNum*/);
void SetEP_KIND(UINT8 /*bEpNum*/);
void ClearEP_KIND(UINT8 /*bEpNum*/);
void Set_Status_Out(UINT8 /*bEpNum*/);
void Clear_Status_Out(UINT8 /*bEpNum*/);
void SetEPDoubleBuff(UINT8 /*bEpNum*/);
void ClearEPDoubleBuff(UINT8 /*bEpNum*/);
void ClearEP_CTR_RX(UINT8 /*bEpNum*/);
void ClearEP_CTR_TX(UINT8 /*bEpNum*/);
void ToggleDTOG_RX(UINT8 /*bEpNum*/);
void ToggleDTOG_TX(UINT8 /*bEpNum*/);
void ClearDTOG_RX(UINT8 /*bEpNum*/);
void ClearDTOG_TX(UINT8 /*bEpNum*/);
void SetEPAddress(UINT8 /*bEpNum*/, UINT8 /*bAddr*/);
UINT8 GetEPAddress(UINT8 /*bEpNum*/);
void SetEPTxAddr(UINT8 /*bEpNum*/, UINT16 /*wAddr*/);
void SetEPRxAddr(UINT8 /*bEpNum*/, UINT16 /*wAddr*/);
UINT16 GetEPTxAddr(UINT8 /*bEpNum*/);
UINT16 GetEPRxAddr(UINT8 /*bEpNum*/);
void SetEPCountRxReg(UINT32 * /*pdwReg*/, UINT16 /*wCount*/);
void SetEPTxCount(UINT8 /*bEpNum*/, UINT16 /*wCount*/);
void SetEPRxCount(UINT8 /*bEpNum*/, UINT16 /*wCount*/);
UINT16 GetEPTxCount(UINT8 /*bEpNum*/);
UINT16 GetEPRxCount(UINT8 /*bEpNum*/);
void SetEPDblBuf0Addr(UINT8 /*bEpNum*/, UINT16 /*wBuf0Addr*/);
void SetEPDblBuf1Addr(UINT8 /*bEpNum*/, UINT16 /*wBuf1Addr*/);
void SetEPDblBuffAddr(UINT8 /*bEpNum*/, UINT16 /*wBuf0Addr*/, UINT16 /*wBuf1Addr*/);
UINT16 GetEPDblBuf0Addr(UINT8 /*bEpNum*/);
UINT16 GetEPDblBuf1Addr(UINT8 /*bEpNum*/);
void SetEPDblBuffCount(UINT8 /*bEpNum*/, UINT8 /*bDir*/, UINT16 /*wCount*/);
void SetEPDblBuf0Count(UINT8 /*bEpNum*/, UINT8 /*bDir*/, UINT16 /*wCount*/);
void SetEPDblBuf1Count(UINT8 /*bEpNum*/, UINT8 /*bDir*/, UINT16 /*wCount*/);
UINT16 GetEPDblBuf0Count(UINT8 /*bEpNum*/);
UINT16 GetEPDblBuf1Count(UINT8 /*bEpNum*/);
EP_DBUF_DIR GetEPDblBufDir(UINT8 /*bEpNum*/);
void FreeUserBuffer(UINT8 bEpNum/*bEpNum*/, UINT8 bDir);
UINT16 ToWord(UINT8, UINT8);
UINT16 ByteSwap(UINT16);

#endif /* __USB_REGS_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
