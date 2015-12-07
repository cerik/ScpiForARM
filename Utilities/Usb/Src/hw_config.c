/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"


ErrorStatus HSEStartUpStatus;
void RTC_Configuration(void);
void Timer2Configuration(void);
extern void USB_Istr(void);


/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Set_System(void)
{ 
     GPIO_Configuration();
     Timer2Configuration();
}
void ConfigurationGPIO(void)
{
  // Configure the used GPIOs 
  GPIO_Configuration();
}
void RCC_Configuration(void)
{
}
/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
    bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else 
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
    NVIC_EnableIRQ(CAN1_RX1_IRQn);
    // enable timer2
} 
/*******************************************************************************
* Function Name : Timer2Config.
* Description   : interrupt enable
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void Timer2Configuration(void)
{
}
/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{ 
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
}
void DisableClock(void)
{
     RCC_APB1PeriphClockCmd(0x3AFEC83F/*RCC_APB1Periph_ALL*/,DISABLE);
     RCC_APB2PeriphClockCmd(0x0000FFFD/*RCC_APB2Periph_ALL*/,DISABLE);
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*  Notes:
*     For STM32F108CB, Some of these chip has problem of USB.
*     The issue is the serial number of chip.
*     So, these three line code of this function have to bee commented.
*    And that three value is detected base on good chip.
*******************************************************************************/
void Get_SerialNum(void)
{
  UINT32 Device_Serial0, Device_Serial1, Device_Serial2;
  Device_Serial0 = 0x66e0032;//*(UINT32*)(0x1FFFF7E8);
  Device_Serial1 = 0x66e0032;//*(UINT32*)(0x1FFFF7EC);
  Device_Serial2 = 0x66e0032;//*(UINT32*)(0x1FFFF7F0);
  
  if (Device_Serial0 != 0)
  {
    TMC_StringSerial[2]  = (UINT8)(Device_Serial0  & 0x000000FF);  
    TMC_StringSerial[4]  = (UINT8)((Device_Serial0 & 0x0000FF00) >> 8);
    TMC_StringSerial[6]  = (UINT8)((Device_Serial0 & 0x00FF0000) >> 16);
    TMC_StringSerial[8]  = (UINT8)((Device_Serial0 & 0xFF000000) >> 24);
    
    TMC_StringSerial[10] = (UINT8)(Device_Serial1  & 0x000000FF);  
    TMC_StringSerial[12] = (UINT8)((Device_Serial1 & 0x0000FF00) >> 8);
    TMC_StringSerial[14] = (UINT8)((Device_Serial1 & 0x00FF0000) >> 16);
    TMC_StringSerial[16] = (UINT8)((Device_Serial1 & 0xFF000000) >> 24);
    
    TMC_StringSerial[18] = (UINT8)(Device_Serial2  & 0x000000FF);  
    TMC_StringSerial[20] = (UINT8)((Device_Serial2 & 0x0000FF00) >> 8);
    TMC_StringSerial[22] = (UINT8)((Device_Serial2 & 0x00FF0000) >> 16);
    TMC_StringSerial[24] = (UINT8)((Device_Serial2 & 0xFF000000) >> 24);
  }
}
void USB_Connect (boolean con) 
{ 
  *CNTR = CNTR_FRES;                         // Force USB Reset  
  *ISTR = 0;                                 // Clear Interrupt Status  
  if (con) {
    *CNTR = CNTR_RESETM;                     // USB Reset Interrupt Mask  
  } else {
    *CNTR = CNTR_FRES | CNTR_PDWN;           // Switch Off USB Device  
  }
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
