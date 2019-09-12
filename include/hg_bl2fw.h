#ifndef HG_BL2FW_H__
#define	HG_BL2FW_H__

struct bl2fw_hdr{
	u32 	magic;
	u32 	version;

	u32 	flash_addr;
	u32 	size;
	u32 	load_addr;
	u32 	exec_addr;

	u8	    data_en_key[256];
	u16  	flags;
	u16  	crc16;
};

struct bl2fw_plain_data_suf {
	u32 	chipid;
	u32 	crc32;
};

#define	BL2FW_HDR_SIZE	512
#define	BL_MAGIC	0x64616f6c		/*load*/
#define FW_MAGIC	0x77667766		/*fwfw*/

#define	BL2FW_FLAGS_BL		    (1)
#define	BL2FW_FLAGS_BB_FW	    (2)
#define	BL2FW_FLAGS_AP_FW 	    (3)
#define	BL2FW_FLAGS_AP_DES_EN 	    (1 << 3)
#define BL2FW_FLAGS_AP_FW_LINUX     (1 << 4)
#define BL2FW_FLAGS_SE_DIS          (1 << 7)

#define BL2FW_FLAGS_TYPE_INDEX(n)      ((n & 0x7) << 0)                                         
#define BL2FW_FLAGS_TYPE_INDEX_MASK    (0x7 << 0)                                               
#endif
