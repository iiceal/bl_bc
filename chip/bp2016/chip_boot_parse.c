#include "common.h"
#include "regs/scm.h"
#include "bl2fw_if.h"
#include "mbr_if.h"
#include "rsv_save.h"

//#define BL2FW_DEBUG
//cbp = chip boot parse

//#define CBP_DEBUG

#ifdef CBP_DEBUG
#define cbp_debug(fmt, args...)  printf("Chip boot parse: " fmt "\n", ##args);
#else
#define cbp_debug(fmt, args...)
#endif

extern void cpu1_rom_area_set(U32 start, U32 end);
extern void cpu1_ram_area_set(U32 start, U32 end);

extern int spinor_hw_read_page(u32 fl_addr, u8* rbuf, int len);

struct reserved_area_data * reserved_save;

struct	bl2fw_rsv_data bbfw_main, bbfw_bak;

bool bl2fw_data_save(struct bl2fw_rsv_data * bl2fw_save, u32 bl2fw_type, u32 fl_addr, bool se);
extern void v7_dma_flush_range(u32, u32);
unsigned long do_go_exec (unsigned long (*entry)(int), unsigned size)
{
    return entry(0);
}

#if 1
static u8   rd_in_buf[2*256] __attribute__((__aligned__(64)));
bool check_bl2fw_hdr(char * bl_name, u32 bl2fw_type, u32 * fl_addr)
{
    u32  fl_size;
    int  fl_pg_size = 256;
    u8 * rd_hdr_buf = get_bl2fw_hdr_dbuf();

    if(get_flash_mbr(bl_name, fl_addr, &fl_size) == false) {
        cbp_debug("bl2fw.mbr error, name: %s\n", bl_name);
        return false;
    }
	
    if((fl_size % fl_pg_size) != 0) {//must be aligned 256 
        cbp_debug("1.bl2fw.size error, name: %s\n", bl_name);
        return false;
    }
    spinor_hw_read_page(*fl_addr, rd_hdr_buf, fl_pg_size);
    spinor_hw_read_page(*fl_addr + fl_pg_size, rd_hdr_buf + fl_pg_size, fl_pg_size);
    if(is_bl2fw_hdr_valid(bl2fw_type) == false) {
        cbp_debug("2.bl2fw.size error, name: %s\n", bl_name);
        return false;
    }	

    print_bl2fw_hdr_info();

    return true;
}

bool check_bl2fw_sign(char * bl_name, u32 bl2fw_type, struct  bl2fw_rsv_data * r)
{
    u32  fl_addr, fl_size, load_addr, fl_addr_save;
    int  fl_pg_size = 256;
    int  out_dsize = 0, page_count, offset = 0, i;
    u8 * out_data;

    if(check_bl2fw_hdr(bl_name, bl2fw_type, &fl_addr) == false)
        return false;

    fl_addr += get_bl2fw_hdr_fls_addr();
    fl_size = get_bl2fw_hdr_fls_size();
    load_addr = get_bl2fw_hdr_load_addr();
    fl_addr_save = fl_addr;

    out_dsize = fl_size;
    out_data = (u8*) load_addr;

    page_count = fl_size / fl_pg_size;
    offset = 0;
    cbp_debug("%s: fl_addr 0x%x cnt %d  load to 0x%x\n", __func__, fl_addr, page_count, load_addr);
    printf("Bink: qspi read bbfw begin.\n");
    for(i = 0; i< page_count; i++) {
        //this copy maybe  delete
        spinor_hw_read_page(fl_addr + offset, rd_in_buf, fl_pg_size);
        memcpy(out_data + offset, rd_in_buf, fl_pg_size);
        offset += fl_pg_size;
    }
    printf("Bink: qspi read bbfw done.\n");
    out_dsize = out_dsize;
#if 0  
    if(is_bl2fw_data_valid(0, out_data, out_dsize, false) == false) {
        cbp_debug("bl2fw.security check data error, name: %s\n", bl_name);
        return false;	
    }
#endif
    bl2fw_data_save(r, bl2fw_type, fl_addr_save, false);
#if 1
    r->valid = 1;
    r->fl_addr = fl_addr;
    r->load_addr = load_addr; 
    r->exec_addr = get_bl2fw_hdr_exec_addr();
    r->dsize = fl_size;
    r->flags = get_bl2fw_hdr_flags();  
#endif
    return true;		
}

