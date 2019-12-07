#include <common.h>
#include <pll.h>
#include <regs/scm.h>

#ifdef CONFIG_ENABLE_TRACE
#define pll_debug(fmt, args...)  printf(fmt"\n", ##args);
#else
#define pll_debug(fmt, args...)
#endif

unsigned int pow(unsigned int base, unsigned int exp)
{
    unsigned int r=1, i=0;

    if(!exp)
    {
        return 1;
    }else{
        for(i=1; i<=exp; i++)
        {
            r = base*r;
        }
    }

    return r;
}

static unsigned int get_strip_refclk_rate_kHz(void)
{
    u32 stap = hwp_apSCM->strappingstatus;
    pll_debug("strappingstatus regv: 0x%x", stap);
    switch(stap & SCM_STRAP_REFCLK_MASK) {
    case SCM_STRAP_REFCLK_10M:
        return 10000;
    case SCM_STRAP_REFCLK_20M:
        return 20000;
    case SCM_STRAP_REFCLK_25M:
        return 25000;
    default:
        return RET_FAILED;
    }
    return RET_FAILED;
}

int get_M_N_NO(unsigned int CLK_IN, unsigned int ratekhz,  unsigned int *m, unsigned int *n, unsigned int *no)
{
    unsigned int NO;    //pll23_ctrl [13:12]
    unsigned int N=0;     //pll23_ctrl [3:0]
    unsigned int M=0;     //pll23_ctrl [11:4]
    unsigned int n_o[4] = {1,2,4,8};
    unsigned int n_o_ok[4] = {0};
    int j=0, i=0;

    if((ratekhz < PLL_OUT_MIN) || (ratekhz > PLL_RANGE_MAX))
    {
        printf("ERROR: the pll output range: 62.5MHz ~~ 1500MHz, now is %d kHz\n", ratekhz);
        return RET_FAILED;
    }

    for(i=0; i<4; i++)
    {
        if(((ratekhz*n_o[i]) >= PLL_RANGE_MIN) && ((ratekhz*n_o[i]) <= PLL_RANGE_MAX))
        {
            n_o_ok[j]=i;
            j++;
        }
    }

    if(j == 0)
    {
        printf("%s: get NO ERROR!\n", __func__);
        return RET_FAILED;
    }
    pll_debug("NO %d %d %d %d\n", n_o_ok[0], n_o_ok[1], n_o_ok[2], n_o_ok[3]);

    for(i=0; i<j; i++)
    {
        NO = n_o_ok[i];

        for(N=1; N<=0xf; N++)
        {
            M = (ratekhz * N * pow(2, NO))/CLK_IN;
            if((M<=0xff) && (M>1) &&
                    ((CLK_IN/N) >=1000) && ((CLK_IN/N) <=50000))
            {
                *m = M;
                *n = N;
                *no = NO;
                pll_debug("%s : M %d N %d NO %d \n", __func__, M, N, NO);
                return RET_OK;
            }
        }
    }

    printf("%s: get M N NO ERROR!\n", __func__); 
    return RET_FAILED;
}

/* 
 * ratekhz is the PLL0 output which we want to set
 * the valid range of ratekhz: 500MHz =< ratekhz =<1500MHz
 *
 * CLK_OUT = CLK_IN * (M/N) * (1/NO) 
 */
int pll0_set_rate_kHz(unsigned int ratekhz)
{
    unsigned int NO=2;    //pll01_ctrl [13:12]
    unsigned int N=0xf;     //pll01_ctrl [3:0]
    unsigned int M=0;     //pll01_ctrl [11:4]
    unsigned int regv = 0;
    unsigned int CLK_IN = 0;
    int ret = 0;

    CLK_IN = get_strip_refclk_rate_kHz();
    if(RET_FAILED == CLK_IN)
    {
        printf("ERROR: refclk is ERROR\n");
        return RET_FAILED;
    }

    ret = get_M_N_NO(CLK_IN, ratekhz,  &M, &N, &NO);
    if(RET_FAILED == ret)
    {
        printf("ERROR: get M N NO is ERROR\n");
        return RET_FAILED;
    }

    pll_debug("ref = %d M=%d N=%d \n", get_strip_refclk_rate_kHz(), M, N);

    regv =( (M<<4) | N | (NO <<12));

    pll_debug("regv =0x%x \n", regv);

    //switch cpu0 clk to reference clk
    N = hwp_apSCM->clk_ctrl;
    hwp_apSCM->clk_ctrl = (N|0x3);

    //set pdrst of ppl0 to 1, we can set PLL divider only after set pdrst 
    M = hwp_apSCM->pll01_ctrl;
    N = ((M&0xffffbfff)|(0x4000));
    //write pdrst
    hwp_apSCM->pll01_ctrl = N;

    //write pll divider
    M = hwp_apSCM->pll01_ctrl;
    //clear divider
    N = (M&(0xffffC000));
    regv |= N;
    hwp_apSCM->pll01_ctrl = regv;

    pll_debug("regv = 0x%x", regv);

    //set pdrst to 0
    M = hwp_apSCM->pll01_ctrl;
    N = (M&(0xffffbfff));
    hwp_apSCM->pll01_ctrl = N;

    //delay time
    udelay(700);

    //switch reference clk to cpu0 clk
    N = hwp_apSCM->clk_ctrl;
    hwp_apSCM->clk_ctrl = ((N &(~0x3)) | 0x1);

    return RET_OK;
}

