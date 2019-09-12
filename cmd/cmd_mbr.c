#include <bl_common.h>
#include <drivers/drv_qspi.h>
#include <hg_mbr.h>
#include <mbr_if.h>
#include "cmd_mbr.h"


#define MBR_MAX_CNT       128
#define MBR_STRUCT_SIZE   sizeof(struct fls_fmt_data)
#define MBR_FLASH_ADDR    0x00

static BL_CMD_T mbr_descriptor;

static u32 mbr_valid_cnt = 0;

//Risk: on this case which only stored the valid mbr info causes the risk that
//will destroy the origin flash data when update the FLASH data used mbr_entry.
static u8 entry_init_ok = 0;
struct fls_fmt_data mbr_entry[MBR_MAX_CNT];

bool mbr_flash_cfg_read(u8 *sbuf,u32 len)
{
    int  offset, i;
    int  fl_pg_size = 256, block_count = len / fl_pg_size;
    u8 * rd_page_buf = sbuf;

    offset = 0;

    if(false == init_spinor())
    {
        printf("\nSpinor init failed.\n");
        return -1;
    }

    for(i = 0; i< block_count; i++) {
        offset = spinor_hw_read_page((i*fl_pg_size), rd_page_buf, fl_pg_size);
        if(offset < 0) {
            printf("read mbr cfg failed, and qspi read timeout\n");
            return false;
        }
        rd_page_buf += offset;
        if((*(rd_page_buf -1) == 0xff) && (*(rd_page_buf - 2) == 0xff) && (*(rd_page_buf - 3) == 0xff))
            break;          
    }
    return true;
}

bool is_mbr_addr_valid(struct fls_fmt_data *mbr_info,MBR_NOCHECK_E no_check)
{
    u32 i;
    struct fls_fmt_data *p_mbr_entry;
    if(0 == mbr_valid_cnt)
        return true;

    if(0 == mbr_info->fl_size)
    {
        printf("\n Error, Length = 0x%x on MBR %s.\n\n",
                mbr_info->fl_size,mbr_info->name);
        return false;
    }

    p_mbr_entry = mbr_entry;
    for(i = 0; i < mbr_valid_cnt; i++)
    {
       if((no_check-1) == i)
       {
            p_mbr_entry++;
            continue;
       }

       if((mbr_info->fl_addr >= (p_mbr_entry->fl_addr + p_mbr_entry->fl_size))|| 
                ((mbr_info->fl_addr + mbr_info->fl_size) <= p_mbr_entry->fl_addr))
        {
            p_mbr_entry++;
            continue;
        }else
        {
            printf("\nMBR config info is invalid!!!\n");
            printf("%s [0x%08x - 0x%08x] is overlaped with %s [0x%08x - 0x%08x] !!!\n\n",
                    mbr_info->name,
                    mbr_info->fl_addr,
                    mbr_info->fl_addr + mbr_info->fl_size,
                    p_mbr_entry->name,
                    p_mbr_entry->fl_addr,
                    p_mbr_entry->fl_addr + p_mbr_entry->fl_size); 
            return false;
        }
        p_mbr_entry++;
    }
    return true;

}

bool is_mbr_name_valid(struct fls_fmt_data *mbr_info,MBR_NOCHECK_E no_check)
{
    //Check logic:The first two bytes of mbr name 
    //are not 0xff means the name is valid.
    u32 i;
    u32 ret;
    struct fls_fmt_data *p_mbr_entry;
    if((0xff == mbr_info->name[0]) && (0xff == mbr_info->name[1]))
        return false;

    p_mbr_entry = mbr_entry;
    for(i = 0; i < mbr_valid_cnt; i++)
    {
        
        if((no_check-1) == i)
        {
            p_mbr_entry++;
            continue;
        }
        ret = memcmp(mbr_info->name,p_mbr_entry->name,16);
#if 0 
        printf("Added name data:");
        for(j=0;j<16;j++)
            printf("%2x",mbr_info->name[j]);
        printf("\n");
        printf("cmp name data:");
        for(j=0;j<16;j++)
            printf("%2x",p_mbr_entry->name[j]);
        printf("\n ret = %d, i = %d \n\n",ret,i);
#endif        
        if(0 == ret)
        {
            printf("\nMBR config info is invalid!!!\n");
            printf("name:%s is conflict with MBR:%s [0x%08x - 0x%08x] !!!\n\n",
                    mbr_info->name,
                    p_mbr_entry->name,
                    p_mbr_entry->fl_addr,
                    p_mbr_entry->fl_addr + p_mbr_entry->fl_size); 
            return false;
        }
        p_mbr_entry++;
    }
    
   // printf("This name %s of mbr info is invalid, please double check it.\n",
   //         mbr_info->name);
    return true;
}

