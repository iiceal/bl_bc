#ifndef MBR_IF_H__
#define	MBR_IF_H__

#define MBR_SIZE 4096

/*name*/
#define		MBR_NAME   		"mbr(self)"
#define		AUTHCODE_NAME		"auth.code"
#define		BL_NAME			"ramloader"
#define		BL_BACKUP_NAME		"ramloader-bk"
#define		BB_FW_NAME		"bb.firmware"
#define		BB_FW_BACKUP_NAME	"bb.firmware-bk"
#define		AP_FW_NAME		"ap.firmware"
#define		AP_FW_BACKUP_NAME	"ap.firmware-bk"

/*flags*/
#define		FLS_DATA_FLAGS_VALID	(1 << 0)	



u8 * get_flash_mbr_dbuf(void);
bool is_mbr_cfg_valid(void);
bool get_flash_mbr(const char * name, u32 * fl_addr, u32 * fl_size);

//void print_mbr_info(void);
#endif
