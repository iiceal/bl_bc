#include <common.h>

#if 0
static uint	dp_last_addr, dp_last_size;
static uint	dp_last_length = 0x40;

static	ulong	base_address = 0;

#define MAX_LINE_LENGTH_BYTES (64)
#define DEFAULT_LINE_LENGTH_BYTES (16)
int print_buffer (ulong addr, void* data, uint width, uint count, uint linelen)
{
	/* linebuf as a union causes proper alignment */
	union linebuf {
#ifdef CONFIG_SYS_SUPPORT_64BIT_DATA
		uint64_t uq[MAX_LINE_LENGTH_BYTES/sizeof(uint64_t) + 1];
#endif
		uint32_t ui[MAX_LINE_LENGTH_BYTES/sizeof(uint32_t) + 1];
		uint16_t us[MAX_LINE_LENGTH_BYTES/sizeof(uint16_t) + 1];
		uint8_t  uc[MAX_LINE_LENGTH_BYTES/sizeof(uint8_t) + 1];
	} lb;
	int i;
#ifdef CONFIG_SYS_SUPPORT_64BIT_DATA
	uint64_t x;
#else
	uint32_t x;
#endif

	if (linelen*width > MAX_LINE_LENGTH_BYTES)
		linelen = MAX_LINE_LENGTH_BYTES / width;
	if (linelen < 1)
		linelen = DEFAULT_LINE_LENGTH_BYTES / width;

	while (count) {
		printf("%08lx:", addr);

		/* check for overflow condition */
		if (count < linelen)
			linelen = count;

		/* Copy from memory into linebuf and print hex values */
		for (i = 0; i < linelen; i++) {
			if (width == 4)
				x = lb.ui[i] = *(volatile uint32_t *)data;
#ifdef CONFIG_SYS_SUPPORT_64BIT_DATA
			else if (width == 8)
				x = lb.uq[i] = *(volatile uint64_t *)data;
#endif
			else if (width == 2)
				x = lb.us[i] = *(volatile uint16_t *)data;
			else
				x = lb.uc[i] = *(volatile uint8_t *)data;
#ifdef CONFIG_SYS_SUPPORT_64BIT_DATA
			printf(" %0*llx", width * 2, (long long)x);
#else
			printf(" %0*x", width * 2, x);
#endif
			data += width;
		}

		/* Print data in ASCII characters */
		for (i = 0; i < linelen * width; i++) {
			if (!isprint(lb.uc[i]) || lb.uc[i] >= 0x80)
				lb.uc[i] = '.';
		}
		lb.uc[i] = '\0';
		printf("    %s\n", lb.uc);

		/* update references */
		addr += linelen * width;
		count -= linelen;
	}

	return 0;
}

int cmd_get_data_size(const char* arg, int default_size)
{
        /* Check for a size specification .b, .w or .l.
         */
        int len = strlen(arg);
        if (len > 2 && arg[len-2] == '.') {
                switch(arg[len-1]) {
                case 'b':
                        return 1;
                case 'w':
                        return 2;
                case 'l':
                        return 4;
#ifdef CONFIG_SYS_SUPPORT_64BIT_DATA
				case 'q':
						return 8;
#endif
                case 's':
                        return -2;
                default:
                        return -1;
                }
        }
        return default_size;
}

/* Memory Display
 *
 * Syntax:
 *	md{.b, .w, .l} {addr} {len}
 */
#define DISP_LINE_LEN	16
int do_mem_md ( shell_cmd_t *cptr, const unsigned int argc, const char **argv)
{
	ulong	addr, length;
	int	size;
	int rc = 0;

	/* We use the last specified parameters, unless new ones are
	 * entered.
	 */
	addr = dp_last_addr;
	size = dp_last_size;
	length = dp_last_length;

	if (argc < 2)
		return -EINVAL;

	/* New command specified.  Check for a size specification.
	 * Defaults to long if no or incorrect specification.
	 */
	if ((size = cmd_get_data_size(argv[0], 4)) < 0)
		return -EINVAL;

	/* Address is specified since argc > 1
	 */
	addr = simple_strtoul(argv[1], NULL, 16);
	addr += base_address;

	/* If another parameter, it is the length to display.
	 * Length is the number of objects, not number of bytes.
	 */
	if (argc > 2)
		length = simple_strtoul(argv[2], NULL, 16);

	/* Print the lines. */
	print_buffer(addr, (void*)addr, size, length, DISP_LINE_LEN/size);
	addr += size*length;

	dp_last_addr = addr;
	dp_last_length = length;
	dp_last_size = size;
	return (rc);
}

int do_mem_mw (shell_cmd_t *cptr, const unsigned int argc, const char **argv)
{
#ifdef CONFIG_SYS_SUPPORT_64BIT_DATA
	u64 writeval;
#else
	ulong writeval;
#endif
	ulong	addr, count;
	int	size;

	if ((argc < 3) || (argc > 4))
		return -EINVAL;

	/* Check for size specification.
	*/
	if ((size = cmd_get_data_size(argv[0], 4)) < 1)
		return -EINVAL;

	/* Address is specified since argc > 1
	*/
	addr = simple_strtoul(argv[1], NULL, 16);
	addr += base_address;

	/* Get the value to write.
	*/
#ifdef CONFIG_SYS_SUPPORT_64BIT_DATA
	writeval = simple_strtoull(argv[2], NULL, 16);
#else
	writeval = simple_strtoul(argv[2], NULL, 16);
#endif

	/* Count ? */
	if (argc == 4) {
		count = simple_strtoul(argv[3], NULL, 16);
	} else {
		count = 1;
	}

	while (count-- > 0) {
		if (size == 4)
			*((u32  *)addr) = (u32)writeval;
#ifdef CONFIG_SYS_SUPPORT_64BIT_DATA
		else if (size == 8)
			*((u64 *)addr) = (u64)writeval;
#endif
		else if (size == 2)
			*((ushort *)addr) = (ushort)writeval;
		else
			*((u_char *)addr) = (u_char)writeval;
		addr += size;
	}
	return 0;
}

#ifdef CONFIG_SYS_MALLOC_DEBUG
int do_mem_stats (shell_cmd_t *cptr, const unsigned int argc, const char **argv)
{
    malloc_stats();
    return 0;
}
#endif

void register_common_cmd(DYN_SHELL_ITEM_T* p_common_shell)
{
	BL_SHELL_ADD_CMD(p_common_shell,
			"mw",0,
			"mw             [.b/.w/.h] [addr] [value] [length]",
			"mw             [.b/.w/.h] [addr] [value] [length]",
			do_mem_mw); 
	BL_SHELL_ADD_CMD(p_common_shell,
			"md",0,
			"md             [.b/./w/.h] [addr] [length]",
			"md             [.b/./w/.h] [addr] [length]",
			do_mem_md); 
#ifdef CONFIG_SYS_MALLOC_DEBUG
	BL_SHELL_ADD_CMD(p_common_shell,
			"mst",0,
			"mst        [Display malloc stats]",
			"mst        [Display malloc stats]",
			do_mem_stats); 
#endif
}
#endif