bool is_mbr_flag_valid(struct fls_fmt_data *mbr_info)
{

    //check the flags valid bit, if not 1 return false.
    if((mbr_info->flags & FLS_FLAGS_VALID) == 0)
    {
      //  printf("This flags [0x%x] of %s mbr info is invalid, please double check it.\n",
      //          mbr_info->flags,
      //          mbr_info->name);
        return false;
    }

    return true;
}

bool is_mbr_crc_valid(struct fls_fmt_data *mbr_info)
{
    //check crc.
    u16 crc;
    crc = crc16((void *)mbr_info, MBR_STRUCT_SIZE - 2);
    if(mbr_info->crc16 != crc)
    {
        return false;
    }

    return true;
}

bool is_mbr_valid(struct fls_fmt_data* mbr_info, MBR_NOCHECK_E no_check)
{

    //check the name of mbr info.
    if(false ==is_mbr_name_valid(mbr_info,no_check))
        return false;

    if(false ==is_mbr_flag_valid(mbr_info))
        return false;

    if(false == is_mbr_addr_valid(mbr_info,no_check))
        return false;

    if(false == is_mbr_crc_valid(mbr_info))
        return false;
    
    return true;
}
int _mbr_flash_update(u8 *mbr_data)
{
//    u8 *mbr_data;
    
    //Updata the mbr date in the ram reserved region.
    //Is it necessary ?
    //Actrually, I destroyed the origin mbr data distrubition.
    mbr_data = get_flash_mbr_dbuf();
    memcpy(mbr_data,(u8 *)mbr_entry, MBR_MAX_CNT * MBR_STRUCT_SIZE);
    
    printf("MBR data update on flash Done.\n");


//    upgrade_version_to_flash(0x0, mbr_data, MBR_MAX_CNT * MBR_STRUCT_SIZE);

    fl_upgrade_if(MBR_FLASH_ADDR,(u32)mbr_data,MBR_SIZE);
    return 0;

}

int mbr_modify(u32 index,struct fls_fmt_data *mbr)
{
    u32 final_index;
      
    if(index > mbr_valid_cnt)
    {
        printf("Add mbr failed, select mbr index is out of range.\n");
        return -1;
    }

    if(false == is_mbr_valid(mbr,(MBR_NOCHECK_E)index))
    {
        printf("Add mbr failed, please check the input Parameters.\n");
        return -1;
    }

    final_index = index -1;
    //notice the offset of mbr entry  
    memcpy((u8 *)(mbr_entry + final_index),(u8 *)mbr,MBR_STRUCT_SIZE);
    
    _mbr_flash_update((u8 *)mbr_entry);
    return 0;
}

int _mbr_add(struct fls_fmt_data *mbr)
{
    
    if(false == is_mbr_valid(mbr,MBR_CHECK_ALL))
    {
        printf("Add mbr failed, please check the input Parameters.\n");
        return -1;
    }

    //notice the offset of mbr entry  
    memcpy((u8 *)(mbr_entry + mbr_valid_cnt),(u8 *)mbr,MBR_STRUCT_SIZE);
    mbr_valid_cnt ++;

    _mbr_flash_update((u8 *)mbr_entry);
    return 0;
}

