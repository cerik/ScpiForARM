/**
  ******************************************************************************
  * @file    stm32f107.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   STM32F107 hardware configuration
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x_iwdg.h"
#include "stm32f10x_rcc.h"
#include "stm32_eth.h"
#include "datatype.h"
#include "netconf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DP83848_PHY            /* Ethernet pins mapped on STM3210C-EVAL Board */
#define PHY_ADDRESS       0x01 /* Relative to STM3210C-EVAL Board */

//#define MII_MODE             /* MII mode for STM3210C-EVAL Board (MB784) (check jumpers setting) */
#define RMII_MODE              /* RMII mode for STM3210C-EVAL Board (MB784) (check jumpers setting) */

//===================================================================
// Local Varibale & Macro Definition
// Tick Counter from power-on;
static UINT32 gSysTickCounter=0;

#if defined(__CC_ARM)
#pragma import(__use_no_semihosting)

struct __FILE {
  int handle;   // Add whatever you need here 
};
#endif

FILE __stdout;
FILE __stdin;

//===================================================================
// Description: Get a char from UART1
// Return     : One char UART1 received 
// Note       : RXNE will be auto cleared by software read to the 
//              UART_DR register.
UINT8 getch(void)
{
    while (!(USART1->SR & USART_FLAG_RXNE));
    return (UINT8)(USART1->DR & 0x1FF);
}
char putch(UINT8 dat)
{
    while(!(USART1->SR & USART_FLAG_TXE));
    USART1->DR = (dat & 0x1FF);
    return dat;
}
int fputc(int ch, FILE *f) {
    return putch(ch);
}
int fgetc(FILE *f) {
    return getch();
}
void _ttywrch(int ch) {
    putch(ch);
}
int ferror(FILE *f) {
    return EOF;
}
void _sys_exit(int return_code) {
label:  goto label;
}

//===================================================================
//   Independent Watchdog Configure
//   IWDGCLK = 40 kHz
//   WDT Timeout = 5s
//   CNT_CLK = 40 kHz / 256 = 156.25 Hz
void InitIWDT(UINT32 ms)
{
#define __IWGDCLK     (40000UL/(0x04<<IWDG_Prescaler_256))
    UINT16 reload;
    reload = ms* 40000UL/(0x04<<IWDG_Prescaler_256)/1000UL-1;
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);//clk = 40 kHz / 256 = 156.25 Hz
    IWDG_SetReload(reload);
    IWDG_ReloadCounter();
    IWDG_Enable();
}

//===================================================================
// Function Name  : Gpio Init function.
// Description    : 
// Input          : None
// Output         : None
// Return         : None
// Attention      : None
void InitSysGpio(void)
{
}

//===========================================
// SPI1
void InitSPI1(void)
{
    SPI_InitTypeDef   SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 ,ENABLE);

    // SPI1_SCK ->PA5, SPI1_MISO->PA6, SPI1_MOSI->PA7
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
}

//===================================================================
// Function Name  : InitUart
// Description    : Configure USART1 
// Input          : None
// Output         : None
// Return         : None
// Attention      : None
void InitUart1(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure; 

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);

    //USART1_TX -> PA9 , USART1_RX -> PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure); 
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    USART_ClearFlag(USART1,USART_FLAG_TC);
    USART_Cmd(USART1, ENABLE);
}

//===================================================================
//  Tick Cycle = 1ms
//  SYSCLK = 72MHz
//  HCLK   = 72MHz
//  PCLK1  = 18MHz
//  PCLK2  = 72MHz
void InitSysTick(UINT32 ms)
{
    INT32  cnts;
    RCC_ClocksTypeDef RCC_ClocksStatus;
    RCC_GetClocksFreq(&RCC_ClocksStatus);
  
    cnts = RCC_ClocksStatus.HCLK_Frequency/8/ (1000/ms);
    SysTick->LOAD  = (cnts - 1);
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;/* Enable timer interrupt. */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* Enable timer.           */ 
}

//=================================================================
//  Free Run Timer : Timer2,configured as a free running timer.
//
//  PCLK1 = 18MHz , CLK_TIM2 = 2*PCLK1 = 36MHz
//  1. cycle = 1us.
//  2. It will support for the us delay function.
void InitUsTimer(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    RCC_GetClocksFreq(&RCC_ClocksStatus);

    TIM_DeInit(TIM2);
    TIM_TimeBaseInitStruct.TIM_Period        = 0xFFFF;
    TIM_TimeBaseInitStruct.TIM_Prescaler     = RCC_ClocksStatus.PCLK1_Frequency *2/1000000-1; 
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
    TIM_Cmd(TIM2,ENABLE);
}

