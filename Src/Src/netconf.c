/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   Network connection configuration
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
#include <string.h>
#include "datatype.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "netconf.h"

/* Private typedef -----------------------------------------------------------*/
#define MAX_DHCP_TRIES      4
#define SELECTED            1
#define NOT_SELECTED        (!SELECTED)
#define CLIENTMAC6          2

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif netif;
UINT32 TCPTimer = 0;
UINT32 ARPTimer = 0;

#ifdef LWIP_DHCP
UINT32 DHCPfineTimer = 0;
UINT32 DHCPcoarseTimer = 0;
static UINT32 IPaddress = 0;
#endif

UINT32 DisplayTimer = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


//================================================================
// User command process handler.
//
static INT8 CmdProcess(UINT8 *inbuffer,msg_pkg_t *outmsg)
{
    cmd_header_t *header=(cmd_header_t*)inbuffer;
    switch(header->cmdid)
    {
    case 1://Get Input & Output Status
        header = (cmd_header_t*)outmsg->buffer; 
        header->headertag[0]=HEADER_TAG;
        header->headertag[1]=HEADER_TAG;
        header->length = 8;
        header->cmdid=1;
        outmsg->size = sizeof(cmd_header_t)+8;
        break;
    case 2://Set Output Status
        header = (cmd_header_t*)outmsg->buffer;
        outmsg->size = sizeof(cmd_header_t)+1;
        memcpy(outmsg->buffer,inbuffer,sizeof(cmd_header_t));
        break;
    default:
        outmsg->size = sizeof(cmd_header_t)+1;
        memcpy(outmsg->buffer,inbuffer,sizeof(cmd_header_t));
        break;
    }
    return 0;
}

/**
  * @brief  This function is called when an error occurs on the connection 
  * @param  arg
  * @parm   err
  * @retval None 
  */
static void SockConnErr(void *arg, err_t err)
{
    //struct name *name;
    //name = (struct name *)arg;
    //mem_free(name);
}

/**
  * @brief  Called when a data is received on the telnet connection
  * @param  arg the user argument
  * @param  pcb the tcp_pcb that has received the data
  * @param  p   the packet buffer
  * @param  err the error value linked with the received data
  * @retval error value
  */
static err_t SockRecv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    struct pbuf *q;
    msg_pkg_t outmsg;

    /* We perform here any necessary processing on the pbuf */
    if (p != NULL) 
    {
        /* We call this function to tell the LwIp that we have processed the data */
        /* This lets the stack advertise a larger window, so more data can be received*/
        tcp_recved(pcb, p->tot_len);

        for(q=p; q != NULL; q = q->next)
        {
            CmdProcess(q->payload,&outmsg);
            tcp_write(pcb,outmsg.buffer,outmsg.size,TCP_WRITE_FLAG_COPY);
        }
        /* End of processing, we free the pbuf */
        pbuf_free(p);
    }
    else if (err == ERR_OK)
    {
        /* When the pbuf is NULL and the err is ERR_OK, the remote end is closing the connection. */
        /* We free the allocated memory and we close the connection */
        return tcp_close(pcb);
    }
    return ERR_OK;
}

/**
  * @brief  This function when the Telnet connection is established
  * @param  arg  user supplied argument 
  * @param  pcb  the tcp_pcb which accepted the connection
  * @param  err  error value
  * @retval ERR_OK
  */
static err_t SockAccept(void *arg, struct tcp_pcb *pcb, err_t err)
{
    /* Tell LwIP to associate this structure with this connection. */
    /* Configure LwIP to use our call back functions. */
    tcp_err(pcb, SockConnErr);
    tcp_recv(pcb, SockRecv);
    return ERR_OK;
}


/**
  * @brief  Called when a frame is received
  * @param  None
  * @retval None
  */
void LwIP_Pkt_Handle(void)
{
    /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
    ethernetif_input(&netif);
}

/**
  * @brief  LwIP periodic tasks
  * @param  localtime the current LocalTime value
  * @retval None
  */