int _mbr_delete(u32 index)
{
    u32 i;
    u32 final_index ;
    //final index is (index -1) , because mbr dump index count begin 1;
    final_index = index -1;

//    printf("Debug, final_index = %d, index = %d, mbr_valid_cnt = %d\n",
//            final_index,index,mbr_valid_cnt);

    if(final_index >= mbr_valid_cnt)
    {
        printf("Delete MBR faild, index = %d is invalid Parameters.\n ",index);
        return -1;
    }

    //delete the select mbr;
    memset((u8 *)(mbr_entry + final_index), 0xff, MBR_STRUCT_SIZE);

    if(final_index == (mbr_valid_cnt -1))
    {
        mbr_valid_cnt--;
        _mbr_flash_update((u8 *)mbr_entry);
        return 0;
    }

    //sort the mbr entry after delete done;
    //move the post MBR to pre-place.
    for(i = final_index; i < mbr_valid_cnt; i++)
        memcpy((u8 *)(mbr_entry + i),(u8 *)(mbr_entry + i +1),MBR_STRUCT_SIZE);
   
    //delete the tail.
    if(final_index < (mbr_valid_cnt -1))
    {
        memset((u8 *)(mbr_entry + (mbr_valid_cnt -1)), 0xff, MBR_STRUCT_SIZE);
    }
    
    mbr_valid_cnt--;
    _mbr_flash_update((u8 *)mbr_entry);
    return 0;

}

u32 mbr_entry_init(void)
{
//    u8 mbr_data[MBR_SIZE];
    struct fls_fmt_data* fls;
    u32 i;
    //Get mbr info from ram resvered data region.
    //Shoud improvement that add the way to get data from FLASH.
    //such like: read_flash(0x00,(u8 *)mbr_entry,MBR_MAX_CNT * sizeof(struct fls_fmt_data));
    //mbr_data = get_flash_mbr_dbuf();
    //dump_mem_data(mbr_data,1024);

    memset((u8 *)mbr_entry, 0xff, MBR_MAX_CNT * MBR_STRUCT_SIZE);
    if(false == mbr_flash_cfg_read((u8 *)mbr_entry,MBR_SIZE))
    {
        printf("\nRead mbr cfg on flash Faild.\n");
        return -1;
    }
        //    memcpy((u8 *)mbr_entry, mbr_data, MBR_MAX_CNT * MBR_STRUCT_SIZE);
    
    fls = mbr_entry;
    
    for(i = 0; i < MBR_MAX_CNT; i++)
    {
       //mbr_entry only stored the valid mbr info.
       //valid mbr info is continous.
       if(true == is_mbr_valid(fls,MBR_CHECK_ALL))
       {
//            memcpy((u8 *)(mbr_entry + mbr_valid_cnt), (u8 *)fls, MBR_STRUCT_SIZE);
            mbr_valid_cnt++;
            fls++;
       }else{
            break; 
       }
    }
    
    if(0 == mbr_valid_cnt)
    {
        printf("No valid MBR found, please check it.\n");
        return -1;
    }

    return 0;
  //  mbr_entry_dump();
}

u32 mbr_entry_dump(void)
{
    u32 i;
    struct fls_fmt_data *p_mbr = mbr_entry;

    if(0 == mbr_valid_cnt)
    {
        printf("No valid mbr info show! please check it.\n");
        return -1; 
    }

    printf("%-19s%-12s%-12s%-10s\n","Name","Saddr","Eaddr","Length");
    for(i = 0; i < mbr_valid_cnt;i++)
    {
        printf("[%d]%-16s0x%08x  0x%08x  0x%08x\n",
                i+1,
                p_mbr->name,
                p_mbr->fl_addr,
                p_mbr->fl_addr + p_mbr->fl_size,
                p_mbr->fl_size);
        p_mbr ++;    
    }

    return 0;
} 


