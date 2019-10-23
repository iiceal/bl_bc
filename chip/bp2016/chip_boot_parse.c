#include "common.h"
#include "regs/scm.h"
#include "bl2fw_if.h"
#include "mbr_if.h"
#include "rsv_save.h"

#ifdef CONFIG_ENABLE_TRACE
#define parse_debug(fmt, args...)  printf("%s: %d " fmt, __func__, __LINE__, ##args)
#else
#define parse_debug(fmt, args...)
#endif

extern int spinor_hw_read_page(u32 fl_addr, u8* rbuf, int len);
extern void v7_dma_flush_range(u32, u32);

struct	bl2fw_rsv_data bbfw_main, bbfw_bak;

unsigned long do_go_exec (unsigned long (*entry)(int), unsigned size)
{
    return entry(0);
}

static u8   rd_in_buf[256] __attribute__((__aligned__(64)));
bool check_bl2fw_hdr(char * bl_name, u32 bl2fw_type, u32 * fl_addr)
{
    u32  fl_size;
    int  fl_pg_size = 256;
    u8 * rd_hdr_buf = get_bl2fw_hdr_dbuf();

    if(get_flash_mbr(bl_name, fl_addr, &fl_size) == false) {
        printf("bl2fw.mbr error, name: %s\n", bl_name);
        return false;
    }
	
    if((fl_size % fl_pg_size) != 0) {//must be aligned 256 
        printf("1.bl2fw.size error, name: %s\n", bl_name);
        return false;
    }
    spinor_hw_read_page(*fl_addr, rd_hdr_buf, fl_pg_size);
    spinor_hw_read_page(*fl_addr + fl_pg_size, rd_hdr_buf + fl_pg_size, fl_pg_size);
    if(is_bl2fw_hdr_valid(bl2fw_type) == false) {
        printf("2.bl2fw.size error, name: %s\n", bl_name);
        return false;
    }	

    // print_bl2fw_hdr_info();

    return true;
}

#define PINGPONG_BUFSIZE 256
u8 in_buf[PINGPONG_BUFSIZE] __attribute__((__aligned__(64)));
u8 _in_buf_pong[PINGPONG_BUFSIZE] __attribute__((__aligned__(64)));
extern int flash_read_dma_init_once(u32 len);
extern int qspi_dma_read_start_once(u8 cmd, int rx_len, int dma_rdlr);
extern int qspi_dma_read_start_new(u8 cmd, u32 addr);
extern int flash_dma_read_start(u32 fl_addr, u8 *rx_dbuf, int rx_len);
extern int flash_dma_read_wait_done(void);
extern int flash_dma_read_finished(void);
extern void v7_dma_inv_range(u32, u32);
extern bool is_bl2fw_data_valid_new(u32 crc32_v, u8 *input_data, int dsize, bool check_chipid);

