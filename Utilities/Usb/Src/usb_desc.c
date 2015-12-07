/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Descriptors for Custom HID Demo
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
#include "usb_desc.h"

#include "usb_tmc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
  *****************************************************************
  *****************************************************************
  */
const UINT8 TMC_DeviceDescriptor[18]=
{
	0x12,		/* bLength */
	0x01,		/* bDescriptorType */
	0x00,0x02,	/* bcdUSB = 0x0200 */
	0x00,		/* bDeviceClass,Class found in interface descriptor */
	0x00,		/* bDeviceSubClass,Subclass found in interface descriptor */
	0x00,		/* bDeviceProtocol */
	0x40,		/* bMaxPacketSize0,(only 8,16,32,or 64 are valid),Ref-USB2.0 Spec-9.6.1*/
	0x83,0x04,	/* idVendor  = 0x0483 */
	0x50,0x57,	/* idProduct = 0x5750 */
	0x00,0x02,	/* bcdDevice = 0x0200,Ref-USB2.0 Spec-9.6.1 */
	0x01,		/* iManufacturer : Index of string descriptor describing manufacturer. */
	0x02,		/* iProduct : Index of string descriptor describing product. */
	0x03,		/* iSerialNumber : Index of string descriptor describing the devices's serial number. */
	0x01		/* bNumConfigurations : Ref USB2.0-9.6.1 */
};

const UINT8 TMC_ConfigDescriptor[32] = 
{
	/*------------- Configuration Descriptor -------------------*/
	0x09,		/* bLength: Size of this Desc in Bytes. */
	0x02,		/* bDescriptorType: Configuration Descriptor Type. */
	0x20,0x00,	/* wTotalLength: =config+inferface+Ep+class|vendor. */
	0x01,		/* bNumInterfaces: Number of interface supported by this configuration. */
	0x01,		/* bConfigurationValue: Value to use as an argument to the SetConfiguration() request. */
	0x04,		/* iConfiguration: Index of string describing this configuration. */
	0x80,		/* bmAttributes:
				       D7:    Reserved (set to one)
				       D6:    Self-powered
				       D5:    Rmote Wakeup
				       D4..0:Reserved (reset to zero) 
	 			  */
	0x32,		/* bMaxPower: Maximum power consumption per the USB2.0 spec. */

	/*------------- Interface 0  Descriptor ---------------------*/
	0x09,		/* bLength */
	0x04,		/* bDescriptorType */
	0x00,		/* bInterfaceNumber: Number of this interface. Zero-based value ...*/
	0x00,		/* bAlternateSetting */
	0x02,		/* bNumEndpoints: Number of endpoints used by this interface (excluding ep 0) */
	0xFE,		/* bInterfaceClass: Application-Class calss code; */
	0x03,		/* bInterfaceSubClass. */
	0x00,		/* binterfaceProtocol: 
					0=USBTMC interface, No subclass spec applies; 
					1=USBTMC USB488 interface.
				*/
	0x05,		/* iInterface: Index of string descriptor describing this interface. */

	/*------------- Endpointer IN  Descriptor ---------------------*/
	0x07,		/* bLength: Size of the descriptor in bytes. */
	0x05,		/* bDescriptorType: Endpoint Descriptor Type. */
	0x82,		/* bEndpointAddress: In endpoint.
 					Bit 3..0 = The endpoint number
 					Bit 6..4 = Reserved, reset to zero
 					Bit 7     = Direction, ignored for control endpoints
 							0:Out endpoint
 							1:In endpoint
				  */
	0x02,		/* bmAttributes: 
					Bit 1..0 : Transfer Type
						00 = Control  ------> Bit 5..2 = 0
						01 = Isochronous
						10 = Bulk----------> Bit 5..2 = 0
						11 = Interrupt------> Bit 5..2 = 0
					Bit 3..2 : Synchronization Type
					       00 = No Sync
					       01 = Async
						10 = Implicit feedback Data endpoint
						11 = Reserved
					Bit 5..4 : Usage Type
						00 = Data endpoint
						01 = Feedback endpoint
						10 = Implicit feedback Data endpoint
						11 = Reserved
				  */
    EP_MAXPKGSIZE&0xFF,(EP_MAXPKGSIZE>>8)&0x07, 
                       /* wMaxPacketSize: 0x0054= 84Byes,,Must be a multiple of 4.
					 		Bit 10..0:  maximum packet size (in bytes)
					 		Bit 12..11: for high-speediso and interrupt endpoints.
					 			00 = None (1 transaction per microframe)
					 			01 = 1 additional; 10=2 additional; 11=Reserved.
					 		Bit 15..13 : must be set to zero.
				  		*/
	0x00,		/* bInterval: Interval for polling endpoint for data transfers , (00ms) */

	/*------------- Endpointer OUT  Descriptor ---------------------*/
	0x07,		/* bLength: Size of the descriptor in bytes. */
	0x05,		/* bDescriptorType: Endpoint Descriptor Type. */
	0x01,		/* bEndpointAddress:  Out Endpoint. */
	0x02,		/* bmAttributes:  Bulk.  */
	EP_MAXPKGSIZE&0xFF,(EP_MAXPKGSIZE>>8)&0x07,	/* wMaxPacketSize: 0x0054 = 84Byes,,Must be a multiple of 4.*/
	0x10		/* bInterval: Polling Interval (16 ms). */
};


/* USB String Descriptors (optional) */
const UINT8 TMC_StringLangID[TMC_SIZ_STRING_LANGID] =
{
  TMC_SIZ_STRING_LANGID,		/* bLength: N+2; Size of this descriptor in bytes. */
  0x03,		/* bDescriptorType: 0x03; STRING Descriptor Type. */
  0x09,0x04	/* wLANGID[0]: 0x0409= U.S.English. */
}; /* LangID = 0x0409: U.S. English */

const UINT8 TMC_StringVendor[TMC_SIZ_STRING_VENDOR] =
{
  TMC_SIZ_STRING_VENDOR,		/* Size of Vendor string */
  0x03, 	/* bDescriptorType*/

  /* Manufacturer: LYS&CCZY */
  'L', 0, 'Y', 0, 'S', 0,'&', 0,'C', 0, 'C', 0, 'Z', 0,'Y',0,' ',0
};

const UINT8 TMC_StringProduct[TMC_SIZ_STRING_PRODUCT] =
{
  TMC_SIZ_STRING_PRODUCT,		/* bLength */
  0x03,		/* bDescriptorType */
  'A', 0, 'M', 0, 'E', 0, 'C', 0, '-', 0, '-', 0,'S',0,'H',0
};

//these value will be replaced by the device serial number in the function:Get_SerialNum()
UINT8 TMC_StringSerial[TMC_SIZ_STRING_SERIAL] =
{
  TMC_SIZ_STRING_SERIAL,		/* bLength */
  0x03,		/* bDescriptorType */
  '1',0,'2',0,'3',0,'4',0,'5',0,'6',0,'7',0,'8',0,'9',0,'A',0,'B',0,'C',0
};

const UINT8 TMC_StringConfiguration[TMC_SIZ_STRING_CONFIGURE]=
{
	TMC_SIZ_STRING_CONFIGURE,
	0x03,
	'A',0,'M',0,'E',0,'C',0,'-',0,'H',0,'S',0
};

const UINT8 TMC_StrintInterface[TMC_SIZ_STRING_INTERFACE]=
{
	TMC_SIZ_STRING_INTERFACE,
	0x03,
	'U',0,'S',0,'B',0,'T',0,'M',0,'C',0,' ',0
};