void dump_mem_data(u8 *data,u32 len)
{
    //Dump mem data:
    //fomate: four bytes for a word;
    //        four word for a row;
    int i,j;
    for(j = 0;j < len/4;j ++)
    {
        printf("0x");
        for(i = 0;i < 4;i++)
        {
            printf("%2x",data[i+j*4]);
        }
        printf(" ");
        if(j % 4 == 0)
            printf("\n");
    }
    printf("\n");
}

static void do_mbr_help(void)
{
    printf("\nUsage of mbr cmd:\n\n");
    
    printf("[1]show all mbr info.\n\n");
    printf(" mbr\n\n");
    
    printf("[2]modify select mbr .\n\n");
    printf(" mbr modify [sel] [part] [value]\n\n");
    printf("  [sel]:  index of mbr. \n\n");
    printf("  [part]: port of mbr you want to modify. valid: name, addr, len \n\n");
    printf("  [val:   value of the mbr you want to modify. \n\n");
    printf("example1: modify the third mbr  name to \"ap3fw\"\n\n");
    printf("  mbr modify 3 name ap3fw\n\n");
    printf("example2: modify the second mbr addr to 0x21200\n\n");
    printf("  mbr modify 2 addr 0x21200\n\n");
    printf("example3: modify the second mbr Length to 0x2100\n\n");
    printf("  mbr modify 2 len 0x2100\n\n");
    
    printf("[3]add an new MBR.\n\n");
    printf("  mbr add [name] [addr] [len]\n\n");
    printf("   [name]: name of the new MBR. \n\n");
    printf("   [addr]: start addr of the new MBR. \n\n");
    printf("   [len]:  len of the new MBR. \n\n");
    printf("  example: add a new MBR .name: ap3fw Saddr:0x21200 Length:0x2100\n\n");
    printf("    mbr add ap3fw 0x21200 0x2100\n\n");

    printf("[4]delete an exit MBR.\n\n");
    printf("  mbr del [sel]\n\n");
    printf("   [sel]: index of the MBR which you want to delete. \n\n");
    printf("  example: delete the second MBR.\n\n");
    printf("    mbr del 2\n\n");
    
    printf("[5]get mbr cmd help info.\n\n");
    printf("  mbr help \n\n");
}

static int do_mbr_delete_cmd(u32 index)
{
    return _mbr_delete(index);
}

static int do_mbr_add_cmd(u8 *name, u32 addr, u32 len)
{
    struct fls_fmt_data mbr;
    u32  crc;
    struct fls_fmt_data *p_mbr = &mbr;
    //p_mbr point init;
    memset((u8 *)p_mbr, 0xff, sizeof(struct fls_fmt_data));
    
    memcpy(p_mbr->name,name,16);
    p_mbr->fl_addr = addr;
    p_mbr->fl_size = len;

    //user can change flag bit if necessary.
    p_mbr->flags = FLS_FLAGS_VALID;
    
    crc = crc16((void *)p_mbr, MBR_STRUCT_SIZE - 2);
    p_mbr->crc16 = crc;

    if(0 > _mbr_add(p_mbr))
    {
        printf("Error, mbr name:%s saddr 0x%08x len:0x%08x ADD FAILED.\n\n ",
                name,addr,len);
        return -1;
    }

    return 0;
}


