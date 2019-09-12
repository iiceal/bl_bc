#ifndef BL2FW_IF_H__
#define	BL2FW_IF_H__
#define	FLASH_ADDR_LIMITED_SIZE			(16*1024*1024)

bool is_bl2fw_hdr_valid(u32 op_type);

bool get_bl2fw_hdr_aes_enkey(u8 * key, int len);

u32 get_bl2fw_hdr_version(void);

bool is_bl2fw_hdr_apkey_enabled(void);

u32 get_bl2fw_hdr_fls_addr(void);

u32 get_bl2fw_hdr_fls_size(void);

u32 get_bl2fw_hdr_load_addr(void);

u32 get_bl2fw_hdr_exec_addr(void);

u8 * get_bl2fw_hdr_dbuf(void);

u16 get_bl2fw_hdr_flags(void);

void print_bl2fw_hdr_info(void);

bool is_bl2fw_data_valid(u32 chipid, u8 * input_data, int dsize, bool check_chipid );

bool is_bl2fw_hdr_linux_flags(void);
    
#define	BL2FW_TYPE_BL		(1)
#define	BL2FW_TYPE_BB_FW	(2)
#define	BL2FW_TYPE_AP_FW 	(3)

/*error code*/
#define     BL2FW_SUCCESS                       (0)
#define     BL2FW_ERR_MBR_NOT_FOUND             (-1)
#define     BL2FW_ERR_MBR_INVALID               (-2)
#define     BL2FW_ERR_HDR_INVALID               (-3)
#define     BL2FW_ERR_RSA_KEY_INDEX_INVALID     (-4)
#define     BL2FW_ERR_RSA_FAILED                (-5)
#define     BL2FW_ERR_RSA_KEY_VALUE_INVALID     (-6)
#define     BL2FW_ERR_GET_RSA_KEY_FAILED        (-7)
#define     BL2FW_ERR_AES_INIT_FAILED           (-8)
#define     BL2FW_ERR_DATA_INVALID              (-9)
#endif
