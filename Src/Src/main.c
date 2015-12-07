/**
  ******************************************************************************
  * @file    main.c
  * @author  CCZY
  * @version V1.0.0
  * @date    2015.12.07
  * @brief   Main program body
  ******************************************************************************
  * @CompilerOption:
  *     STM32F10X_CL -----------Chip Type defination
  *     USE_STDPERIPH_DRIVER ---use standard peripheral driver
  *     LWIP -------------------use LwIP stack and its communication
  *     IWDG -------------------Enable Independent Watchdog
  *     SCPI -------------------Enable SCPI Library
  *     USB --------------------Enable USB Library
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 </center></h2>
  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "bsp.h"

#ifdef USB
#include "usb_tmc.h"
#include "usb_core.h"
#include "usb_init.h"
#include "hw_config.h"
#endif
#ifdef SCPI
#include "scpi-def.h"
#include "parser.h"
#endif
#ifdef LWIP
#include "netconf.h"
#endif

/*
 ***************************************************************
 *
 *        External Function definition
 *
 ***************************************************************
 */

/*
 ***************************************************************
 *
 *        Global Variable definition
 *
 ***************************************************************
 */
msg_buffer_t g_MsgBuffer;

/*
 ***************************************************************
 * MIAN ENTRY                                              
 ***************************************************************
 */
int main(void)
{
    UINT32 len;
    InitSysGpio();
    InitUart1();
    InitSysTick(1);
#ifdef USB
    USB_Init();
    USB_Connect(TRUE); //WDT will occurs if uncommit this function.
#endif
#ifdef IWDG
    InitIWDT(1);
#endif
    InitUsTimer();
    InitMsTimer();
#ifdef LWIP
    InitLwIP(); /* Initilaize the LwIP satck */
    InitSock(); /* Initilaize the HelloWorld module */
#endif

    printf("system ok.\n");
    while(1)
    {
#ifdef SCPI
        if(TRUE == g_MsgBuffer.rxdone)
        {
            memcpy(g_MsgBuffer.scpiBuffer,g_MsgBuffer.uartBuffer,g_MsgBuffer.size);
            len = g_MsgBuffer.size;
            g_MsgBuffer.scpiBuffer[g_MsgBuffer.size]=0;
            g_MsgBuffer.size=0;
            g_MsgBuffer.rxdone=FALSE;
            SCPI_Input(&scpi_context, (char const*)g_MsgBuffer.scpiBuffer,len);
        }
#endif
    }
}
