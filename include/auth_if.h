#ifndef AUTH_IF_H__
#define	AUTH_IF_H__

#define	AUTH_CODE_DBUF_SIZE	256
//#define ENABLE_AUTH_LOG		1

bool is_auth_data_valid(u32 chipid);
bool is_auth_bbjtag_disabled(void);
bool is_auth_apjtag_disabled(void);
bool is_auth_bl_verf_disabled(void);
bool is_auth_fw_verf_disabled(void);
bool is_auth_ap_key_enabled(void);
bool is_auth_trace_disabled(void);
int  get_auth_key_index(void);
u32  get_auth_version(void);
u8 * get_auth_dbuf(void);
void print_authcode_info(void);

#endif
