
#ifndef _BSP_H_
#define _BSP_H_

#include "datatype.h"

UINT8   getch(void);
char    putch(UINT8 dat);
void    InitIWDT(UINT32 ms);
void    InitSysGpio(void);
void    InitSPI1(void);

void    InitUart1(void);
void    InitUsTimer(void);
void    InitMsTimer(void);
void    WaitUs(INT32 us);
void    WaitMs(INT32 ms);
void    InitSysTick(UINT32 ms);
UINT32  GetSysTickMs(void);
void    StartCounter(tagCounter *counter,INT32 ms);
boolean CounterArrived(tagCounter *counter);

#endif
