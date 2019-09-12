#ifndef _CMD_MBR_H_  
#define _CMD_MBR_H_

typedef enum{
    
    MBR_ITEM_MBR = 0,
    MBR_ITEM_AUTH,
    MBR_ITEM_BL,
    MBR_ITEM_BL_BK,
    MBR_ITEM_BB,
    MBR_ITEM_BB_BK,
    MBR_ITEM_AP,
    MBR_ITEM_AP_BK,

    MBR_ITEM_ALL,
    MBR_ITEM_UNUSED,
}MBR_ITEM_E;

typedef enum{

    MBR_CMD_UNUSED = 0,
    MBR_CMD_DUMP,
    MBR_CMD_MODIFY,
    MBR_CMD_ADD,
    MRB_CMD_DELETE,

}MBR_CMD_E;

typedef enum{

    MBR_CHECK_ALL = 0,
    NO_CHECK_MBR_1,
    NO_CHECK_MBR_2,
    NO_CHECK_MBR_3,
    NO_CHECK_MBR_4,
    NO_CHECK_MBR_5,
    NO_CHECK_MBR_6,
    NO_CHECK_MBR_7,
    NO_CHECK_MBR_8,
    NO_CHECK_MBR_9,

}MBR_NOCHECK_E;

typedef enum{

    MBR_REGION_UNUSED = 0,
    MBR_REGION_NAME,
    MBR_REGION_ADDR,
    MBR_REGION_SIZE,
    MBR_REGION_FLAG,

}MBR_REGION_E;

MBR_ITEM_E mbr_get_item_type(const char *s);
int do_mbr_dump_cmd(u8 *mbr_dbuf,MBR_ITEM_E item);


bool mbr_flash_cfg_read(u8 *sbuf,u32 len);
bool is_mbr_addr_valid(struct fls_fmt_data *mbr_info,MBR_NOCHECK_E no_check);
bool is_mbr_name_valid(struct fls_fmt_data *mbr_info,MBR_NOCHECK_E no_check);
bool is_mbr_flag_valid(struct fls_fmt_data *mbr_info);
bool is_mbr_crc_valid(struct fls_fmt_data *mbr_info);
bool is_mbr_valid(struct fls_fmt_data* mbr_info, MBR_NOCHECK_E no_check);
int _mbr_flash_update(u8 *mbr_data);
int mbr_modify(u32 index,struct fls_fmt_data *mbr);
int _mbr_add(struct fls_fmt_data *mbr);
int _mbr_delete(u32 index);
u32 mbr_entry_init(void);
u32 mbr_entry_dump(void);

#endif
