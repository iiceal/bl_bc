/**
  ******************************************************************************
  * @file    lwipopts.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   lwIP Options Configuration.
  *          This file is based on Utilities\lwip_v1.3.2\src\include\lwip\opt.h 
  *          and contains the lwIP configuration for the STM32F4x7 demonstration.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__
#include "common.h"
#define sys_now timer_get_tick32

/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT    0

/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */
#define NO_SYS                  1

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT           CONFIG_MEM_ALIGNMENT

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE                CONFIG_MEM_SIZE

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
#define MEMP_NUM_PBUF           CONFIG_MEMP_NUM_PBUF
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        CONFIG_MEMP_NUM_UDP_PCB
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB        CONFIG_MEMP_NUM_TCP_PCB
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN CONFIG_MEMP_NUM_TCP_PCB_LISTEN
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG        CONFIG_MEMP_NUM_TCP_SEG
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */
#define MEMP_NUM_SYS_TIMEOUT    CONFIG_MEMP_NUM_SYS_TIMEOUT

//todo start
/* The following four are used only with the sequential API and can be
   set to 0 if the application only will use the raw API. */
/* MEMP_NUM_NETBUF: the number of struct netbufs. */
//#define MEMP_NUM_NETBUF         CYGNUM_LWIP_MEMP_NUM_NETBUF
/* MEMP_NUM_NETCONN: the number of struct netconns. */
//#define MEMP_NUM_NETCONN        CYGNUM_LWIP_MEMP_NUM_NETCONN
/* MEMP_NUM_APIMSG: the number of struct api_msg, used for
   communication between the TCP/IP stack and the sequential
   programs. */
//#define MEMP_NUM_API_MSG        CYGNUM_LWIP_MEMP_NUM_APIMSG
/* MEMP_NUM_TCPIPMSG: the number of struct tcpip_msg, which is used
   for sequential API communication and incoming packets. Used in
   src/api/tcpip.c. */
//#define MEMP_NUM_TCPIP_MSG      CYGNUM_LWIP_MEMP_NUM_TCPIP_MSG
//todo end

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE          CONFIG_PBUF_POOL_SIZE

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE       CONFIG_PBUF_POOL_BUFSIZE


/* ---------- TCP options ---------- */
#define LWIP_TCP                defined(CONFIG_LWIP_TCP)
#define TCP_TTL                 CONFIG_TCP_TTL

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         defined(CONFIG_TCP_QUEUE_OOSEQ)

/* TCP Maximum segment size. */
#define TCP_MSS                 CONFIG_TCP_MSS
/* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF             (5*TCP_MSS)

/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. */
#define TCP_SND_QUEUELEN        (4* TCP_SND_BUF/TCP_MSS)

/* TCP receive window. */
#define TCP_WND                 (4*TCP_MSS)


/* ---------- ICMP options ---------- */
#define LWIP_ICMP               defined(CONFIG_LWIP_ICMP)

/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */
#define LWIP_DHCP               defined(CONFIG_LWIP_DHCP)
// todo: check require udp

/* ---------- UDP options ---------- */
#define LWIP_UDP                defined(CONFIG_LWIP_UDP)
#define UDP_TTL                 CONFIG_UDP_TTL


/* ---------- Statistics options ---------- */
#define LWIP_STATS              defined(CONFIG_LWIP_STATS)
//#define LWIP_PROVIDE_ERRNO    1


/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN            defined(CONFIG_LWIP_NETCONN)

/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */
#define LWIP_SOCKET             defined(CONFIG_LWIP_SOCKET)


/*
   ----------------------------------------
   ---------- Lwip Debug options ----------
   ----------------------------------------
*/
// #define LWIP_DEBUG                      0

#endif /* __LWIPOPTS_H__ */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
