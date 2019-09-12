#ifndef _REGS_QSPI_H_
#define _REGS_QSPI_H_

#define REG_AP_QSPI0_BASE          (APB0_BASE+0x00000)

typedef volatile struct
{
    REG32 ctrlr0;     		   /* 0x00000000*/
    REG32 ctrlr1;      	   	   /* 0x00000004*/
    REG32 ssienr;     	 	   /* 0x00000008*/
    REG32 baudr;         	   /* 0x0000000c*/

    REG32 txftlr;	      	   /* 0x00000010*/
    REG32 rxftlr;     		   /* 0x00000014*/
    REG32 txflr;    		   /* 0x00000018*/
    REG32 rxflr;    	           /* 0x0000001c*/

    REG32 sr;	  	           /* 0x00000020*/
    REG32 imr;	   		   /* 0x00000024*/
    REG32 isr;   		   /* 0x00000028*/
    REG32 risr;    		   /* 0x0000002c*/

    REG32 txoicr; 	           /* 0x00000030*/
    REG32 rxoicr;      		   /* 0x00000034*/
    REG32 rxuicr;  	           /* 0x00000038*/
    REG32 ahbicr;      		   /* 0x0000003c*/

    REG32 icr;         		   /* 0x00000040*/
    REG32 hold_wp;     		   /* 0x00000044*/
    REG32 read_cmd;   		   /* 0x00000048*/
    REG32 pgm_cmd;   	           /* 0x0000004c*/

    REG32 cache_flush; 		   /* 0x00000050*/
    REG32 cache_dis_update;  	   /* 0x00000054*/
    REG32 txfifo_flush;  	   /* 0x00000058*/
    REG32 rxfifo_flush;    	   /* 0x0000005c*/

    REG32 dma_ctrl;		   /* 0x00000060*/
    REG32 dma_tdlr;		   /* 0x00000064*/
    REG32 dma_rdlr;		   /* 0x00000068*/	
    REG32 dr;			   /* 0x0000006c*/	
} HWP_QSPI_T;


#define hwp_apQspi0                ((HWP_QSPI_T*) (REG_AP_QSPI0_BASE))

/*ctrlr0*/
#define QSPI_CTRLR0_DFS(n)		(((n) & 0xf) << 0)
#define QSPI_CTRLR0_DFS_MASK		(0xf << 0)
#define QSPI_CTRLR0_FRF(n)		(((n) & 0x3) << 4)
#define QSPI_CTRLR0_FRF_MASK		(0x3 << 4)
#define QSPI_CTRLR0_SCPH(n)		(((n) & 1) << 6)
#define QSPI_CTRLR0_SCPH_MASK		(1 << 6)
#define QSPI_CTRLR0_SCPOL(n)		(((n) & 0x1) << 7)
#define QSPI_CTRLR0_SCPOL_MASK		(0x1 << 7)
#define QSPI_CTRLR0_TMOD(n)		(((n) & 0x3) << 8)
#define QSPI_CTRLR0_TMOD_MASK		(0x3 << 8)
#define QSPI_CTRLR0_E2PRMODE(n)		(((n) & 0x7) << 10)
#define QSPI_CTRLR0_E2PRMODE_MASK	(0x7 << 10)
#define QSPI_CTRLR0_CONTINOUS(n)	(((n) & 0x1) << 13)
#define QSPI_CTRLR0_CONTINOUS_MASK	(0x1 << 13)
#define QSPI_CTRLR0_DFS_HIGH		(0x1 << 16)

#define	QSPI_CTRLR0_DFS_BYTE		(0x7)
#define QSPI_CTRLR0_TMOD_TX		(0x1)
#define QSPI_CTRLR0_TMOD_RX		(0x3)
/*ctrlr1*/
#define QSPI_CTRLR1_NDF(n)		(((n) & 0xff) << 0)
#define QSPI_CTRLR1_NDF_MASK		(0xff << 0)

/*ssienr*/
#define QSPI_SSIENR_EN(n)		(((n) & 0x1) << 0)
#define QSPI_SSIENR_EN_MASK		(0x1 << 0)
#define QSPI_SSIENR_AHB_EN(n)		(((n) & 0x1) << 1)
#define QSPI_SSIENR_AHB_EN_MASK		(0x1 << 1)

/*baudr*/
#define QSPI_BAUDR_DIV(n)		(((n) & 0x3ff) << 0)
#define QSPI_BAUDR_DIV_MASK		(0x3ff << 0)