bool check_bl2fw_sign_async(char *bl_name, u32 bl2fw_type, struct bl2fw_rsv_data *r)
{
    u32 fl_addr, fl_size, load_addr;
    int fl_pg_size = 256;
    int page_count, offset = 0, i;
    u8 *out_data, *inbuf_ping, *inbuf_pong, *inb_op, *inb_rd;
    u32 crc32_v = 0;

    if(check_bl2fw_hdr(bl_name, bl2fw_type, &fl_addr) == false)
        return false;

    fl_addr += get_bl2fw_hdr_fls_addr();
    fl_size = get_bl2fw_hdr_fls_size();
    load_addr = get_bl2fw_hdr_load_addr();
    out_data = (u8 *) load_addr;

    page_count = fl_size / fl_pg_size;
    offset = 0;
    parse_debug("%s: fl_addr 0x%x cnt %d  load to 0x%x\n", __func__, fl_addr, page_count, load_addr);

    inbuf_ping = in_buf;
    inbuf_pong = _in_buf_pong;
    inb_rd = inbuf_ping;

    flash_read_dma_init_once(PINGPONG_BUFSIZE);
    qspi_dma_read_start_once(0xeb, PINGPONG_BUFSIZE, 15);
    // first block
    flash_dma_read_start(fl_addr + offset, inb_rd, fl_pg_size);
    flash_dma_read_wait_done();

    for (i = 1; i < page_count; i++) {
        inb_op = inb_rd;
        inb_rd = ((i % 2) == 0) ? inbuf_ping : inbuf_pong;
        offset += fl_pg_size;
        flash_dma_read_start(fl_addr + offset, inb_rd, fl_pg_size);
        v7_dma_inv_range((u32) inb_op, (u32)(inb_op + fl_pg_size)); // fixed cpu prefech issue
        memcpy(out_data, inb_op, fl_pg_size);
        crc32_v = crc32(crc32_v, inb_op, fl_pg_size);
        out_data += fl_pg_size;
        flash_dma_read_wait_done();
    }
    // last block
    inb_op = inb_rd;
    v7_dma_inv_range((u32) inb_op, (u32)(inb_op + fl_pg_size)); // fixed cpu prefech issue
    memcpy(out_data, inb_op, fl_pg_size);
    crc32_v = crc32(crc32_v, inb_op, (fl_pg_size - 4));

    flash_dma_read_finished();
    out_data = (u8 *) load_addr;
    if (is_bl2fw_data_valid_new(crc32_v, out_data, fl_size, false) == false) {
        printf("bl2fw.security check data error, name: %s\n", bl_name);
        return false;
    }

    r->valid = 1;
    r->fl_addr = fl_addr;
    r->load_addr = load_addr;
    r->exec_addr = load_addr;
    r->dsize = fl_size;
    r->flags = get_bl2fw_hdr_flags();

    return true;
}