static int do_mbr_modify_cmd(u32 index,MBR_REGION_E region, void *val)
{
    u16 crc16_m;
    struct fls_fmt_data fls; 
    struct fls_fmt_data *p_fls = &fls;
    memset((u8 *)p_fls, 0xff, MBR_STRUCT_SIZE);    
    memcpy((u8 *)p_fls, (u8 *)(mbr_entry + (index -1)),MBR_STRUCT_SIZE);

    switch(region)
    {
        case MBR_REGION_ADDR:
            printf("\nModify %s MBR flash_addr value, before val:0x%8x, setting val:0x%8x.\n",
                    p_fls->name,p_fls->fl_addr,val);
            p_fls->fl_addr = *(u32 *)val;
            break;
        case MBR_REGION_SIZE:
            printf("\nModify %s MBR flash_size value, before val:0x%8x, setting val:0x%8x.\n",
                    p_fls->name,p_fls->fl_size,val);
            p_fls->fl_size = *(u32 *)val;
            break;

        case MBR_REGION_NAME:
            printf("\nModify %s MBR name,setting name:%s.\n",
                    p_fls->name,p_fls->flags,(u8 *)val);
            
            memcpy(p_fls->name,(u8 *)val,16);
            break;
        default:
            printf("\nModify region type can not recognise,please confirm it.\n\n");
            return -1;
            break;
    }

    
    crc16_m = crc16((void*)p_fls, sizeof(struct fls_fmt_data) - 2);  
    printf("Modify %s MBR crc16 value, pre-set val:0x%x, post-set val:0x%x.\n",
                    p_fls->name,p_fls->crc16,crc16_m);
    p_fls->crc16 = crc16_m;
    
    mbr_modify(index,p_fls);
    return 0;
}

int do_mbr_dump_cmd(u8 *mbr_dbuf,MBR_ITEM_E item)
{
    int i;
    struct fls_fmt_data *fls; 

    printf("%-16s%-12s%-12s%-10s\n","Name","FLASH Saddr","FLASH Eaddr","Length");

    if(MBR_ITEM_ALL == item)
    {
        fls = (struct fls_fmt_data *)mbr_dbuf;
       //need improve.now simply to dump 8 mbr info.
       //need check mbr valid.
       // dump_data(mbr_dbuf,256);
        
        for(i = 0; i < 128; i++)
        {   
            if(fls->name[0]== 0xff)
                return 0;
            printf("%-16s0x%08x  0x%08x  0x%8x\n",
                        fls->name,
                        fls->fl_addr,
                        fls->fl_addr + fls->fl_size,
                        fls->fl_size);
            fls ++;
            
        }

    }else     //need protect.
    {

        fls = ((struct fls_fmt_data *)(mbr_dbuf)) + item;
        printf("%-16s0x%08x  0x%08x  0x%8x\n",
                        fls->name,
                        fls->fl_addr,
                        fls->fl_addr + fls->fl_size,
                        fls->fl_size);
    }
    return 0;
}

static u32 mbr_get_setValue(const char *s)
{
    return (u32)(simple_strtoul(s, NULL, 16));
}

static MBR_REGION_E mbr_get_region_type(const char *s)
{
    MBR_REGION_E item;
    if(0 == strcmp(s,"addr"))
    {
        item = MBR_REGION_ADDR;
    }else if(0 == strcmp(s,"len"))
    {
        item = MBR_REGION_SIZE;
    }else if(0 == strcmp(s,"name"))
    {
        item = MBR_REGION_NAME;
    }else 
    {
        item = -1;
        printf("Error input part \"%s\", please check mbr cmd type!\n"
                ,s);
    }

    return item;
}

u32 mbr_get_index(const char *s)
{
    return (u32)(simple_strtoul(s, NULL, 16));
}


