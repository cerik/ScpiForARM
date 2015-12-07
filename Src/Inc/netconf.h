																			   /**
  ******************************************************************************
  * @file    netconf.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   This file contains all the functions prototypes for the netconf.c 
  *          file.
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
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NETCONF_H
#define __NETCONF_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "datatype.h"
     
/* Includes ------------------------------------------------------------------*/
void InitLwIP(void);
void InitSock(void);
void LwIP_Pkt_Handle(void);
void LwIP_Periodic_Handle(UINT32 localtime);

#define  PORT 2000

#define HEADER_TAG 0xA5

typedef struct {
    UINT8 headertag[2];
    UINT8 cmdid;
    UINT8 length;//byte length of data field.
}cmd_header_t;

typedef struct{
    UINT8 buffer[32];
    UINT16 size;
}msg_pkg_t;


#ifdef __cplusplus
}
#endif

#endif /* __NETCONF_H */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

