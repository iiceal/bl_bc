#ifndef _REGS_SIM_H_
#define _REGS_SIM_H_

#define REG_SIM0_BASE (APB2_BASE + 0x00000)
#define REG_SIM1_BASE (APB2_BASE + 0x10000)
#define REG_SIM2_BASE (APB2_BASE + 0x20000)

typedef volatile struct {
    REG32 port;         /* 0x00000000*/
    REG32 cntl;         /* 0x00000004*/
    REG32 _pad_0x08;    /* 0x00000008*/
    REG32 enable;       /* 0x0000000c*/
    REG32 int_stat;     /* 0x00000010*/
    REG32 int_raw_stat; /* 0x00000014*/
    REG32 int_enable;   /* 0x00000018*/
    REG32 xmt_buf;      /* 0x0000001c*/
    REG32 rcv_buf;      /* 0x00000020*/
    REG32 xmt_th;       /* 0x00000024*/
    REG32 guard;        /* 0x00000028*/
    REG32 sres;         /* 0x0000002c*/
    REG32 ch_wait;      /* 0x00000030*/
    REG32 gpcnt;        /* 0x00000034*/
    REG32 divisor;      /* 0x00000038*/
    REG32 _pad_0x3c;    /* 0x0000003c*/
    REG32 lowimen;      /* 0x00000040*/
    REG32 hiclknum;     /* 0x00000044*/
    REG32 hicnt0_set;   /* 0x00000048*/
    REG32 hicnt1_set;   /* 0x0000004c*/
    REG32 hicnt2_set;   /* 0x00000050*/
} HWP_SIM_T;

#define hwp_sim0 ((HWP_SIM_T *) (REG_SIM0_BASE))
#define hwp_sim1 ((HWP_SIM_T *) (REG_SIM1_BASE))
#define hwp_sim2 ((HWP_SIM_T *) (REG_SIM2_BASE))

typedef enum {
    SIM_CLK_BAUD_E_372 = 0,
    SIM_CLK_BAUD_E_256,
    SIM_CLK_BAUD_E_128,
    SIM_CLK_BAUD_E_64,
    SIM_CLK_BAUD_E_32,
    SIM_CLK_BAUD_E_32_1,
    SIM_CLK_BAUD_E_16,
    SIM_CLK_BAUD_E_DIVISOR
} SIM_CLK_BAUD_E;


#define DEFAULT_GP_CNT              50000UL /*for ATR waiting time*/
#define SIM_RTX_FIFO_SIZE           64

// SIM_PORT(0X00)
#define SIM_PORT_SAPD               (1 << 0) /*AUTO POWER OFF*/
#define SIM_PORT_SRST               (1 << 2) /*RESET PIN OUTPUT*/
#define SIM_PORT_SCEN               (1 << 3) /*CLK OUT ENABLE*/
#define SIM_PORT_SCSP               (1 << 4) /*CLK OUT 0/1*/
#define SIM_PORT_PE                 (1 << 5) /*no parity*/

// SIM_CNTL(0X04)
#define SIM_CNTL_IC                 (1 << 0)        /*DATA FORMAT CONTROL*/
#define SIM_CNTL_ICM                (1 << 1)        /*START CHARACTER MODE CONTROL*/
#define SIM_CNTL_ANACK              (1 << 2)        /*START CHARACTER ERROR NACK CONTROL*/
#define SIM_CNTL_ONACK              (1 << 3)        /*OVERFLOW START CHARACTER ERROR NACK CONTROL*/
#define SIM_CNTL_CWTEN              (1 << 7)        /*CHARACTER WAITING COUNTER VALID*/
#define SIM_CNTL_GPEN               (1 << 8)        /*GENERAL TIMER VALID*/
#define SIM_CNTL_NACK_EN            (1 << 9)        /*NACK ENABLE*/
#define SIM_CNTL_BAUD_SEL_BIT       4               /*START BIT4~6*/
#define SIM_CNTL_BAUD_SEL_MASK              (7 << SIM_CNTL_BAUD_SEL_BIT)
#define SIM_CNTL_BAUD_SIM_CLKOUT_372        ((SIM_CLK_BAUD_E_372 << SIM_CNTL_BAUD_SEL_BIT) & (SIM_CNTL_BAUD_SEL_MASK))
#define SIM_CNTL_BAUD_SIM_CLKOUT_256        ((SIM_CLK_BAUD_E_256 << SIM_CNTL_BAUD_SEL_BIT) & (SIM_CNTL_BAUD_SEL_MASK))
#define SIM_CNTL_BAUD_SIM_CLKOUT_128        ((SIM_CLK_BAUD_E_128 << SIM_CNTL_BAUD_SEL_BIT) & (SIM_CNTL_BAUD_SEL_MASK))
#define SIM_CNTL_BAUD_SIM_CLKOUT_64         ((SIM_CLK_BAUD_E_64 << SIM_CNTL_BAUD_SEL_BIT) & (SIM_CNTL_BAUD_SEL_MASK))
#define SIM_CNTL_BAUD_SIM_CLKOUT_32         ((SIM_CLK_BAUD_E_32 << SIM_CNTL_BAUD_SEL_BIT) & (SIM_CNTL_BAUD_SEL_MASK))
#define SIM_CNTL_BAUD_SIM_CLKOUT_32_1       ((SIM_CLK_BAUD_E_32_1 << SIM_CNTL_BAUD_SEL_BIT) & (SIM_CNTL_BAUD_SEL_MASK))
#define SIM_CNTL_BAUD_SIM_CLKOUT_16         ((SIM_CLK_BAUD_E_16 << SIM_CNTL_BAUD_SEL_BIT) & (SIM_CNTL_BAUD_SEL_MASK))
#define SIM_CNTL_BAUD_SIM_CLKOUT_DIV        ((SIM_CLK_BAUD_E_DIVISOR << SIM_CNTL_BAUD_SEL_BIT) & (SIM_CNTL_BAUD_SEL_MASK))

