/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Descriptor Header for Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

#include "usb_type.h"

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05


#define TMC_SIZ_DEVICE_DESC               18
#define TMC_SIZ_CONFIG_DESC               32
#define TMC_SIZ_STRING_LANGID             4
#define TMC_SIZ_STRING_VENDOR             20
#define TMC_SIZ_STRING_PRODUCT            18
#define TMC_SIZ_STRING_SERIAL             26
#define TMC_SIZ_STRING_INTERFACE          16
#define TMC_SIZ_STRING_CONFIGURE		  16


extern const UINT8 TMC_DeviceDescriptor[TMC_SIZ_DEVICE_DESC];
extern const UINT8 TMC_ConfigDescriptor[TMC_SIZ_CONFIG_DESC];
extern const UINT8 TMC_StringLangID[TMC_SIZ_STRING_LANGID];
extern const UINT8 TMC_StringVendor[TMC_SIZ_STRING_VENDOR];
extern const UINT8 TMC_StringProduct[TMC_SIZ_STRING_PRODUCT];
extern UINT8 TMC_StringSerial[TMC_SIZ_STRING_SERIAL];
extern const UINT8 TMC_StringConfiguration[TMC_SIZ_STRING_CONFIGURE];
extern const UINT8 TMC_StrintInterface[TMC_SIZ_STRING_INTERFACE];


#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