void LwIP_Periodic_Handle(UINT32 localtime)
{
    /* TCP periodic process every 250 ms */
    if (localtime - TCPTimer >= 250)
    {
//===================================================================
        if (IPaddress != netif.ip_addr.addr)
        {
            UINT8 iptab[4]={0};
            IPaddress = netif.ip_addr.addr;/* Read the new IP address */
            iptab[0] = (UINT8)(IPaddress >> 24);
            iptab[1] = (UINT8)(IPaddress >> 16);
            iptab[2] = (UINT8)(IPaddress >> 8);
            iptab[3] = (UINT8)(IPaddress);
#if LWIP_DHCP
            if (netif.flags & NETIF_FLAG_DHCP)
            {
                 printf("DHCP IP as server : %d.%d.%d.%d \r\n", iptab[3], iptab[2], iptab[1], iptab[0]);
            }
            else
#endif
            {
                printf("Static IP:%d.%d.%d.%d\r\n", iptab[3],iptab[2],iptab[1],iptab[0]);
            }
        }
#if LWIP_DHCP
        else if (IPaddress == 0)
        {
            /* We still waiting for the DHCP server */
            printf("Looking for DHCP server please wait...  \r\n");

            /* If no response from a DHCP server for MAX_DHCP_TRIES times */
            /* stop the dhcp client and set a static IP address */
            if (netif.dhcp->tries > MAX_DHCP_TRIES)
            {
                struct ip_addr ipaddr;
                struct ip_addr netmask;
                struct ip_addr gw;

                printf("    DHCP timeout    \r\n");
                dhcp_stop(&netif);
                IP4_ADDR(&ipaddr, 192, 168, 0, 2);
                IP4_ADDR(&netmask, 255, 255, 255, 0);
                IP4_ADDR(&gw, 192, 168, 0, 1);
                netif_set_addr(&netif, &ipaddr , &netmask, &gw);
            }
        }
#endif
//===================================================================
        TCPTimer =  localtime;
        tcp_tmr();
    }
    /* ARP periodic process every 5s */
    if (localtime - ARPTimer >= ARP_TMR_INTERVAL)
    {
        ARPTimer =  localtime;
        etharp_tmr();
    }

#if LWIP_DHCP
    /* Fine DHCP periodic process every 500ms */
    if (localtime - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
    {
        DHCPfineTimer =  localtime;
        dhcp_fine_tmr();
    }
    /* DHCP Coarse periodic process every 60s */
    if (localtime - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
    {
        DHCPcoarseTimer =  localtime;
        dhcp_coarse_tmr();
    }
#endif
}


/**
  * @brief  Initialize the hello application  
  * @param  None 
  * @retval None 
  */
void InitSock(void)
{
    struct tcp_pcb *pcb;
    /* Create a new TCP control block  */
    pcb = tcp_new();
    /* Assign to the new pcb a local IP address and a port number */
    /* Using IP_ADDR_ANY allow the pcb to be used by any local interface */
    tcp_bind(pcb, IP_ADDR_ANY, PORT);
    /* Set the connection to the LISTEN state */
    pcb = tcp_listen(pcb);
    /* Specify the function to be called when a connection is established */  
    tcp_accept(pcb, SockAccept);   
}


/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
void InitLwIP(void)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
    UINT8 macaddress[6]={0,0,0,0,0,1};

    /* Initializes the dynamic memory heap defined by MEM_SIZE.*/
    mem_init();

    /* Initializes the memory pools defined by MEMP_NUM_x.*/
    memp_init();

#if LWIP_DHCP
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;

    /* Configure the board opeartin mode: Client/Server */  
    //Server = SELECTED;
#else
    IP4_ADDR(&ipaddr, 192, 168, 0, 2);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gw, 192, 168, 0, 1);
#endif

    Set_MAC_Address(macaddress);

    /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            struct ip_addr *netmask, struct ip_addr *gw,
            void *state, err_t (* init)(struct netif *netif),
            err_t (* input)(struct pbuf *p, struct netif *netif))

    Adds your network interface to the netif_list. Allocate a struct
    netif and pass a pointer to this structure as the first argument.
    Give pointers to cleared ip_addr structures when using DHCP,
    or fill them with sane numbers otherwise. The state pointer may be NULL.

    The init function pointer must point to a initialization function for
    your ethernet netif interface. The following code illustrates it's use.*/
    netif_add(&netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    /*  Registers the default network interface.*/
    netif_set_default(&netif);

#if LWIP_DHCP
    /*  Creates a new DHCP client for this interface on the first call.
    Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
    the predefined regular intervals after starting the client.
    You can peek in the netif->dhcp struct for the actual DHCP status.*/
    dhcp_start(&netif);
#endif

    /*  When the netif is fully configured this function must be called.*/
    netif_set_up(&netif);
}


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