int mbr_get_cmd(u32 argc, const char **argv)
{
    u32 val;
    u8 name[16];
    u32 addr;
    u32 item;
    MBR_REGION_E region;

    if(0 == entry_init_ok)
    {
        if(0 > mbr_entry_init())
            return -1;
        entry_init_ok = 1;
    }

    if(argc < 2)
    {
       mbr_entry_dump(); 
        return 0;

    }
//    printf("init ok, valid cnt = %d\n",mbr_valid_cnt);
    if(0 == strcmp(argv[1],"del"))
    {
        item = mbr_get_index(argv[2]);
        if(item > mbr_valid_cnt)
        {
            printf("\nInvaild input index %d .\n",item);
            return -1;
        }

        return do_mbr_delete_cmd(item);
      
    }else if(0 == strcmp(argv[1],"modify"))
    {
        if(argc < 5)
        {
            printf("\nParameters not enough, please check cmd usage.\n\n");
            return -1;
        }

        item = mbr_get_index(argv[2]);
        if(0 > item)
            return -1;
        
        region = mbr_get_region_type(argv[3]);
        if(0 > region)
            return -1;
        
        if(MBR_REGION_NAME == region){
            if(16 < strlen(argv[4]))
            {
                printf("\nLenth of name is out of range,please double check.\n\n");
                return -1;
            }
            memset(name,0x00,16);
            memcpy(name,argv[4],strlen(argv[4]));
            return do_mbr_modify_cmd(item,region,name);
        }
        val = mbr_get_setValue(argv[4]);
        return do_mbr_modify_cmd(item,region,&val);
    }else if(0 == strcmp(argv[1],"add"))
    {
        if(argc < 5)
        {
            printf("\nParameters not enough, please check cmd usage.\n\n");
            return -1;
        }


        if(16 < strlen(argv[2]))
        {
            printf("\nLenth of name is out of range,please double check.");
            return -1;
        }
        
        memset(name,0x00,16);
        memcpy(name,argv[2],strlen(argv[2]));
        addr = (u32)(simple_strtoul(argv[3], NULL, 16));  //parse addr.
        val = (u32)(simple_strtoul(argv[4], NULL, 16));   //parse size.
        return do_mbr_add_cmd(name,addr,val);

    }else if(0 == strcmp(argv[1],"help"))
    {
        do_mbr_help(); 
    
    }else
        printf("\n***Error, enter \"mbr help\" to get help info.***\n");

    return 0;

}

static u32 mbr_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    
    mbr_get_cmd(argc, argv);
    return 0;
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: tool\n";
}

static u8 *long_description(void)
{
#if 0
    char buf[1024];
    memset(buf,0x0,1024);
    char * p_buf = buf;
    u32 ret;
    ret = sprintf(p_buf,"Description: mbr cmd for dump, modify, add, delete MBR.\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"Usage of mbr cmd:\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"  [1]show all mbr info.\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    example: dump all MBR info\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"             mbr\n\n");
    p_buf +=ret;
    
    ret = sprintf(p_buf,"  [2]modify select mbr .\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    mbr m [sel] [part] [value]\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"      [sel]:  index of mbr. \n");
    p_buf +=ret;
    ret = sprintf(p_buf,"      [part]: port of mbr you want to modify. valid: name, addr, len \n");
    p_buf +=ret;
    ret = sprintf(p_buf,"      [val:   value of the mbr you want to modify. \n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    example1: modify the third mbr  name to \"ap3fw\"\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"        mbr m 3 name ap3fw\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    example2: modify the second mbr addr to 0x21200\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"        mbr m 2 addr 0x21200\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    example3: modify the second mbr Length to 0x2100\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"        mbr m 2 len 0x2100\n\n");
    p_buf +=ret;
    
    ret = sprintf(p_buf,"  [3]add an new MBR.\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    mbr a [name] [addr] [len]\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"      [name]: name of the new MBR. \n");
    p_buf +=ret;
    ret = sprintf(p_buf,"      [addr]: start addr of the new MBR. \n");
    p_buf +=ret;
    ret = sprintf(p_buf,"      [len]:  len of the new MBR. \n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    example: add a new MBR .name: ap3fw Saddr:0x21200 Length:0x2100\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"        mbr a ap3fw 0x21200 0x2100\n\n");
    p_buf +=ret;

    ret = sprintf(p_buf,"  [4]delete an new MBR.\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    mbr d [sel]\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"      [sel]: index of the MBR which you want to delete. \n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    example: delete the second MBR.\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"        mbr d 2\n\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"  [5]get cmd help info.\n");
    p_buf +=ret;
    ret = sprintf(p_buf,"    mbr h\n\n");
    return (u8 *)p_buf;
#endif
//#if 0        
    return (u8 *) "Description: Dump and modify mbr info of platform\n\n";
//#endif
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &mbr_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "mbr", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = mbr_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