bool check_bl2fw_sign(char * bl_name, u32 bl2fw_type, struct  bl2fw_rsv_data * r)
{
    u32  fl_addr, fl_size, load_addr;
    int  fl_pg_size = 256;
    int  out_dsize, page_count, offset = 0, i;
    u8 * out_data;

    if(check_bl2fw_hdr(bl_name, bl2fw_type, &fl_addr) == false)
        return false;

    fl_addr += get_bl2fw_hdr_fls_addr();
    fl_size = get_bl2fw_hdr_fls_size();
    load_addr = get_bl2fw_hdr_load_addr();

    out_dsize = fl_size;
    out_data = (u8*) load_addr;

    page_count = fl_size / fl_pg_size;
    offset = 0;
    printf("%s: fl_addr 0x%x cnt %d  load to 0x%x\n", __func__, fl_addr, page_count, load_addr);
    for(i = 0; i< page_count; i++) {
        //this copy maybe  delete
        spinor_hw_read_page(fl_addr + offset, rd_in_buf, fl_pg_size);
        memcpy(out_data + offset, rd_in_buf, fl_pg_size);
        offset += fl_pg_size;
    }

    if(is_bl2fw_data_valid(0, out_data, out_dsize, false) == false) {
        printf("bl2fw.security check data error, name: %s\n", bl_name);
        return false;	
    }
    
    //bl2fw_data_save(r, bl2fw_type, fl_addr_save, false);

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

#ifdef CONFIG_FASTBOOT
    #include "fastboot.h"
#endif

void boot_bb_fw_speed(void)
{
    struct bl2fw_rsv_data *bl2fw_save = &bbfw_main;
    U32 bak = 0;
    if (check_bl2fw_sign_async(BB_FW_NAME, BL2FW_TYPE_BB_FW, bl2fw_save) == false) {
        parse_debug("verify bbfw-backup\n");
        if (check_bl2fw_sign_async(BB_FW_BACKUP_NAME, BL2FW_TYPE_BB_FW, bl2fw_save) == false) {
            parse_debug("error!\n");
            return;
        }
        bak = 1;
    }
    v7_dma_flush_range(bl2fw_save->exec_addr, bl2fw_save->exec_addr + bl2fw_save->dsize);
#ifdef CONFIG_FASTBOOT
    fastboot_save_para(bl2fw_save->exec_addr, bl2fw_save->dsize, bl2fw_save->fl_addr, bak);
#endif
    printf("boot bb fw! addr 0x%x\n", bl2fw_save->exec_addr);
    do_go_exec((void *) bl2fw_save->exec_addr, 0);
}

void boot_bb_fw(void)
{
    //struct  bl2fw_rsv_data * bl2fw_save = &reserved_save->bbfw;
    struct  bl2fw_rsv_data * bl2fw_save = &bbfw_main;
    U32 bak = 0;
    if ( check_bl2fw_sign( BB_FW_NAME, BL2FW_TYPE_BB_FW, bl2fw_save) == false )
    {
        //trigger_vcs_memory_dump(2);
        parse_debug("\nverify bbfw-backup ");
        if ( check_bl2fw_sign( BB_FW_BACKUP_NAME, BL2FW_TYPE_BB_FW, bl2fw_save) == false){
            parse_debug("error!\n");
            return;
        }
        bak = 1;
    }
    //boot bb
    v7_dma_flush_range(bl2fw_save->exec_addr, bl2fw_save->exec_addr + bl2fw_save->dsize);
#ifdef CONFIG_FASTBOOT
    fastboot_save_para(bl2fw_save->exec_addr, bl2fw_save->dsize, bl2fw_save->fl_addr, bak);
#endif
    printf("boot bb fw! addr 0x%x\n", bl2fw_save->exec_addr);
    do_go_exec( (void *)bl2fw_save->exec_addr, 0 );
}

int print_bl2fw_info(struct  bl2fw_rsv_data * r, bool se, const char * info)
{
    int ret = 0;
    printf("\r\n********%s***********\r\n", info);
	printf("cpu flash addr : 0x%x\n", r->fl_addr);
	printf("cpu size       : 0x%x (%dK)\n", r->dsize, r->dsize/1024);
	printf("cpu load addr  : 0x%x\n", r->load_addr);
	printf("cpu exec addr  : 0x%x\n", r->exec_addr);
	printf("flags          : 0x%x\n", r->flags);
	printf("security %s\n\n", (se ==true) ? "enabled":"disabled");
	if(r->valid == 1 && r->err_return == BL2FW_SUCCESS) 
	    printf("bootrom check %s Pass\n", info);
	else {
	    printf("valid = %d, err = %d\n", r->valid, r->err_return);
        ret = -1;
	    switch(r->err_return) {
        case BL2FW_ERR_MBR_NOT_FOUND:
            printf("mbr not found!\n");
            break;
        case BL2FW_ERR_MBR_INVALID:
            printf("mbr invalid!\n");
            break;
        case BL2FW_ERR_HDR_INVALID:
            printf("hdr invalid!\n");
            break;
        case BL2FW_ERR_RSA_KEY_INDEX_INVALID:
            printf("rsa_key_index invalid!\n");
            break;
        case BL2FW_ERR_RSA_FAILED:
            printf("rsa failed!\n");
            break;
        case BL2FW_ERR_RSA_KEY_VALUE_INVALID:
            printf("rsa_key_value invalid!\n");
            break;
        case BL2FW_ERR_GET_RSA_KEY_FAILED:
            printf("rsa_get_rsa_key failed!\n");
            break;
        case BL2FW_ERR_AES_INIT_FAILED:
            printf("aes_init failed!\n");
            break;
        case BL2FW_ERR_DATA_INVALID:
            printf("data invalid!\n");
            break;
	    }
	}
    printf("\r\n#########%s end#########\r\n", info);
    return ret;
}

void copy_rom_run_paramter_to_dst(void)
{
    memcpy((void *)CONFIG_BOOT_RSVADDR_START, (void *)CONFIG_ROM_RSVADDR, 8192);
}

extern bool read_mbr_cfg(void);
extern bool copy_mbr_cfg_from_rsv(void);
void boot_parse_init(void)
{
    copy_rom_run_paramter_to_dst();
    copy_mbr_cfg_from_rsv();
    boot_bb_fw_speed();
}

#if 0
void print_stat_time(void)
{
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
    printf("init0: %d, us: %d\n", init0_time_count, init0_time_count/20);
    printf("init1: %d, us: %d\n", init1_time_count, init1_time_count/20);
    printf("auth : %d, us: %d\n", auth_time_count, auth_time_count/20);
    printf("bl   : %d, us: %d\n", bl_time_count, bl_time_count/20);
    printf("bbfw : %d, us: %d\n", bbfw_time_count, bbfw_time_count/20);
    printf("apfw : %d, us: %d\n", apfw_time_count, apfw_time_count/20);
    printf("done : %d, us: %d\n", done_time_count, done_time_count/20);
    printf("total us: %d, ms: %d\n", us_total, us_total/1000);
}

void print_misc_info(void)
{
    switch(reserved_save->boot_type) {
    case BP2016_BOOT_SPIFLASH_AHBREAD:
        printf("boot_type: AHBREAD\n");
        break;
    case BP2016_BOOT_SPIFLASH_QUAD:
        printf("boot_type: QUAD\n");
        break;
    case BP2016_BOOT_SPIFLASH_QUAD_DMA:
        printf("boot_type: QUAD+DMA\n");
        break;
    case BP2016_BOOT_SIMPLE:
        printf("boot_type: SIMPLE\n");
        break;
    }
    print_stat_time();

    printf("eFuse is %s\n", (reserved_save->efuse_valid > 0) ? "valid":"invalid");
    printf("authcode is %s\n", (reserved_save->authcode_valid > 0) ? "valid":"invalid");
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

struct reserved_area_data * reserved_save;
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
//    printf("de-assert cpu1 reset..., and ignore any reset opertion\n");
    
    //__asm__("b .");

    printf("de-assert cpu1 reset\n");
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
    
        printf("**AP FW hdr error, and re-check its' hdr\r\n");
        if(check_bl2fw_sign(AP_FW_NAME, BL2FW_TYPE_AP_FW, r) == false)
            return;
        if(is_bl2fw_hdr_linux_flags() == false)
            return;

        printf("r->dsize = 0x%x\n", r->dsize);
        if(r->dsize <= 256*1024)
            return;

        printf("**AP FW is linux kernel\r\n");
        printf("memcpy from 0x%x to 0x%x, size = 0x%x\n", 
                    r->load_addr + CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE,
                    r->exec_addr, r->dsize - CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE);
        memcpy((void*)r->exec_addr, (void*)(r->load_addr + CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE),
                            r->dsize - CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE);
        v7_dma_flush_range(r->exec_addr, r->exec_addr + r->dsize-CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE);
        v7_dma_flush_range(r->load_addr, r->load_addr + CONFIG_AP_LINUX_DTB_FIXED_MAX_SIZE);
        printf("boot linux ...\n");
        wakeup_cpu1(r->exec_addr, r->load_addr);
    }
}

