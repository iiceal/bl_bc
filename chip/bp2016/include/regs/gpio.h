#ifndef _REGS_GPIO_H_
#define _REGS_GPIO_H_

#define REG_AP_GPIO_BASE         (APB2_BASE+0x30000)

typedef volatile struct
{
    REG32                       porta_dr;              	//0x00000000
    REG32                       porta_ddr;             	//0x00000004
    REG32                       porta_ctl;             	//0x00000008

    REG32                       portb_dr;              	//0x0000000c
    REG32                       portb_ddr;             	//0x00000010
    REG32                       portb_ctl;             	//0x00000014

    REG32                       portc_dr;              	//0x00000018
    REG32                       portc_ddr;             	//0x0000001c
    REG32                       portc_ctl;             	//0x00000020

    REG32                       portd_dr;              	//0x00000024
    REG32                       portd_ddr;             	//0x00000028
    REG32                       portd_ctl;             	//0x0000002c

    REG32			inten;			//0x00000030
    REG32			intmask;		//0x00000034
    REG32			inttype_level;		//0x00000038
    REG32			int_polarity;		//0x0000003c
    REG32			intstatus;		//0x00000040
    REG32			raw_intstatus;		//0x00000044
    REG32			debounce;		//0x00000048
    REG32			porta_eoi;		//0x0000004c

    REG32			ext_porta;		//0x00000050
    REG32			ext_portb;		//0x00000054
    REG32			ext_portc;		//0x00000058
    REG32			ext_portd;		//0x0000005c
    
    REG32			ls_sync;		//0x00000060
    REG32			id_code;		//0x00000064
    REG32			pad_resv_0x68;		//0x00000068
    REG32			ver_id_code;		//0x0000006c
    REG32			config_reg1;		//0x00000070
    REG32			config_reg2;            //0x00000074

} HWP_GPIO_AP_T;

#define hwp_apGpio               ((HWP_GPIO_AP_T*) (REG_AP_GPIO_BASE))

//porta_dr/portb_dr/portc_dr/portd_dr: data_register for gpio set

//porta_ddr/portb_ddr/portc_ddr/portd_ddr: data direction register
#define	GPIO_DDR_INPUT		  (0)
#define	GPIO_DDR_OUTPUT		  (1)

//porta_ctl/portb_ctl/portc_ctl/portd_ctl: data source register
#define	GPIO_CTL_SOFT		  (0)
#define	GPIO_CTL_HW		  (1)

//inten
#define	GPIO_PORTA_INTEN	  (1)
#define	GPIO_PORTA_NORMAL	  (0)

//intmask
#define	GPIO_INT_UNMASK	  	  (0)
#define	GPIO_INT_MASK	  	  (1)

//inttype_level
#define	GPIO_INTTYPE_LEVEL	  (0)
#define	GPIO_INTTYPE_EDGE	  (1)

//int_polarity
#define	GPIO_INT_POLARITY_LOW	  (0)
#define	GPIO_INT_POLARITY_HIGH	  (1)

//debounce
#define	GPIO_DEBOUNCE_EN	  (1<<0)

//porta_eoi
#define	GPIO_PORTA_INT_CLR	  (1)

//ls_sync
#define	GPIO_LS_SYNC_EN		  (1<<0)



#endif
