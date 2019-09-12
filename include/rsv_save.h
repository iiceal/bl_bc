#ifndef RSV_SAVE_H__
#define	RSV_SAVE_H__
/*Need include some header file*/
#include "auth_if.h"
#include "mbr_if.h"
struct bl2fw_rsv_data {
	u32 load_addr;
	u32 exec_addr;
	u32 fl_addr;
	u32 dsize;
	u16 flags;
	u16 valid;
	int err_return;
};

struct reserved_area_data {
	struct	bl2fw_rsv_data bl;
	struct	bl2fw_rsv_data bbfw;
	struct	bl2fw_rsv_data apfw;

	bool efuse_valid;
	bool authcode_valid;
	bool bl_se;
	bool bbfw_se;
	bool apfw_se;
	bool apfw_key_en;
    u8   boot_type;
	u8   reserved_d8;
#if 0   
    u32 init0_time_count;
    u32 init1_time_count;
    u32 auth_time_count;
    u32 bl_time_count;
    u32 bbfw_time_count;
    u32 apfw_time_count;
    u32 done_time_count;
#endif
	
	u8  auth_code[AUTH_CODE_DBUF_SIZE];
	u8  mbr_data[MBR_SIZE];
};

void print_mbr_info(struct bl2fw_rsv_data *bbfw_main, struct bl2fw_rsv_data *bbfw_bak);

#endif
