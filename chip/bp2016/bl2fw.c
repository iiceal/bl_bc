#include <common.h>
#include "hg_bl2fw.h"
#include "bl2fw_if.h"

#ifdef BL2FW_DEBUG
#define bl2fw_debug(fmt, args...)  printf("BL2FW: " fmt "\n", ##args);
#else
#define bl2fw_debug(fmt, args...)
#endif

static u8 bl2fw_hdr_dbuf[BL2FW_HDR_SIZE]__attribute__((__aligned__(64)));
static struct bl2fw_hdr * bl2fw;

static bool bl2fw_hdr_crc_valid(void)
{
	u16 crc16_v;
	crc16_v = crc16(bl2fw_hdr_dbuf, sizeof(struct bl2fw_hdr) - sizeof(u16));		
	if(crc16_v == bl2fw->crc16)
		return true;
	return false;
}

static bool b2fw_hdr_magic_valid(void)
{
	u32 bl2fw_type;
	bl2fw_type = bl2fw->flags & BL2FW_FLAGS_TYPE_INDEX_MASK;

	if((bl2fw_type == BL2FW_FLAGS_BL) && (bl2fw->magic == BL_MAGIC)) 
		return true;

	if(((bl2fw_type == BL2FW_FLAGS_BB_FW) || (bl2fw_type == BL2FW_FLAGS_AP_FW)) 
		&& (bl2fw->magic == FW_MAGIC)) 
		return true;

	return false;	
}

static bool b2fw_hdr_flags_type_valid(u32 op_type)
{
	u32 bl2fw_type;
	bl2fw_type = bl2fw->flags & BL2FW_FLAGS_TYPE_INDEX_MASK;

    switch(bl2fw_type) {
        case BL2FW_FLAGS_BL:
            if(op_type != BL2FW_TYPE_BL)
                return false;	
            break;
        case BL2FW_FLAGS_BB_FW:
            if(op_type != BL2FW_TYPE_BB_FW)
                return false;
            break;
        case BL2FW_FLAGS_AP_FW:
            if(op_type != BL2FW_TYPE_AP_FW)
                return false;	
            break;
        default:
            return false;
    }
	return true;
}

static bool b2fw_hdr_addr_valid(void) 
{
	if(bl2fw->flash_addr >= FLASH_ADDR_LIMITED_SIZE) 
		return false;

	if((bl2fw->flash_addr + bl2fw->size) >= FLASH_ADDR_LIMITED_SIZE) 
		return false;
#if 0
	 
#endif
	return true;
}
/*
 *  dsize = plain_data_size + padding len +suf_data_size
 *  dsize % 256 = 0
 */
bool is_bl2fw_data_valid(u32 chipid, u8 * input_data, int dsize, bool check_chipid)
{
	int data_suf_len = sizeof(struct bl2fw_plain_data_suf);
	struct bl2fw_plain_data_suf  * dsuf;
	u32 crc32_v = 0;

	if(input_data == NULL)
		return false;

	if((dsize <= data_suf_len) || ((dsize % 256) != 0)) { 
		bl2fw_debug("data length is invalid, %d\n", dsize);
		return false;
	}

	crc32_v = crc32(0, input_data, dsize - 4);
	dsuf = (struct bl2fw_plain_data_suf *)(input_data + dsize - data_suf_len);

	if(dsuf->crc32 != crc32_v) {
		bl2fw_debug("crc is invalid, 0x%x\n", crc32_v);
		return false;
	}

	if((check_chipid == true) && (dsuf->chipid != chipid)) {
		bl2fw_debug("chipid is invalid\n");
		return false;
	}
	return true;
}

