#ifndef __DRV_SIM_H_
#define __DRV_SIM_H_

enum SIM_RET_VALUES {
    SIM_RET_OK = 0,
    SIM_RET_FAIL = -1,
    SIM_RET_TIMEOUT = -2,
};

extern int sim_read(u32 index, void *rbuf, u32 *rlen, u32 timeout_ms);
extern int sim_write(u32 index, void *wbuf, u32 *wlen, u32 timeout_ms);
extern int sim_atr(u32 index, void *rbuf, u32 *rlen, u32 etu, u32 parity, u32 timeout_ms);
extern int sim_set_clk(u32 index, u32 etu);

#endif /* __DRV_SIM_H_ */