int check_oem_bbfw_sign(void)
{
    u32  OEM_BB_FLASH_STORE_ADDR = 0x300000;
    u32  OEM_BB_DDR_STORE_EXEC_ADDR = 0x20000000;
    
    u32  fl_addr;
    int  fl_size;
    int  fl_pg_size = 256;
    int  out_dsize = 0, page_count, offset = 0, i;
    u8 * out_data;

    spinor_hw_read_page(OEM_BB_FLASH_STORE_ADDR, rd_in_buf, fl_pg_size);
    //fl_size = ((u32*)rd_in_buf)[0];
    memcpy(&fl_size,rd_in_buf,4);
    
    if(fl_size < 0)
    {
        printf("Bink: get fw size = %d from fw hdr.\n",fl_size);
        return -1;
    }
    out_dsize = fl_size;
    out_data = (u8*)OEM_BB_DDR_STORE_EXEC_ADDR;

    fl_addr = OEM_BB_FLASH_STORE_ADDR + fl_pg_size;
    page_count = fl_size / fl_pg_size;
    cbp_debug("%s: fl_addr 0x%x cnt %d  load to 0x%x\n", __func__, fl_addr, page_count, load_addr);
    printf("Bink: qspi read bbfw begin. fl_size = %d.\n",fl_size);
    for(i = 0; i< page_count; i++) {
        //this copy maybe  delete
        spinor_hw_read_page(fl_addr + offset, rd_in_buf, fl_pg_size);
        memcpy(out_data + offset, rd_in_buf, fl_pg_size);
        offset += fl_pg_size;
    }
    printf("Bink: qspi read bbfw done.out_dsize = %d\n",out_dsize);
    out_dsize = out_dsize;

    return out_dsize;		
}


static void control_swrst_ctrl_regs(int bit)                                                                                             
{
    u32 swrst = hwp_apSCM->swrst_ctrl;
    swrst &= ~(1 << bit);
    hwp_apSCM->swrst_ctrl = swrst;
    mdelay(10);
    swrst |= (1 << bit);
    hwp_apSCM->swrst_ctrl = swrst;
}

static void swrst_cpu1_reset(void)
{
    control_swrst_ctrl_regs(0);
}

static void wakeup_cpu1(u32 exec_addr, u32 dtb_addr)
{
    u32 magic_v = 0xFAAE2000;
    if(dtb_addr == 0) 
        magic_v = 0;

    hwp_apSCM->cpu1right_ctrl = 0x1fffffff; //access permission
    hwp_apSCM->cpu1trap_addr = 0xFFFFFFFF;

    hwp_apSCM->cpu1zero_addr = 0xf000;
    //cpu1_rom_area_set(0xfc00, 0x10000);
    hwp_apSCM->cpu1rom_baddr = 0xf000;
    hwp_apSCM->cpu1rom_eaddr = 0x10000;

    hwp_apSCM->rsv_1160_11ec[0] = magic_v;
    hwp_apSCM->rsv_1160_11ec[1] = dtb_addr;
    hwp_apSCM->sw_ap_jaddr = exec_addr;
    hwp_apSCM->sw_ap_magic = 0xfaae1000;
//    hwp_apSCM->sw_ap_magic = 0;
//    cbp_debug("de-assert cpu1 reset..., and ignore any reset opertion\n");
    
    //__asm__("b .");

    cbp_debug("de-assert cpu1 reset\n");
    swrst_cpu1_reset();
}