int pll0_get_rate_kHz(void)
{
    unsigned int NO=0;    //pll01_ctrl [13:12]
    unsigned int N=0;     //pll01_ctrl [3:0]
    unsigned int M=0;      //pll01_ctrl [11:4]
    unsigned int regv=0;

    regv = hwp_apSCM->pll01_ctrl;
    N = regv&0xf;
    M = (regv>>4)&0xff;
    NO = (regv>>12)&0x3;

    NO = pow(2, NO);

    regv = (get_strip_refclk_rate_kHz()* M)/(N * NO);

    return regv;
}

/* 
 * ratekhz is the PLL1 output which we want to set
 * the valid range of ratekhz: 500MHz =< ratekhz =<1500MHz
 *
 * CLK_OUT = CLK_IN * (M/N) * (1/NO) 
 */
int pll1_set_rate_kHz(unsigned int ratekhz)
{
    unsigned int NO=2;    //pll01_ctrl [29:28]
    unsigned int N=0xf;     //pll01_ctrl [19:16]
    unsigned int M=0;     //pll01_ctrl [27:20]
    unsigned int regv = 0;
    unsigned int CLK_IN = 0;
    int ret;

    CLK_IN = get_strip_refclk_rate_kHz();
    if(RET_FAILED == CLK_IN)
    {
        printf("ERROR: refclk is ERROR\n");
        return RET_FAILED;
    }
    
    pll_debug("CLK_IN %d CLK_OUT = %d \n", CLK_IN, ratekhz);

    ret = get_M_N_NO(CLK_IN, ratekhz,  &M, &N, &NO);
    if(RET_FAILED == ret)
    {
        printf("ERROR: get M N NO is ERROR\n");
        return RET_FAILED;
    }

    pll_debug("ref = %d M=%d N=%d  NO = %d \n", get_strip_refclk_rate_kHz(), M, N, NO);

    regv =(( (NO << 12) | (M<<4) | N) << 16);

    pll_debug("regv = 0x%x\n", regv);

    //set pdrst of ppl0 to 1, we can set PLL divider only after set pdrst 
    M = hwp_apSCM->pll01_ctrl;
    N = ((M&0xbfffffff)|(0x40000000));
    //write pdrst
    hwp_apSCM->pll01_ctrl = N;

    //write pll divider
    M = hwp_apSCM->pll01_ctrl;
    //clear divider
    N = (M&(0xC000ffff));
    regv |= N;
    hwp_apSCM->pll01_ctrl = regv;
    pll_debug("regv = 0x%x", regv);

    //set pdrst to 0
    M = hwp_apSCM->pll01_ctrl;
    N = (M&(0xbfffffff));
    hwp_apSCM->pll01_ctrl = N;

    //delay time
    udelay(700);

    return RET_OK;
}

int pll1_get_rate_kHz(void)
{
    unsigned int NO=0;    //pll01_ctrl [29:28]
    unsigned int N=0;     //pll01_ctrl [19:16]
    unsigned int M=0;     //pll01_ctrl [27:20]
    unsigned int regv=0;

    regv = ((hwp_apSCM->pll01_ctrl)>>16);
    N = regv&0xf;
    M = (regv>>4)&0xff;
    NO = (regv>>12)&0x3;

    NO = pow(2, NO);

    regv = (get_strip_refclk_rate_kHz()* M)/(N * NO);

    return regv;
}


/* 
 * ratekhz is the PLL2 output which we want to set
 * the valid range of ratekhz: 500MHz =< ratekhz =<1500MHz
 *
 * CLK_OUT = CLK_IN * (M/N) * (1/NO) 
 */