//=================================================================
//  Wait Timer: Timer3 as free running timer.
//
//  PCLK1 = 18MHz , CLK_TIM3 = 2*PCLK1 = 36MHz
// Configuration:
//    1.Free Running;
//    2.Counter up mode
//    3.counter cycle is 1ms;
void InitMsTimer(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_GetClocksFreq(&RCC_ClocksStatus);

    TIM_DeInit(TIM3);    
    TIM_TimeBaseInitStruct.TIM_Period        = 0xFFFF;
    TIM_TimeBaseInitStruct.TIM_Prescaler     = RCC_ClocksStatus.PCLK1_Frequency *2/1000-1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    TIM_Cmd(TIM3,ENABLE);
}

//================================================================
// us base time delay.
// It based on the Timer 2
void WaitUs(INT32 us)
{
    UINT32 t0, t1;
    t0 = TIM_GetCounter(TIM2);;
    while ( us > 0 )
    {
        t1 = TIM_GetCounter(TIM2);;
        us -= (t1<t0)?(t1 + MAXUINT16 - t0):(t1-t0);
        t0 = t1;
    }
}

//================================================================
// The Wait MS function based on the TIMER3;
void  WaitMs(INT32 ms)
{
    UINT32 t0, t1;
    t0 = TIM_GetCounter(TIM3);
    while ( ms > 0 )
    {
        t1 = TIM_GetCounter(TIM3);
        ms -= (t1<t0)?(t1 + MAXUINT16 - t0):(t1-t0);
        t0 = t1;
    }
}

//================================================================
// *   Free Counter Function Group
// *   Counter Unit: 1ms
//
void StartCounter(tagCounter *counter,INT32 ms)
{
    counter->t0 = TIM_GetCounter(TIM3);    
    counter->ms = ms;
}

boolean CounterArrived(tagCounter *counter)
{
    if(counter->ms > 0)
    {
        counter->t1 = TIM_GetCounter(TIM3);
        counter->ms -= (counter->t1 < counter->t0)?(counter->t1 + MAXUINT32 - counter->t0):(counter->t1 - counter->t0);
        counter->t0 = counter->t1;
    }
    return (counter->ms > 0)?FALSE:TRUE;
}


//================================================================
//  * @brief  Configures the different GPIO ports.
//  * @param  None
//  * @retval None
//  *
//  * AF Output Push Pull:
//    - ETH_MII_MDIO / ETH_RMII_MDIO      : PA2
//    - ETH_MII_MDC  / ETH_RMII_MDC       : PC1
//    - ETH_MII_TXD2                      : PC2
//    - ETH_MII_TX_EN / ETH_RMII_TX_EN    : PB11
//    - ETH_MII_TXD0 / ETH_RMII_TXD0      : PB12
//    - ETH_MII_TXD1 / ETH_RMII_TXD1      : PB13
//    - ETH_MII_PPS_OUT / ETH_RMII_PPS_OUT: PB5
//    - ETH_MII_TXD3                      : PB8
//
//    - ETH_MII_CRS CRS                   : PA0
//    - ETH_MII_RX_CLK / ETH_RMII_REF_CLK : PA1
//    - ETH_MII_COL                       : PA3
//    - ETH_MII_RX_DV / ETH_RMII_CRS_DV   : PD8
//    - ETH_MII_TX_CLK                    : PC3
//    - ETH_MII_RXD0 / ETH_RMII_RXD0      : PD9
//    - ETH_MII_RXD1 / ETH_RMII_RXD1      : PD10
//    - ETH_MII_RXD2                      : PD11
//    - ETH_MII_RXD3                      : PD12
//    - ETH_MII_RX_ER                     : PB10
//
//    - ETH_MII_MCO                       : PA8
static void InitEthGPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOs and ADC1 clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO , ENABLE);
                           
    /* ETHERNET pins configuration */
    /* AF Output Push Pull:
    - ETH_MII_MDIO / ETH_RMII_MDIO      : PA2
    - ETH_MII_MDC  / ETH_RMII_MDC       : PC1
    - ETH_MII_TXD2                      : PC2
    - ETH_MII_TX_EN / ETH_RMII_TX_EN    : PB11
    - ETH_MII_TXD0 / ETH_RMII_TXD0      : PB12
    - ETH_MII_TXD1 / ETH_RMII_TXD1      : PB13
    - ETH_MII_PPS_OUT / ETH_RMII_PPS_OUT: PB5
    - ETH_MII_TXD3                      : PB8 */

    /* Configure PA2 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PC1, PC2 and PC3 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PB5, PB8, PB11, PB12 and PB13 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_11 |
                                    GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /**************************************************************/
    /*               For Remapped Ethernet pins                   */
    /*************************************************************/
    /* Input (Reset Value):
    - ETH_MII_CRS CRS                   : PA0
    - ETH_MII_RX_CLK / ETH_RMII_REF_CLK : PA1
    - ETH_MII_COL                       : PA3
    - ETH_MII_RX_DV / ETH_RMII_CRS_DV   : PD8
    - ETH_MII_TX_CLK                    : PC3
    - ETH_MII_RXD0 / ETH_RMII_RXD0      : PD9
    - ETH_MII_RXD1 / ETH_RMII_RXD1      : PD10
    - ETH_MII_RXD2                      : PD11
    - ETH_MII_RXD3                      : PD12
    - ETH_MII_RX_ER                     : PB10 */

    /* ETHERNET pins remapp in STM3210C-EVAL board: RX_DV and RxD[3:0] */
    GPIO_PinRemapConfig(GPIO_Remap_ETH, ENABLE);

    /* Configure PA0, PA1 and PA3 as input */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PB10 as input */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure PC3 as input */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure PD8, PD9, PD10, PD11 and PD12 as input */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure); /**/

    /* MCO pin configuration------------------------------------------------- */
    /* Configure MCO as alternate function push-pull : PA8 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//================================================================
//  * @brief  Configures the nested vectored interrupt controller.
//  * @param  None
//  * @retval None
//
static void InitEthNVIC(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;

    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

    /* 2 bit for pre-emption priority, 2 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

    /* Enable the Ethernet global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    
}


//================================================================
//  * @brief  Configures the Ethernet Interface
//  * @param  None
//  * @retval None
//
void InitEthernet(void)
{
    ETH_InitTypeDef ETH_InitStructure;

    /* NVIC configuration */
    InitEthNVIC();  

    /* Configure the GPIO ports */
    InitEthGPIO();
    
    /* MII/RMII Media interface selection ------------------------------------------*/