void before_boot_ecos_or_linux(void)
{ 
    #define     CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE      (256 * 1024)
    /*
     * For linux: LoadAddress is the DTB_START address
     *            ExecAddress is the Kernel Boot address
    */

    struct  bl2fw_rsv_data * r = &reserved_save->apfw;
    if((r->valid != 1) || (r->err_return != BL2FW_SUCCESS)) {
        if(r->err_return != BL2FW_ERR_HDR_INVALID)
            return;
    
        cbp_debug("**AP FW hdr error, and re-check its' hdr\r\n");
        if(check_bl2fw_sign(AP_FW_NAME, BL2FW_TYPE_AP_FW, r) == false)
            return;
        if(is_bl2fw_hdr_linux_flags() == false)
            return;

        cbp_debug("r->dsize = 0x%x\n", r->dsize);
        if(r->dsize <= 256*1024)
            return;

        cbp_debug("**AP FW is linux kernel\r\n");
        cbp_debug("memcpy from 0x%x to 0x%x, size = 0x%x\n", 
                    r->load_addr + CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE,
                    r->exec_addr, r->dsize - CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE);
        memcpy((void*)r->exec_addr, (void*)(r->load_addr + CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE),
                            r->dsize - CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE);
        v7_dma_flush_range(r->exec_addr, r->exec_addr + r->dsize-CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE);
        v7_dma_flush_range(r->load_addr, r->load_addr + CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE);
        cbp_debug("boot linux ...\n");
        wakeup_cpu1(r->exec_addr, r->load_addr);
    }
}
#endif

void boot_ecos_or_linux(void)
{
    struct  bl2fw_rsv_data *bbfw;
#if 0
    struct  bl2fw_rsv_data *bbfw, *apfw;
    apfw = &reserved_save->apfw;
    if((apfw->valid == 1) && (apfw->err_return == BL2FW_SUCCESS)) {
         if(apfw->exec_addr != apfw->load_addr) {
            cbp_debug("1. memcpy ...\n");
            memcpy((void*)apfw->exec_addr, (void*)apfw->load_addr, apfw->dsize);
            cbp_debug("2. flush\n\n\n");
            v7_dma_flush_range(apfw->exec_addr, apfw->exec_addr + apfw->dsize);
        }
        wakeup_cpu1(apfw->exec_addr, 0);
    }
#endif

    bbfw = &reserved_save->bbfw;
    if((bbfw->valid == 1) && (bbfw->err_return == BL2FW_SUCCESS)) {
        //read flash
        if(bbfw->exec_addr != bbfw->load_addr) {
            cbp_debug("1. memcpy ...\n");
            memcpy((void*)bbfw->exec_addr, (void*)bbfw->load_addr, bbfw->dsize);
            cbp_debug("2. flush\n\n\n");
            v7_dma_flush_range(bbfw->exec_addr, bbfw->exec_addr + bbfw->dsize);
        }
        cbp_debug("3. jump to bbfw\n\n\n");
        do_go_exec((void*)bbfw->exec_addr, 0);
    }
}

void boot_bb_fw(void)
{
#if 0
    //struct  bl2fw_rsv_data * bl2fw_save = &reserved_save->bbfw;
    struct  bl2fw_rsv_data * bl2fw_save = &bbfw_main;
    if ( check_bl2fw_sign( BB_FW_NAME, BL2FW_TYPE_BB_FW, bl2fw_save) == false )
    {
        //trigger_vcs_memory_dump(2);
        printf("Bink: check bb done. do dma flush begin.\n");
        cbp_debug("verify bbfw-backup\n");
        if ( check_bl2fw_sign( BB_FW_BACKUP_NAME, BL2FW_TYPE_BB_FW, bl2fw_save) == true){
            v7_dma_flush_range(bl2fw_save->exec_addr, bl2fw_save->exec_addr + bl2fw_save->dsize);
            printf("Bink : go exec bb bk fw.\n");
            do_go_exec( (void *)bl2fw_save->exec_addr, 0 );
        }
        // goto _normal_boot;
    }else{
        //boot bb
        printf("Bink: check bb bk done. do dma flush begin.\n");
        cbp_debug("boot bb fw! addr 0x%x\n", bl2fw_save->exec_addr);
        v7_dma_flush_range(bl2fw_save->exec_addr, bl2fw_save->exec_addr + bl2fw_save->dsize);
        printf("Bink : go exec bb fw.\n");
        do_go_exec( (void *)bl2fw_save->exec_addr, 0 );
    }
#endif
    u32  OEM_BB_DDR_STORE_EXEC_ADDR = 0x20000000;
    int  oem_bb_fw_size = 0;
    oem_bb_fw_size = check_oem_bbfw_sign();
    if(oem_bb_fw_size < 0)
        return;
    printf("Bink: check bb bk done. do dma flush begin.\n");
    cbp_debug("boot bb fw! addr 0x%x. fw size = %d.\n", OEM_BB_DDR_STORE_EXEC_ADDR,oem_bb_fw_size);
    v7_dma_flush_range(OEM_BB_DDR_STORE_EXEC_ADDR, 
            OEM_BB_DDR_STORE_EXEC_ADDR+oem_bb_fw_size);
    printf("Bink : go exec bb fw.\n");
    do_go_exec( (void *)OEM_BB_DDR_STORE_EXEC_ADDR, 0 );

}

