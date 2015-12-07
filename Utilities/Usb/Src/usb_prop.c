/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : All processings related to Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_rcc.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "usb_tmc.h"
#include "debug.h"

UINT8 capability = 1;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UINT32 ProtocolValue;
tagTmcSpecRequest gTmcSpecReq={0};
static UINT8 gCtrlCmdBuffer[24]={0};

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
{
  EP_NUM,
  1
};

DEVICE_PROP Device_Property =
{
  TMC_init,
  TMC_Reset,
  TMC_Status_In,
  TMC_Status_Out,
  TMC_Data_Setup,
  TMC_NoData_Setup,
  TMC_Get_Interface_Setting,
  TMC_GetDeviceDescriptor,
  TMC_GetConfigDescriptor,
  TMC_GetStringDescriptor,
  0,
  0xFF,/*MAX PACKET SIZE*/
};

/* 
  * usb standard request command 
  * Ref USB Spec 2.0 -- 9.4[Standard Device Requests]
  */
USER_STANDARD_REQUESTS User_Standard_Requests =
{
  CustomHID_GetConfiguration,
  TMC_SetConfiguration,
  CustomHID_GetInterface,
  CustomHID_SetInterface,
  CustomHID_GetStatus,
  CustomHID_ClearFeature,
  CustomHID_SetEndPointFeature,
  CustomHID_SetDeviceFeature,
  TMC_SetDeviceAddress
};

ONE_DESCRIPTOR Device_Descriptor =
{
  (UINT8*)TMC_DeviceDescriptor,
  TMC_SIZ_DEVICE_DESC
};

ONE_DESCRIPTOR Config_Descriptor =
{
  (UINT8*)TMC_ConfigDescriptor,
  TMC_SIZ_CONFIG_DESC
};


