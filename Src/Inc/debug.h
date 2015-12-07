#ifndef __DEBUG__H__
#define __DEBUG__H__

#include <stdio.h>

#define DBG_ON  1
#define DBG_OFF 0

#define FPGA_OFF 0


/*
 * ѡ����Ժ���,1:�򿪲��Ժ���,0:�رղ��� 
 */
#define TEST_FUNC 0

/*
 * ���Կ���,1:�򿪴������������Ϣ,0:�رյ�����Ϣ
 */
#define CMD_DEBUG 0

#define APP_DEBUG 0
#define DBG_UART      DBG_OFF

#define USB_DEBUG     DBG_OFF
#define TMC_DEBUG     0
#define TMC_CMD_DEBUG 0
#define TMC_CH_READ   0
#define TMC_ERROR_LOG 1

//#define UART_LOG /* flick value and wave will be send into uart by printf,,so use uart tool debug*/

//#define D_MSG(format,args...) do{printf(format,##args);}while(0)//__VA_ARGS__);}while(0)

//#define DBG_MSG(format) D_MSG format

#define DEBUG_MSG(debug,format,args...) \
  do {\
    if((debug)&& DBG_ON)\
    {\
      printf("%s:%d --> "format,__func__,__LINE__,##args);\
    }\
  }while(0)
    



#define DEBUG_LOG(debug,format,args...) \
  do{\
    if((debug)&& DBG_ON)\
    {\
      printf(format,##args);\
    }\
  }while(0)
                              



#endif