bool is_bl2fw_hdr_valid(u32 op_type)
{
	if(b2fw_hdr_magic_valid() == false) {
		bl2fw_debug("magic error: 0x%x\n", bl2fw->magic);
		return false;
	}
	
	if(b2fw_hdr_flags_type_valid(op_type) == false) {
		bl2fw_debug("flags type error: 0x%x\n", bl2fw->flags & BL2FW_FLAGS_TYPE_INDEX_MASK);
		return false;
	}

	if(bl2fw_hdr_crc_valid() == false) {
		bl2fw_debug("crc error\n");
		return false;
	}

	if(b2fw_hdr_addr_valid() == false) {
		bl2fw_debug("fls_addr = 0x%x\n", bl2fw->flash_addr); 
		bl2fw_debug("size     = 0x%x\n", bl2fw->size); 
		bl2fw_debug("load_addr= 0x%x\n", bl2fw->load_addr); 
		return false;
	}

	if((bl2fw->size % 256) != 0) {
		bl2fw_debug("size must be aligned 256B, size = %d\n", bl2fw->size);
		return false;
	}

	return true;
}

bool get_bl2fw_hdr_aes_enkey(u8 * key, int len)
{
	if(len < 256)
	     return false;

	memcpy(key, &bl2fw->data_en_key[0], 256);
	return true;
}

u32 get_bl2fw_hdr_version(void)
{
	return bl2fw->version;
}

bool is_bl2fw_hdr_apkey_enabled(void)
{
	return (bl2fw->flags & BL2FW_FLAGS_AP_DES_EN) > 0 ? true:false;
}

bool is_bl2fw_hdr_linux_flags(void)
{
	return (bl2fw->flags & BL2FW_FLAGS_AP_FW_LINUX) > 0 ? true:false;
}


u32 get_bl2fw_hdr_fls_addr(void)
{
	return bl2fw->flash_addr;
}

u32 get_bl2fw_hdr_fls_size(void)
{
	return bl2fw->size;
}

u32 get_bl2fw_hdr_load_addr(void)
{
	return bl2fw->load_addr;
}

u32 get_bl2fw_hdr_exec_addr(void)
{
	return bl2fw->exec_addr;
}

u16 get_bl2fw_hdr_flags(void)
{
	return bl2fw->flags;
}


u8 * get_bl2fw_hdr_dbuf(void)
{
	bl2fw = (struct bl2fw_hdr *)bl2fw_hdr_dbuf;
	return bl2fw_hdr_dbuf;
}

void print_bl2fw_hdr_info(void)
{
	u32 bl2fw_type = bl2fw->flags & BL2FW_FLAGS_TYPE_INDEX_MASK;
	switch(bl2fw_type) {
	case BL2FW_FLAGS_BL:
		bl2fw_debug("BL_HDR\n");
		break;
	case BL2FW_FLAGS_BB_FW:
		bl2fw_debug("BB_FW_HDR\n");
		break;
	case BL2FW_FLAGS_AP_FW:
		bl2fw_debug("AP_FW_HDR\n");
		break;
	default:
		return;
	}

	bl2fw_debug("version        : %d.%dv\n", 
		(bl2fw->version >> 16) & 0xffff, bl2fw->version & 0xffff);
	
	bl2fw_debug("cpu flash addr : 0x%x\n", bl2fw->flash_addr);
	bl2fw_debug("cpu size       : 0x%x (%dK)\n", bl2fw->size, bl2fw->size/1024);
	bl2fw_debug("cpu load addr  : 0x%x\n", bl2fw->load_addr);
	bl2fw_debug("cpu exec addr  : 0x%x\n", bl2fw->exec_addr);
	bl2fw_debug("flags          : 0x%x\n", bl2fw->flags);
	if(BL2FW_FLAGS_AP_FW == bl2fw_type) { 
		bl2fw_debug("apkey enabled  : %s\n", 
			is_bl2fw_hdr_apkey_enabled() == true ? "use efuse key":"use hdr key");
		if(bl2fw->flags & BL2FW_FLAGS_AP_FW_LINUX)	
            bl2fw_debug("This firmware is linux kernel(dtb + linux kernel)\n");
		if(bl2fw->flags & BL2FW_FLAGS_SE_DIS)
            bl2fw_debug("This firmware is non-security verify\n");
    }
}