int pll2_set_rate_kHz(unsigned int ratekhz)
{
    unsigned int NO=3;    //pll23_ctrl [13:12]
    unsigned int N=0xf;     //pll23_ctrl [3:0]
    unsigned int M=0;     //pll23_ctrl [11:4]
    unsigned int regv = 0;
    unsigned int CLK_IN = 0;
    int ret;

    CLK_IN = get_strip_refclk_rate_kHz();
    if(RET_FAILED == CLK_IN)
    {
        printf("ERROR: refclk is ERROR\n");
        return RET_FAILED;
    }

    pll_debug("CLK_IN %d CLK_OUT = %d \n", CLK_IN, ratekhz);

    ret = get_M_N_NO(CLK_IN, ratekhz,  &M, &N, &NO);
    if(RET_FAILED == ret)
    {
        printf("ERROR: get M N NO is ERROR\n");
        return RET_FAILED;
    }

    regv =( (NO<<12) | (M<<4) | N);

    //set pdrst of ppl0 to 1, we can set PLL divider only after set pdrst 
    M = hwp_apSCM->pll23_ctrl;
    N = ((M&0xffffbfff)|(0x4000));
    //write pdrst
    hwp_apSCM->pll23_ctrl = N;

    //write pll divider
    M = hwp_apSCM->pll23_ctrl;
    //clear divider
    N = (M&(0xffffC000));
    regv |= N;
    hwp_apSCM->pll23_ctrl = regv;
    pll_debug("regv = 0x%x", regv);

    //set pdrst to 0
    M = hwp_apSCM->pll23_ctrl;
    N = (M&(0xffffbfff));
    hwp_apSCM->pll23_ctrl = N;

    //delay time
    udelay(700);

    return RET_OK;
}

int pll2_get_rate_kHz(void)
{
    unsigned int NO=0;    //pll23_ctrl [13:12]
    unsigned int N=0;     //pll23_ctrl [3:0]
    unsigned int M=0;      //pll23_ctrl [11:4]
    unsigned int regv=0;

    regv = hwp_apSCM->pll23_ctrl;
    N = regv&0xf;
    M = (regv>>4)&0xff;
    NO = (regv>>12)&0x3;

    NO = pow(2, NO);

    regv = (get_strip_refclk_rate_kHz()* M)/(N * NO);

    return regv;
}

//pll3 refclk is 80MHz
//#define pll3_ref_clk_khz    (80000)
//pll3 default reference clk
int pll3_ref_clk_khz = 64000;

/* 
 * ratekhz is the pll3 output which we want to set
 * the valid range of ratekhz: 500MHz =< ratekhz =<1500MHz
 *
 * CLK_OUT = CLK_IN* (M/N) * (1/NO) 
 */
int pll3_set_rate_kHz(unsigned int ratekhz)
{
    unsigned int NO=2;    //pll23_ctrl [29:28]
    unsigned int N=0xf;     //pll23_ctrl [19:16]
    unsigned int M=0;     //pll23_ctrl [27:20]
    unsigned int regv = 0;
    int ret;

    printf("%s: PLL3 reference clk is %d kHz\n", __func__, pll3_ref_clk_khz);

    ret = get_M_N_NO(pll3_ref_clk_khz, ratekhz,  &M, &N, &NO);
    if(RET_FAILED == ret)
    {
        printf("ERROR: get M N NO is ERROR\n");
        return RET_FAILED;
    }

    regv =(((NO<<12) | (M<<4) | N) << 16);

    //set pdrst of ppl0 to 1, we can set PLL divider only after set pdrst 
    M = hwp_apSCM->pll23_ctrl;
    N = ((M&0xbfffffff)|(0x40000000));
    //write pdrst
    hwp_apSCM->pll23_ctrl = N;

    //write pll divider
    M = hwp_apSCM->pll23_ctrl;
    //clear divider
    N = (M&(0xC000ffff));
    regv |= N;
    hwp_apSCM->pll23_ctrl = regv;
    pll_debug("regv = 0x%x", regv);

    //set pdrst to 0
    M = hwp_apSCM->pll23_ctrl;
    N = (M&(0xbfffffff));
    hwp_apSCM->pll23_ctrl = N;

    //delay time
    udelay(700);

    return RET_OK;
}

int pll3_get_rate_kHz(void)
{
    unsigned int NO=0;    //pll23_ctrl [29:28]
    unsigned int N=0;     //pll23_ctrl [19:16]
    unsigned int M=0;     //pll23_ctrl [27:20]
    unsigned int regv=0;

    printf("%s: PLL3 reference clk is %d kHz\n", __func__, pll3_ref_clk_khz);

    regv = ((hwp_apSCM->pll23_ctrl)>>16);
    N = regv&0xf;
    M = (regv>>4)&0xff;
    NO = (regv>>12)&0x3;

    NO = pow(2, NO);

    regv = (pll3_ref_clk_khz * M)/(N * NO);

    return regv;
}