ONE_DESCRIPTOR String_Descriptor[6] =
{
  {(UINT8*)TMC_StringLangID, TMC_SIZ_STRING_LANGID},
  {(UINT8*)TMC_StringVendor, TMC_SIZ_STRING_VENDOR},
  {(UINT8*)TMC_StringProduct,TMC_SIZ_STRING_PRODUCT},
  {(UINT8*)TMC_StringSerial, TMC_SIZ_STRING_SERIAL},
  {(UINT8*)TMC_StringConfiguration,TMC_SIZ_STRING_CONFIGURE},
  {(UINT8*)TMC_StrintInterface,TMC_SIZ_STRING_INTERFACE},
};

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : CustomHID_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
UINT8 *TMC_GetDeviceDescriptor(UINT16 Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : CustomHID_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
UINT8 *TMC_GetConfigDescriptor(UINT16 Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : CustomHID_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
UINT8 *TMC_GetStringDescriptor(UINT16 Length)
{
  UINT8 wValue0 = pInformation->USBwValue0;
  if (wValue0 > 5)
  {
    return NULL;
  }
  else 
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : CustomHID_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT TMC_Get_Interface_Setting(UINT8 Interface, UINT8 AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_SetProtocol
* Description    : Joystick Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT TMC_SetProtocol(void)
{
  UINT8 wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protcol value.
*******************************************************************************/
UINT8 *TMC_GetProtocolValue(UINT16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (UINT8 *)(&ProtocolValue);
  }
}

UINT8 *TMC_initiate_abort_bulk_out(UINT16 Length)
{
    gTmcLayerInfo.rxState = TMC_IDLE;

    gCtrlCmdBuffer[0] = STATUS_SUCCESS;
    gCtrlCmdBuffer[1] = gTmcLayerInfo.lastTmcBulkOutHeader.bTag;

    //cmd_buffer[1]=(UINT8)pInformation->USBwValues.w &0xFF;
    //
    //abort action
    pInformation->Ctrl_Info.Usb_wLength=2;
    return gCtrlCmdBuffer;
}

UINT8 *TMC_check_abort_bulk_out_status(UINT16 Length)
{
    gCtrlCmdBuffer[0]=STATUS_SUCCESS;
    gCtrlCmdBuffer[1]=0x00;
    gCtrlCmdBuffer[2]=0x00;
    gCtrlCmdBuffer[3]=0x00;

    //NBYTES_RXD
    gCtrlCmdBuffer[4]=(gTmcLayerInfo.rxCount>>0) & 0xFF;
    gCtrlCmdBuffer[5]=(gTmcLayerInfo.rxCount>>8) & 0xFF;
    gCtrlCmdBuffer[6]=(gTmcLayerInfo.rxCount>>16)& 0xFF;;
    gCtrlCmdBuffer[7]=(gTmcLayerInfo.rxCount>>24)& 0xFF;
    pInformation->Ctrl_Info.Usb_wLength=8;
    return gCtrlCmdBuffer;
}

UINT8 *TMC_initiate_abort_bulk_in(UINT16 Length)
{
    gCtrlCmdBuffer[0]=STATUS_SUCCESS;
    gCtrlCmdBuffer[1]=gTmcLayerInfo.lastTmcBulkInHeader.bTag;
    //gCtrlCmdBuffer[1]=(UINT8)pInformation->USBwValues.w &0xFF;
    //pInformation.Ctrl_Info.PacketSize=2;
    //abort action
    pInformation->Ctrl_Info.Usb_wLength=2;
    return gCtrlCmdBuffer;
}
UINT8 *TMC_check_abort_bulk_in_status(UINT16 Length)
{
    gCtrlCmdBuffer[0]=STATUS_SUCCESS;
    gCtrlCmdBuffer[1]=0x00;
    gCtrlCmdBuffer[2]=0x00;
    gCtrlCmdBuffer[3]=0x00;

    //NBYTES_RXD
    gCtrlCmdBuffer[4]=0x00;
    gCtrlCmdBuffer[5]=0x00;
    gCtrlCmdBuffer[6]=0x00;
    gCtrlCmdBuffer[7]=0x00;
    pInformation->Ctrl_Info.Usb_wLength=8;
    return gCtrlCmdBuffer;
}

UINT8 *TMC_initiate_clear(UINT16 Length)
{
    gTmcLayerInfo.rxState = TMC_IDLE;
    gCtrlCmdBuffer[0]=STATUS_SUCCESS;
    pInformation->Ctrl_Info.Usb_wLength=1;
    //
    //clear action
    return gCtrlCmdBuffer;
}

UINT8 *TMC_check_clear_status(UINT16 Length)
{
    gCtrlCmdBuffer[0]=STATUS_SUCCESS;
    gCtrlCmdBuffer[1]=0;
    //
    //abort action
    pInformation->Ctrl_Info.Usb_wLength=2;
    return gCtrlCmdBuffer;
}

UINT8 *TMC_get_capabilities(UINT16 Length)
{
    UINT8 i;
    for(i=0;i<24;i++) gCtrlCmdBuffer[i]=0;
    gCtrlCmdBuffer[0]=STATUS_SUCCESS;
    //bcdUSBTMC
    gCtrlCmdBuffer[2]=TMC_DeviceDescriptor[2];
    gCtrlCmdBuffer[3]=TMC_DeviceDescriptor[3];
    //USBTMC Interface Capabilities
    gCtrlCmdBuffer[4]=(0<<2)|(0<<1)|(0<<0);//talk-only
    gCtrlCmdBuffer[4]=0;//unsupport TermChar
    pInformation->Ctrl_Info.Usb_wLength = 0x18;
    return gCtrlCmdBuffer;
}

/*******************************************************************************
* Function Name  : CustomHID_init.
* Description    : Custom HID init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void TMC_init(void)
{
  /* Update the serial number string descriptor with the data from the unique ID*/
    Get_SerialNum();
    
    pInformation->Current_Configuration = 0;
  /* Connect the device */
    PowerOn();
  /* USB interrupts initialization */
    _SetISTR(0);       /* clear pending interrupts */

    _SetCNTR(IMR_MSK); /* set interrupts mask */

    bDeviceState = UNCONNECTED;

    //CCZY
    RCC_APB1PeriphClockCmd(0x00800000/*RCC_APB1Periph_USB*/,ENABLE); /* enable clock for USB */

    /* Enable USB Interrupts */
    USB_Interrupts_Config();/* set priority lower than SVC */
}

/*******************************************************************************
* Function Name  : CustomHID_Reset.
* Description    : Custom HID reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void TMC_Reset(void)
{
  /* Set TMC DEVICE as not configured */
     pInformation->Current_Configuration = 0;
     pInformation->Current_Interface = 0;  /*the default Interface*/
  
  /* Current Feature initialization */
     pInformation->Current_Feature = TMC_ConfigDescriptor[7];

  /* Set Buffer Table Base Address */
     SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
     SetEPType(ENDP0, EP_CONTROL);
     SetEPTxStatus(ENDP0, EP_TX_STALL);
     SetEPRxAddr(ENDP0, ENDP0_RXADDR);
     SetEPTxAddr(ENDP0, ENDP0_TXADDR);
     Clear_Status_Out(ENDP0);
     SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
     SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
     SetEPType(ENDP1, EP_BULK);
     SetEPRxAddr(ENDP1, ENDP1_RXADDR);
     SetEPRxCount(ENDP1, EP_MAXPKGSIZE);  //70 --> 84
     SetEPRxStatus(ENDP1, EP_RX_VALID);
     //SetEPTxStatus(ENDP1, EP_TX_DIS);

  /* Initialize Endpoint 2 */
     SetEPType(ENDP2, EP_BULK);
     SetEPTxAddr(ENDP2, ENDP2_TXADDR);
     SetEPTxCount(ENDP2, EP_MAXPKGSIZE);  //70 --> 84
     SetEPTxStatus(ENDP2, EP_TX_NAK);

     bDeviceState = ATTACHED;
  
  /* Set this device to response on default address */
     SetDeviceAddress(0);
}
/*******************************************************************************
* Function Name  : CustomHID_SetConfiguration.
* Description    : Udpade the device state to configured and command the ADC 
*                  conversion.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void TMC_SetConfiguration(void)
{
    if (pInformation->Current_Configuration != 0)
    {
        /* Device configured */
        bDeviceState = CONFIGURED;
    }
}
/*******************************************************************************
* Function Name  : CustomHID_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void TMC_SetDeviceAddress (void)
{
    bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : CustomHID_Status_In.
* Description    : Joystick status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void TMC_Status_In(void)
{
}

/*******************************************************************************
* Function Name  : CustomHID_Status_Out
* Description    : Joystick status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void TMC_Status_Out (void)
{
}

/*******************************************************************************
* Function Name  : CustomHID_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT TMC_Data_Setup(UINT8 RequestNo)
{
    UINT8 *(*CopyRoutine)(UINT16);
    
    CopyRoutine = NULL;
    switch(pInformation->USBbmRequestType)
    {
    case 0xA2:
        switch(RequestNo)
        {
        case 1://INITIATE_ABORT_BULK_OUT
            CopyRoutine=TMC_initiate_abort_bulk_out;
            break;
        case 2://CHECK_ABORT_BULK_OUT_STATUS
            CopyRoutine=TMC_check_abort_bulk_out_status;
            break;
        case 3://INITIATE_ABORT_BULK_IN
            CopyRoutine=TMC_initiate_abort_bulk_in;
            break;
        case 4://CHECK_ABORT_BULK_IN_STATUS
            CopyRoutine=TMC_check_abort_bulk_in_status;
            break;
        default:
            break;
        }
        
        break;
    case 0xA1:
        switch(RequestNo)
        {
        case 5://INITIATE_CLEAR
            CopyRoutine=TMC_initiate_clear;
            break;
        case 6://CHECK_CLEAR_STATUS
            CopyRoutine=TMC_check_clear_status;
            break;
        case 7://GET_CAPABILITIES
            CopyRoutine=TMC_get_capabilities;
            break;
        case 64:
            break;
        default:
            break;
        }
        break;
    }
    
    if (CopyRoutine == NULL)
    {
        return USB_UNSUPPORT;
    }
    
    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    pInformation->Ctrl_Info.Usb_wOffset = 0;
    (*CopyRoutine)(0);

    return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : CustomHID_NoData_Setup
* Description    : handle the no data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT TMC_NoData_Setup(UINT8 RequestNo)
{
    if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == SET_PROTOCOL))
    {
        return TMC_SetProtocol();
    }
    else
    {
        return USB_UNSUPPORT;
    }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