#ifdef MII_MODE /* Mode MII with STM3210C-EVAL  */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);

    /* Get HSE clock = 25MHz on PA8 pin (MCO) */
    RCC_MCOConfig(RCC_MCO_HSE);

#elif defined RMII_MODE  /* Mode RMII with STM3210C-EVAL */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_RMII);

    /* Set PLL3 clock output to 50MHz (25MHz /5 *10 =50MHz) */
    RCC_PLL3Config(RCC_PLL3Mul_10);
    /* Enable PLL3 */
    RCC_PLL3Cmd(ENABLE);
    /* Wait till PLL3 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLL3RDY) == RESET);

    /* Get PLL3 clock on PA8 pin (MCO) */
    RCC_MCOConfig(RCC_MCO_PLL3CLK);
#endif

    /* Reset ETHERNET on AHB Bus */
    ETH_DeInit();

    /* Software reset */
    ETH_SoftwareReset();

    /* Wait for software reset */
    while (ETH_GetSoftwareResetStatus() == SET);

    /* ETHERNET Configuration ------------------------------------------------------*/
    /* Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter */
    ETH_StructInit(&ETH_InitStructure);

    /* Fill ETH_InitStructure parametrs */
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable  ;
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
#ifdef CHECKSUM_BY_HARDWARE
    ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif

    /*------------------------   DMA   -----------------------------------*/  

    /* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
    the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
    if the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;         
    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;     

    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;       
    ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;   
    ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;                                                          
    ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;      
    ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;                
    ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;          
    ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;                                                                 
    ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

    /* Configure Ethernet */
    ETH_Init(&ETH_InitStructure, PHY_ADDRESS);

    /* Enable the Ethernet Rx Interrupt */
    ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);

    /*------------------Peridical Interrupt Handler-------------------------*/
    InitSysTick(1);
    /* Update the SysTick IRQ priority should be higher than the Ethernet IRQ */
    /* The Localtime should be updated during the Ethernet packets processing */
    NVIC_SetPriority (SysTick_IRQn, 1);  
}

UINT32 GetSysTickMs(void)
{
    return gSysTickCounter;
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler()
{
#ifdef IWDG
    IWDG_ReloadCounter();
#endif
    gSysTickCounter++;
    
#ifdef LWIP
    LwIP_Periodic_Handle(gSysTickCounter);
#endif
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

//================================================================
//  * @brief  This function handles ETH interrupt request.
//  * @param  None
//  * @retval None
//
void ETH_IRQHandler(void)
{
    /* Handles all the received frames */
    while(ETH_GetRxPktSize() != 0) 
    {
#ifdef LWIP
        LwIP_Pkt_Handle();
#endif
    }

    /* Clear the Eth DMA Rx IT pending bits */
    ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
}

