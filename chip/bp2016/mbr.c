#include <common.h>
#include "hg_mbr.h"
#include "mbr_if.h"
#include "bl2fw_if.h"
#include "rsv_save.h"

static u8 mbr_dbuf[MBR_SIZE]__attribute__((__aligned__(64)));  
struct reserved_area_data * reserved_save;
struct bl2fw_rsv_data bbfw_main, bbfw_bak;

u8 * get_flash_mbr_dbuf(void)
{
	return mbr_dbuf;
}

/*fl_addr is only offset address*/
static bool mbr_addr_valid(struct fls_fmt_data * fls) 
{
	if(fls->fl_addr >= FLASH_ADDR_LIMITED_SIZE) 
		return false;

	if((fls->fl_addr + fls->fl_size) >= FLASH_ADDR_LIMITED_SIZE) 
		return false;

	return true;
}

bool is_mbr_cfg_valid(void)
{
	return get_flash_mbr(MBR_NAME, NULL, NULL);	
}

bool get_flash_mbr(const char * name, u32 * fl_addr, u32 * fl_size)
{
	struct fls_fmt_data * fls; 
	int i, num_0xff, length;
	u16 crc16_v;
		
	if(name == NULL)
		return false;

	if(fl_addr != NULL)
		*fl_addr = 0;
	if(fl_size != NULL)
		*fl_size = 0;

	fls = (struct fls_fmt_data *)mbr_dbuf;
	length = 0;

	while(1) {
		num_0xff = 0;
		crc16_v = 0;
		i = 0;

		for(i = 0; i < 16; i++) {
			if((u8)fls->name[i] == 0xff)
				num_0xff ++;
		}

		if(num_0xff == 16)
			break;

		if(strcmp(fls->name, name) == 0) {

			if(mbr_addr_valid(fls) ==false)
				return false;
		
			crc16_v = crc16((void*)fls, sizeof(struct fls_fmt_data) - 2);

			if((fls->flags & FLS_FLAGS_VALID) && (crc16_v == fls->crc16)) {
				if(fl_addr != NULL)
					*fl_addr = fls->fl_addr;
				if(fl_size != NULL)
					*fl_size = fls->fl_size;
				// printf("\nmbr region: %s check ok\n", fls->name);
				return true;
			}
			return false;
		}
		fls++;
		length += sizeof(struct fls_fmt_data);
		if(length >= MBR_SIZE)
			break;
	}

	return false;
}

void print_mbr_info(struct bl2fw_rsv_data *bbfw_main, struct bl2fw_rsv_data *bbfw_bak)
{
	struct fls_fmt_data * fls; 
	int i, num_0xff, length;
	u16 crc16_v;
		
	fls = (struct fls_fmt_data *)mbr_dbuf;
	length = 0;

	printf("\r\n");	
	printf("################### MBR Info ##################\n"); 
	while(1) {
		num_0xff = 0;
		crc16_v = 0;
		i = 0;

		for(i = 0; i < 16; i++) {
			if((unsigned char)fls->name[i] == 0xff)
				num_0xff ++;
		}

		if(num_0xff == 16)
			break;
		crc16_v = crc16((void*)fls, sizeof(struct fls_fmt_data) - 2);
 
		if(fls->flags & FLS_DATA_FLAGS_VALID) {
			printf("Name           : %s \n",  fls->name);
			printf("flash address  : 0x%x\n", fls->fl_addr);
			printf("flash size     : 0x%x (%dK)\n", fls->fl_size, fls->fl_size/1024);
			printf("crc16          : 0x%x\n", fls->crc16);
			printf("crc            : %s\n\n", (crc16_v == fls->crc16) ? "pass":"failed"); 

            if(0 == strncmp(fls->name, BB_FW_NAME, sizeof(BB_FW_NAME)))
            {
                //bbfw_main->load_addr = 0x100000;
                //bbfw_main->exec_addr = 0x100000;
                bbfw_main->fl_addr = fls->fl_addr;
                bbfw_main->dsize = fls->fl_size;
                bbfw_main->flags = fls->flags;
                bbfw_main->valid = 1;
                bbfw_main->err_return = 0;
            }

            if(0 == strncmp(fls->name, BB_FW_BACKUP_NAME, sizeof(BB_FW_BACKUP_NAME)))
            {
                //bbfw_bak->load_addr = 0x100000;
                //bbfw_bak->exec_addr = 0x100000;
                bbfw_bak->fl_addr = fls->fl_addr;
                bbfw_bak->dsize = fls->fl_size;
                bbfw_bak->flags = fls->flags;
                bbfw_bak->valid = 1;
                bbfw_bak->err_return = 0;
            }
		}

		fls++;
		length += sizeof(struct fls_fmt_data);
		if(length >= MBR_SIZE)
			break;
	}

	printf("############### Total %d partition #############\n", 
			length/sizeof(struct fls_fmt_data));	
	printf("\r\n");
}

extern int spinor_hw_read_page(u32 fl_addr, u8* rbuf, int len);
bool read_mbr_cfg(void)
{
    int offset, i;
    int fl_pg_size = 256, block_count = MBR_SIZE / fl_pg_size;
    u8 *rd_page_buf = get_flash_mbr_dbuf();

    offset = 0;
    for (i = 0; i < block_count; i++) {
        offset = spinor_hw_read_page((i * fl_pg_size), rd_page_buf, fl_pg_size);
        if (offset < 0) {
            printf("read mbr cfg failed, and qspi read timeout\n");
            return false;
        }
        rd_page_buf += offset;
        if ((*(rd_page_buf - 1) == 0xff) && (*(rd_page_buf - 2) == 0xff) &&
            (*(rd_page_buf - 3) == 0xff))
            break;
    }
    return true;
}

bool copy_mbr_cfg_from_rsv(void)
{
    u8 * mbr_buf;
    reserved_save =(struct reserved_area_data*)CONFIG_BOOT_RSVADDR_START;
    mbr_buf = get_flash_mbr_dbuf();
    memcpy(mbr_buf, reserved_save->mbr_data, MBR_SIZE);
    // print_mbr_info(&bbfw_main, &bbfw_bak);
    return true;
}