void boot_ecos_or_linux(void)
{
    struct  bl2fw_rsv_data *bbfw;
#if 0
    struct  bl2fw_rsv_data *bbfw, *apfw;
    apfw = &reserved_save->apfw;
    if((apfw->valid == 1) && (apfw->err_return == BL2FW_SUCCESS)) {
         if(apfw->exec_addr != apfw->load_addr) {
            printf("1. memcpy ...\n");
            memcpy((void*)apfw->exec_addr, (void*)apfw->load_addr, apfw->dsize);
            printf("2. flush\n\n\n");
            v7_dma_flush_range(apfw->exec_addr, apfw->exec_addr + apfw->dsize);
        }
        wakeup_cpu1(apfw->exec_addr, 0);
    }
#endif

    bbfw = &reserved_save->bbfw;
    if((bbfw->valid == 1) && (bbfw->err_return == BL2FW_SUCCESS)) {
        //read flash
        if(bbfw->exec_addr != bbfw->load_addr) {
            printf("1. memcpy ...\n");
            memcpy((void*)bbfw->exec_addr, (void*)bbfw->load_addr, bbfw->dsize);
            printf("2. flush\n\n\n");
            v7_dma_flush_range(bbfw->exec_addr, bbfw->exec_addr + bbfw->dsize);
        }
        printf("3. jump to bbfw\n\n\n");
        do_go_exec((void*)bbfw->exec_addr, 0);
    }
}
#endif
