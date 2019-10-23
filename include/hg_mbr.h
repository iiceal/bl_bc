#ifndef HG_MBR_H__
#define	HG_MBR_H__

struct fls_fmt_data {
	char 	name[16];
	u32 	fl_addr;
	u32 	fl_size;
	u32 	resvered;
	u16  	flags;
	u16  	crc16;
};

/*flags*/
#define		FLS_FLAGS_VALID		(3 << 0)

#endif