// SIM_ENABLE(0X0C)
#define SIM_ENABLE_RCV_EN           (1 << 0)   /*RECEIVE ENABLE*/
#define SIM_ENABLE_XMT_EN           (1 << 1)   /*TRANSMIT ENABLE*/
#define SIM_ENABLE_SIM_EN           (1 << 2)   /*SIM MODULE ENABLE*/

// SIM_INT_STAT(0X10)
#define SIM_INT_STAT_RDOEI          (1 << 0)   /*RX FIFO EMPTY READ ERROR*/
#define SIM_INT_STAT_RFNEI          (1 << 1)   /*RX FIFO NON-EMPTY RECEIVE FLAG*/
#define SIM_INT_STAT_RDHFI          (1 << 2)   /*RX FIFO HALF FULL FLAG*/
#define SIM_INT_STAT_RDAFI          (1 << 3)   /*RX FIFO ALOST FULL FLAG*/
#define SIM_INT_STAT_CWTI           (1 << 4)   /*CHAR WAIT TIME OVERFLOW*/
#define SIM_INT_STAT_XTEI           (1 << 5)   /*RESEND NACK ERROR*/
#define SIM_INT_STAT_TDOI           (1 << 6)   /*TX FIFO OVERFLOW*/
#define SIM_INT_STAT_TDAEI          (1 << 7)   /*TX FIFO ALMOST EMPTY FLAG*/
#define SIM_INT_STAT_TDHFI          (1 << 8)   /*TX FIFO HALF EMPTY FLAG*/
#define SIM_INT_STAT_TDNFI          (1 << 9)   /*TX FIFO NOT FULL FLAG*/
#define SIM_INT_STAT_TCL            (1 << 10)  /*TX FINISHED FLAG*/
#define SIM_INT_STAT_GPCNTI         (1 << 11)  /*GENGRAL COUNTER*/

// SIM_INT_ENABLE(0X18)
#define SIM_INT_ENALBE_RDOEE        (1 << 0)   /*RX FIFO READ EMPTY IRQ ENABLE*/
#define SIM_INT_ENALBE_RFNEE        (1 << 1)   /*RX FIFO NOT EMPTY IRQ ENABLE*/
#define SIM_INT_ENALBE_RDHFE        (1 << 2)   /*RX FIFO HALF FULL IRQ ENABLE*/
#define SIM_INT_ENALBE_RDAFE        (1 << 3)   /*RX FIFO ALMOST FULL IRQ ENABLE*/
#define SIM_INT_ENALBE_CWTE         (1 << 4)   /*CHAR WAITINT COUNTER IRQ ENABLE*/
#define SIM_INT_ENALBE_XTEE         (1 << 5)   /*RESEND NACK THRESHOLD OVERFLOW IRQ ENABLE*/
#define SIM_INT_ENALBE_TDOE         (1 << 6)   /*TX FIFO OVERFLOW ERROR IRQ ENABLE*/
#define SIM_INT_ENALBE_TDAEE        (1 << 7)   /*TX FIFO ALMOST EMPTY IRQ ENABLE*/
#define SIM_INT_ENALBE_TDHFE        (1 << 8)   /*TX FIFO HALF EMPTY IRQ ENABLE*/
#define SIM_INT_ENALBE_TDNFE        (1 << 9)   /*TX FIFO NOT FULL IRQ ENABLE*/
#define SIM_INT_ENALBE_TCE          (1 << 10)  /*TX FINISHED IRQ ENABLE*/
#define SIM_INT_ENALBE_GPCNTE       (1 << 11)  /*GENERAL COUNTER IRQ ENABLE*/

// SIM_XMT_FIFO(0X1c)
#define SIM_XMT_FIFO_DEPTH          64
#define SIM_XMT_FIFO_HALF_DEPTH     (SIM_XMT_FIFO_DEPTH >> 1)

// SIM_RCV_BUF(0X20)
#define SIM_RCV_BUF_PE              (1 << 8)   /*FRAME ERROR FLAG*/
#define SIM_RCV_BUF_FE              (1 << 9)   /*PARITY CHECK ERROR FLAG*/

// SIM_GUARD(0X28)
#define SIM_GUARD_RCVR11            (1 << 8)   /* 11 ETU*/
#define SIM_GUARD_PROT_2ETU         2          /* DEFAULT 2 PROTECT ETU*/

// SIM_SRES(0X2C)
#define SIM_SRES_FLUSH_RCV          (1 << 0)   /*RESET RX FIFO AND RX STATE MACHINE*/
#define SIM_SRES_FLUSH_XMT          (1 << 1)   /*RESET TX FIFO AND TX STATE MACHINE*/
#define SIM_SRES_SOFT_RES           (1 << 2)   /*SOFTWARE RESET SIM MODULE*/

#endif /* _REGS_SIM_H_ */