/*txftlr*/
#define QSPI_TXFTLR_TFT(n)		(((n) & 0xff) << 0)
#define QSPI_TXFTLR_TFT_MASK		(0xff << 0)

/*rxftlr*/
#define QSPI_RXFTLR_RFT(n)		(((n) & 0xff) << 0)
#define QSPI_RXFTLR_RFT_MASK		(0xff << 0)

/*txflr*/
#define QSPI_TXFLR_TFTFL(n)		(((n) & 0xff) << 0)

/*rxflr*/
#define QSPI_RXFLR_RFTFL(n)		(((n) & 0xff) << 0)

/* sr */
#define QSPI_SR_BUSY			(1 << 0)
#define QSPI_SR_TFNF			(1 << 1)
#define QSPI_SR_TFE			(1 << 2)
#define QSPI_SR_RFNE			(1 << 3)
#define QSPI_SR_RFF			(1 << 4)

/*imr*/
#define QSPI_IMR_TXEIM(n)		(((n) & 1) << 0)
#define QSPI_IMR_TXEIM_MASK		(1 << 0)
#define QSPI_IMR_TXOIM(n)		(((n) & 1) << 1)
#define QSPI_IMR_TXOIM_MASK		(1 << 1)
#define QSPI_IMR_RXUIM(n)		(((n) & 1) << 2)
#define QSPI_IMR_RXUIM_MASK		(1 << 2)
#define QSPI_IMR_RXOIM(n)		(((n) & 1) << 3)
#define QSPI_IMR_RXOIM_MASK		(1 << 3)
#define QSPI_IMR_RXFIM(n)		(((n) & 1) << 4)
#define QSPI_IMR_RXFIM_MASK		(1 << 4)
#define QSPI_IMR_AHBIM(n)		(((n) & 1) << 5)
#define QSPI_IMR_AHBIM_MASK		(1 << 5)

/*isr*/

#define QSPI_ISR_TXEIS			(1 << 0)
#define QSPI_ISR_TXOIS			(1 << 1)
#define QSPI_ISR_RXUIS			(1 << 2)
#define QSPI_ISR_RXOIS			(1 << 3)
#define QSPI_ISR_RXFIS			(1 << 4)
#define QSPI_ISR_AHBIS			(1 << 5)

/*risr*/
#define QSPI_RISR_TXEIS			(1 << 0)
#define QSPI_RISR_TXOIS			(1 << 1)
#define QSPI_RISR_RXUIS			(1 << 2)
#define QSPI_RISR_RXOIS			(1 << 3)
#define QSPI_RISR_RXFIS			(1 << 4)
#define QSPI_ISR_AHBIS			(1 << 5)

/*hold_wp*/
#define QSPI_HOLDWP_WP(n)		(((n) & 1) << 0)
#define QSPI_HOLDWP_WP_MASK		(1 << 0)
#define QSPI_HOLDWP_HOLD(n)		(((n) & 1) << 1)
#define QSPI_HOLDWP_HOLD_MASK		(1 << 1)

/*read_cmd*/
#define QSPI_READCMD_CMD(n)		(((n) & 0xff) << 0)
#define QSPI_READCMD_CMD_MASK		(0xff << 0)
#define QSPI_READCMD_FLASHM(n)		(((n) & 0xff) << 8)
#define QSPI_READCMD_FLASHM_MASK	(0xff << 8)
#define QSPI_READCMD_DIRECT(n)		(((n) & 0x7) << 16)
#define QSPI_READCMD_DIRECT_MASK	(0x7 << 16)
#define QSPI_READCMD_AHB_PREFETCH(n)	(((n) & 0x1) << 19)
#define QSPI_READCMD_AHB_PREFETCH_MASK	(1 << 19)

/*pgm_cmd*/
#define QSPI_PGMCMD_CMD(n)		(((n) & 0xff) << 0)
#define QSPI_PGMCMD_CMD_MASK		(0xff << 0)
#define QSPI_PGMCMD_PAGESIZE(n)		(((n) & 0x3) << 8)
#define QSPI_PGMCMD_PAGESIZE_MASK	(0x3 << 8)

/*cache_flush*/
#define QSPI_CACHEFLUSH_FLUSH		(1 << 0)

/*cache_dis_update*/
#define QSPI_CACHE_UPDATE_DISABLE(n)	(((n) & 1) << 0)
#define QSPI_CACHE_UDPATE_DISABLE_MASK	(1 << 0)

/*txfifo_flush*/
#define QSPI_TXFIFO_FLUSH		(1 << 0)

/*rxfifo_flush*/
#define QSPI_RXFIFO_FULSH		(1 << 0)

#endif

