/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : All processings related to Custom HID demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_PROP_H
#define __USB_PROP_H

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _HID_REQUESTS
{
  GET_REPORT = 1,
  GET_IDLE,
  GET_PROTOCOL,

  SET_REPORT = 9,
  SET_IDLE,
  SET_PROTOCOL
} HID_REQUESTS;


typedef struct
{
    union{
        UINT8 bmRequstType;
        struct{
            UINT8 recipient:5;
            UINT8 type:2;
            UINT8 dir:1;
        };
    };
    UINT8 bRequest;
    UINT16 wValue;
    UINT16 wIndex;
    UINT16 wLength;
}tagTmcSpecRequest;
#define TMC_SPEC_REQ_SIZE  8  //8 byte

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TMC_init(void);
void TMC_Reset(void);
void TMC_SetConfiguration(void);
void TMC_SetDeviceAddress (void);
void TMC_Status_In (void);
void TMC_Status_Out (void);
RESULT TMC_Data_Setup(UINT8);
RESULT TMC_NoData_Setup(UINT8);
RESULT TMC_Get_Interface_Setting(UINT8 Interface, UINT8 AlternateSetting);
UINT8 *TMC_GetDeviceDescriptor(UINT16 );
UINT8 *TMC_GetConfigDescriptor(UINT16);
UINT8 *TMC_GetStringDescriptor(UINT16);
RESULT TMC_SetProtocol(void);
UINT8 *TMC_GetProtocolValue(UINT16 Length);
RESULT TMC_SetProtocol(void);
#if 0
UINT8 *TMC_GetReportDescriptor(UINT16 Length);
UINT8 *TMC_GetHIDDescriptor(UINT16 Length);
#endif

/* Exported define -----------------------------------------------------------*/
#define CustomHID_GetConfiguration          NOP_Process
//#define CustomHID_SetConfiguration          NOP_Process
#define CustomHID_GetInterface              NOP_Process
#define CustomHID_SetInterface              NOP_Process
#define CustomHID_GetStatus                 NOP_Process
#define CustomHID_ClearFeature              NOP_Process
#define CustomHID_SetEndPointFeature        NOP_Process
#define CustomHID_SetDeviceFeature          NOP_Process
//#define CustomHID_SetDeviceAddress          NOP_Process

// USB TMC Request Value
#define INITIATE_ABORT_BULK_OUT     0x01
#define CHECK_ABORT_BULK_OUT_STATUS 0x02
#define INITIATE_ABORT_BULK_IN      0x03
#define CHECK_ABORT_BULK_IN_STATUS  0x04
#define INITIATE_CLEAR      0x05
#define CHECK_CLEAR_STATUS  0x06
#define GET_CAPABILITIES    0x07

//typedef enum _TMC_STATUS
//{
#define STATUS_SUCCESS  0x01
#define STATUS_PENDING  0x02
#define STATUS_FAILED   0x80
#define STATUS_TRANSFER_NOT_IN_PROGRESS 0x81
#define STATUS_SPLIT_NOT_IN_PROGRESS    0x82
#define STATUS_SPLIT_IN_PROGRESS        0x83
//}TMC_STATUS;
#endif /* __USB_PROP_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
