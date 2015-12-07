/**
  ******************************************************************************
  * @file    client.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   A sample UDP/TCP client
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
#include <string.h>
#include <stdio.h>
#include "datatype.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

/* Private typedef -----------------------------------------------------------*/
#define UDP_SERVER_PORT      7
#define UDP_CLIENT_PORT      4
#define TCP_PORT      4

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static struct tcp_pcb *TcpPCB;

/* Private function prototypes -----------------------------------------------*/
void udp_client_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, UINT16 port);
err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
void tcp_client_err(void *arg, err_t err);
err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, UINT16 len);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the client application.
  * @param  None
  * @retval None
  */
void client_init(void)
{
   struct udp_pcb *upcb;
   struct pbuf *p;
                                  
   /* Create a new UDP control block  */
   upcb = udp_new();   
   
   /* Connect the upcb  */
   udp_connect(upcb, IP_ADDR_BROADCAST, UDP_SERVER_PORT);

   p = pbuf_alloc(PBUF_TRANSPORT, 0, PBUF_RAM);

   /* Send out an UDP datagram to inform the server that we have strated a client application */
   udp_send(upcb, p);   

   /* Reset the upcb */
   udp_disconnect(upcb);
   
   /* Bind the upcb to any IP address and the UDP_PORT port*/
   udp_bind(upcb, IP_ADDR_ANY, UDP_CLIENT_PORT);
   
   /* Set a receive callback for the upcb */
   udp_recv(upcb, udp_client_callback, NULL);

   /* Free the p buffer */
   pbuf_free(p);
  
}

/**
  * @brief  This function is called when a datagram is received
   * @param arg user supplied argument (udp_pcb.recv_arg)
   * @param upcb the udp_pcb which received data
   * @param p the packet buffer that was received
   * @param addr the remote IP address from which the packet was received
   * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_client_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, UINT16 port)
{
  struct tcp_pcb *pcb;
  UINT8 iptab[4];

  /* Read the Server's IP address */
  iptab[0] = (UINT8)((UINT32)(addr->addr) >> 24);  
  iptab[1] = (UINT8)((UINT32)(addr->addr) >> 16);
  iptab[2] = (UINT8)((UINT32)(addr->addr) >> 8);
  iptab[3] = (UINT8)((UINT32)(addr->addr));

  printf("IP: %d.%d.%d.%d\r\n", iptab[3], iptab[2], iptab[1], iptab[0]);

  printf("The server's IP add.");

  /* Create a new TCP control block  */
  pcb = tcp_new();

  /* Assign to the new pcb a local IP address and a port number */
  tcp_bind(pcb, IP_ADDR_ANY, TCP_PORT);

  /* Connect to the server: send the SYN */
  tcp_connect(pcb, addr, TCP_PORT, tcp_client_connected);

  /* Free the p buffer */
  pbuf_free(p);
}

/**
  * @brief  This function is called when the connection with the remote 
  *         server is established
  * @param arg user supplied argument
  * @param tpcb the tcp_pcb which received data
  * @param err error value returned by the tcp_connect 
  * @retval error value
  */
err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  printf("Led control started ");

  TcpPCB = tpcb;
  
  return ERR_OK;
}

/**
  * @brief  Send to the server the led that should be toogled.
  * @param  Led the led that should be toogled
  * @retval None
  */
void tcp_led_control(UINT8 Led)
{
  char ledstatus;

  ledstatus = (char) Led;
  
  tcp_write(TcpPCB, &ledstatus, sizeof(ledstatus), 1);

  /* send the data right now */
  tcp_output(TcpPCB); 
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