int print_bl2fw_info(struct  bl2fw_rsv_data * r, bool se, const char * info)
{
    int ret = 0;
    cbp_debug("\r\n********%s***********\r\n", info);
	cbp_debug("cpu flash addr : 0x%x\n", r->fl_addr);
	cbp_debug("cpu size       : 0x%x (%dK)\n", r->dsize, r->dsize/1024);
	cbp_debug("cpu load addr  : 0x%x\n", r->load_addr);
	cbp_debug("cpu exec addr  : 0x%x\n", r->exec_addr);
	cbp_debug("flags          : 0x%x\n", r->flags);
	cbp_debug("security %s\n\n", (se ==true) ? "enabled":"disabled");
	if(r->valid == 1 && r->err_return == BL2FW_SUCCESS){
    
	    cbp_debug("bootrom check %s Pass\n", info);
    }else {
	    cbp_debug("valid = %d, err = %d\n", r->valid, r->err_return);
        ret = -1;
	    switch(r->err_return) {
        case BL2FW_ERR_MBR_NOT_FOUND:
            cbp_debug("mbr not found!\n");
            break;
        case BL2FW_ERR_MBR_INVALID:
            cbp_debug("mbr invalid!\n");
            break;
        case BL2FW_ERR_HDR_INVALID:
            cbp_debug("hdr invalid!\n");
            break;
        case BL2FW_ERR_RSA_KEY_INDEX_INVALID:
            cbp_debug("rsa_key_index invalid!\n");
            break;
        case BL2FW_ERR_RSA_FAILED:
            cbp_debug("rsa failed!\n");
            break;
        case BL2FW_ERR_RSA_KEY_VALUE_INVALID:
            cbp_debug("rsa_key_value invalid!\n");
            break;
        case BL2FW_ERR_GET_RSA_KEY_FAILED:
            cbp_debug("rsa_get_rsa_key failed!\n");
            break;
        case BL2FW_ERR_AES_INIT_FAILED:
            cbp_debug("aes_init failed!\n");
            break;
        case BL2FW_ERR_DATA_INVALID:
            cbp_debug("data invalid!\n");
            break;
	    }
	}
    cbp_debug("\r\n#########%s end#########\r\n", info);
    return ret;
}

void print_stat_time(void)
{
#if 0
    u32 us_total;
    u32 init0_time_count = reserved_save->init0_time_count ;
    u32 init1_time_count = reserved_save->init1_time_count;
    u32 auth_time_count  = reserved_save->auth_time_count;
    u32 bl_time_count    = reserved_save->bl_time_count;
    u32 bbfw_time_count  = reserved_save->bbfw_time_count;
    u32 apfw_time_count  = reserved_save->apfw_time_count;
    u32 done_time_count  = reserved_save->done_time_count;

    us_total = init0_time_count/20;
    us_total += init1_time_count/20;
    us_total += auth_time_count/20;
    us_total += bl_time_count/20;
    us_total += bbfw_time_count/20;
    us_total += apfw_time_count/20;
    us_total += done_time_count/20;
    cbp_debug("init0: %d, us: %d\n", init0_time_count, init0_time_count/20);
    cbp_debug("init1: %d, us: %d\n", init1_time_count, init1_time_count/20);
    cbp_debug("auth : %d, us: %d\n", auth_time_count, auth_time_count/20);
    cbp_debug("bl   : %d, us: %d\n", bl_time_count, bl_time_count/20);
    cbp_debug("bbfw : %d, us: %d\n", bbfw_time_count, bbfw_time_count/20);
    cbp_debug("apfw : %d, us: %d\n", apfw_time_count, apfw_time_count/20);
    cbp_debug("done : %d, us: %d\n", done_time_count, done_time_count/20);
    cbp_debug("total us: %d, ms: %d\n", us_total, us_total/1000);
#endif
}

void print_misc_info(void)
{
    switch(reserved_save->boot_type) {
    case BP2016_BOOT_SPIFLASH_AHBREAD:
        cbp_debug("boot_type: AHBREAD\n");
        break;
    case BP2016_BOOT_SPIFLASH_QUAD:
        cbp_debug("boot_type: QUAD\n");
        break;
    case BP2016_BOOT_SPIFLASH_QUAD_DMA:
        cbp_debug("boot_type: QUAD+DMA\n");
        break;
    case BP2016_BOOT_SIMPLE:
        cbp_debug("boot_type: SIMPLE\n");
        break;
    }
    print_stat_time();

    cbp_debug("eFuse is %s\n", (reserved_save->efuse_valid > 0) ? "valid":"invalid");
    cbp_debug("authcode is %s\n", (reserved_save->authcode_valid > 0) ? "valid":"invalid");
}

void boot_parse_init(void)
{
    //u8 * mbr_dbuf, * auth_dbuf;
    //int ret = 0;
//    u8 * mbr_dbuf;
//    reserved_save =(struct reserved_area_data*)CONFIG_BOOT_RSVADDR_START;
//    mbr_dbuf = get_flash_mbr_dbuf();
//    memcpy(mbr_dbuf, reserved_save->mbr_data, MBR_SIZE);
    //auth_dbuf = get_auth_dbuf();
    //memcpy(auth_dbuf, reserved_save->auth_code, AUTH_CODE_DBUF_SIZE);

    cbp_debug("\r\n----------------boot_parse_init------------------\r\n");
    boot_bb_fw();
}

void print_boot_par(void)
{
    char *buf = (char *)CONFIG_ROM_RSVADDR;
    int i=0;

    for(i=0; i<8192; i+=16)
    {
        printf("0x%x: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
                i, buf[i], buf[i+1], buf[i+2], buf[i+3], buf[i+4], buf[i+5], buf[i+6],
                buf[i+7], buf[i+8], buf[i+9], buf[i+10], buf[i+11], buf[i+12], buf[i+13],
                buf[i+14], buf[i+15]);
    }
}

void copy_rom_run_paramter_to_dst(void)
{
    memcpy((void *)CONFIG_BOOT_RSVADDR_START, (void *)CONFIG_ROM_RSVADDR, 8192);
}

bool bl2fw_data_save_err_info(u32 bl2fw_type, int err)
{
    switch(bl2fw_type) {
    case BL2FW_TYPE_BL:
        if(reserved_save->bl.err_return == 0)
            reserved_save->bl.err_return = err;
        break;
    case BL2FW_TYPE_BB_FW:
        if(reserved_save->bbfw.err_return == 0)
            reserved_save->bbfw.err_return = err;
        break;
    case BL2FW_TYPE_AP_FW:
        if(reserved_save->apfw.err_return == 0)
            reserved_save->apfw.err_return = err;
        break;
    default:
        return false;
    }
    return true;
}

bool bl2fw_data_save(struct bl2fw_rsv_data * bl2fw_save, u32 bl2fw_type, u32 fl_addr, bool se)
{
    switch(bl2fw_type) {
    case BL2FW_TYPE_BL:
        bl2fw_save = &reserved_save->bl;
        reserved_save->bl_se = se;
        break;
    case BL2FW_TYPE_BB_FW:
        bl2fw_save = &reserved_save->bbfw;
        reserved_save->bbfw_se = se;
        break;
    case BL2FW_TYPE_AP_FW:
        bl2fw_save = &reserved_save->apfw;
        reserved_save->apfw_se = se;
        reserved_save->apfw_key_en = false;// is_ap_key_enabled() ;
        break;
    default:
        return false;
    }
    bl2fw_save->valid = 1;
    bl2fw_save->fl_addr = fl_addr;
    bl2fw_save->load_addr = get_bl2fw_hdr_load_addr();
    bl2fw_save->exec_addr = get_bl2fw_hdr_exec_addr();
    bl2fw_save->dsize = get_bl2fw_hdr_fls_size();
    bl2fw_save->flags = get_bl2fw_hdr_flags();
    return true;
}
