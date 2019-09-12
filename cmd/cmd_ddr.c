#include <common.h>
#include <drivers/drv_ddr.h>

typedef volatile unsigned long vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char vu_char;

static BL_CMD_T ddr_descriptor;
static u32 rdbuf[16];

/*
 * Perform a memory test. A more complete alternative test can be
 * configured using CONFIG_SYS_ALT_MEMTEST. The complete test loops until
 * interrupted by ctrl-c or by a failure of one of the sub-tests.
 */

u32 ddr_get_valid_addr(u32 s, u32 e)
{
    u32 addr = 0;

    while (1) {
        addr = hg_pseudo_random_sequence();
        addr &= ~3;
        if (addr >= s && addr <= e - 4)
            return addr;
    }
    return 0;
}

u32 ddr_get_random_data(void)
{
    return hg_pseudo_random_sequence();
}

u8 ddr_get_random_cmd(void)
{
    return hg_pseudo_random_sequence() & 0x1;
}

u32 do_mem_random(u32 argc, const char **argv)
{
    u32 start, end, addr, data, tmp, errs = 0, i;
    u32 total_data_len = 0;
    u8 cmd, burst_len;
    if (argc > 1)
        start = (u32) simple_strtoul(argv[1], NULL, 16);
    else
        start = (u32) CONFIG_DRAM_START;

    if (argc > 2)
        end = (u32) simple_strtoul(argv[2], NULL, 16);
    else
        end = (u32)(CONFIG_DRAM_START + 0x10000000);

    printf("Testing %08x ... %08x\n", start, end);
    while (1) {
        if (ctrlc())
            return errs;
        cmd = ddr_get_random_cmd();
        switch (cmd) {
        case 0: //
            total_data_len++;
            data = ddr_get_random_data();
            addr = ddr_get_valid_addr(start, end - 4);
            REG_WRITE_UINT32(addr, data);
            tmp = REG_READ_UINT32(addr);
            if (tmp != data) {
                errs++;
                printf("Mem error 0x%x@ 0x%08x: found %08x, expected %08x\n", errs, addr, tmp,
                       data);
            }
            break;
        case 1: // burst
            burst_len = 10;
            i = 0;
            data = ddr_get_random_data();
            addr = ddr_get_valid_addr(start, end - burst_len * 4);
            ddr_burst_write(addr, burst_len, data);
            ddr_burst_read(addr, burst_len, rdbuf);
            for (i = 0; i < burst_len; i++) {
                if (rdbuf[i] != data) {
                    errs++;
                    printf("Mem error 0x%x@ 0x%08x: found %08x, expected %08x\n", errs,
                           addr + i * 4, rdbuf[i], data);
                }
            }
            total_data_len += burst_len;
            break;
        }
        if ((total_data_len > 0) && ((total_data_len % 262144) == 0))
            printf("Memory Testing %d MBytes\n", total_data_len / 1024 / 256);
    }
    return errs;
}

int do_mem_alt_mtest(int argc, const char **argv)
{
    u_long *addr, *start, *end;
    ulong val;
    ulong readback;
    ulong errs = 0;
    int iterations = 1;
    int iteration_limit;

    u_long len;
    u_long offset;
    u_long test_offset;
    u_long pattern;
    u_long temp;
    u_long anti_pattern;
    vu_long num_words;
    int j;

    static const ulong bitpattern[] = {
        0x00000001, /* single bit */
        0x00000003, /* two adjacent bits */
        0x00000007, /* three adjacent bits */
        0x0000000F, /* four adjacent bits */
        0x00000005, /* two non-adjacent bits */
        0x00000015, /* three non-adjacent bits */
        0x00000055, /* four non-adjacent bits */
        0xaaaaaaaa, /* alternating 1/0 */
    };

    if (argc > 1)
        start = (ulong *) simple_strtoul(argv[1], NULL, 16);
    else
        start = (ulong *) CONFIG_DRAM_START;

    if (argc > 2)
        end = (ulong *) simple_strtoul(argv[2], NULL, 16);
    else
        end = (ulong *) (CONFIG_DRAM_START + 0x10000000);

    if (argc > 3)
        pattern = (ulong) simple_strtoul(argv[3], NULL, 16);
    else
        pattern = 0;

    if (argc > 4)
        iteration_limit = (ulong) simple_strtoul(argv[4], NULL, 16);
    else
        iteration_limit = 0;

    printf("Testing %08x ... %08x:\n", (uint)(ulong) start, (uint)(ulong) end);

    for (;;) {

        if (iteration_limit && iterations > iteration_limit) {
            printf("Tested %d iteration(s) with %lu errors.\n", iterations - 1, errs);
            return errs;
        }
        if (ctrlc())
            return errs;

        printf("Iteration: %8d\r", iterations);
        iterations++;

        /*
         * Data line test: write a pattern to the first
         * location, write the 1's complement to a 'parking'
         * address (changes the state of the data bus so a
         * floating bus doen't give a false OK), and then
         * read the value back. Note that we read it back
         * into a variable because the next time we read it,
         * it might be right (been there, tough to explain to
         * the quality guys why it prints a failure when the
         * "is" and "should be" are obviously the same in the
         * error message).
         *
         * Rather than exhaustively testing, we test some
         * patterns by shifting '1' bits through a field of
         * '0's and '0' bits through a field of '1's (i.e.
         * pattern and ~pattern).
         */
        addr = start;
        puts("Data line testing ...");
        for (j = 0; j < sizeof(bitpattern) / sizeof(bitpattern[0]); j++) {
            val = bitpattern[j];
            for (; val != 0; val <<= 1) {
                *addr = val;
                readback = *addr;
                if (readback != val) {
                    printf("FAILURE (data line): "
                           "expected %08lx, actual %08lx\n",
                           val, readback);
                    errs++;
                }
                *addr = ~val;
                readback = *addr;
                if (readback != ~val) {
                    printf("FAILURE (data line): "
                           "Is %08lx, should be %08lx\n",
                           readback, ~val);
                    errs++;
                }
                if (ctrlc())
                    return errs;
            }
        }
        puts(" Pass ");

        /*
         * Based on code whose Original Author and Copyright
         * information follows: Copyright (c) 1998 by Michael
         * Barr. This software is placed into the public
         * domain and may be used for any purpose. However,
         * this notice must not be changed or removed and no
         * warranty is either expressed or implied by its
         * publication or distribution.
         */

        /*
         * Address line test
         *
         * Description: Test the address bus wiring in a
         *              memory region by performing a walking
         *              1's test on the relevant bits of the
         *              address and checking for aliasing.
         *              This test will find single-bit
         *              address failures such as stuck -high,
         *              stuck-low, and shorted pins. The base
         *              address and size of the region are
         *              selected by the caller.
         *
         * Notes:	For best results, the selected base
         *              address should have enough LSB 0's to
         *              guarantee single address bit changes.
         *              For example, to test a 64-Kbyte
         *              region, select a base address on a
         *              64-Kbyte boundary. Also, select the
         *              region size as a power-of-two if at
         *              all possible.
         *
         * Returns:     0 if the test succeeds, 1 if the test fails.
         */

        puts("Address line testing ...");
        len = ((ulong) end - (ulong) start) / sizeof(vu_long);
        pattern = (vu_long) 0xaaaaaaaa;
        anti_pattern = (vu_long) 0x55555555;

        /*
         * Write the default pattern at each of the
         * power-of-two offsets.
         */
        for (offset = 1; offset < len; offset <<= 1) {
            start[offset] = pattern;
            if (ctrlc())
                return errs;
        }

        /*
         * Check for address bits stuck high.
         */
        test_offset = 0;
        start[test_offset] = anti_pattern;

        for (offset = 1; offset < len; offset <<= 1) {
            temp = start[offset];
            if (temp != pattern) {
                printf("\nFAILURE: Address bit stuck high @ 0x%.8lx:"
                       " expected 0x%.8lx, actual 0x%.8lx\n",
                       (ulong) &start[offset], pattern, temp);
                errs++;
            }
            if (ctrlc())
                return errs;
        }
        start[test_offset] = pattern;

        /*
         * Check for addr bits stuck low or shorted.
         */
        for (test_offset = 1; test_offset < len; test_offset <<= 1) {
            start[test_offset] = anti_pattern;

            for (offset = 1; offset < len; offset <<= 1) {
                temp = start[offset];
                if ((temp != pattern) && (offset != test_offset)) {
                    printf("\nFAILURE: Address bit stuck low or shorted @"
                           " 0x%.8lx: expected 0x%.8lx, actual 0x%.8lx\n",
                           (ulong) &start[offset], pattern, temp);
                    errs++;
                }
                if (ctrlc())
                    return errs;
            }
            start[test_offset] = pattern;
        }

        puts(" Pass ");
        /*
         * Description: Test the integrity of a physical
         *		memory device by performing an
         *		increment/decrement test over the
         *		entire region. In the process every
         *		storage bit in the device is tested
         *		as a zero and a one. The base address
         *		and the size of the region are
         *		selected by the caller.
         *
         * Returns:     0 if the test succeeds, 1 if the test fails.
         */
        num_words = ((ulong) end - (ulong) start) / sizeof(vu_long) + 1;

        /*
         * Fill memory with a known pattern.
         */
        for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
            start[offset] = pattern;
            if (ctrlc())
                return errs;
        }

        /*
         * Check each location and invert it for the second pass.
         */
        for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
            temp = start[offset];
            if (temp != pattern) {
                printf("\nFAILURE (read/write) @ 0x%.8lx:"
                       " expected 0x%.8lx, actual 0x%.8lx)\n",
                       (ulong) &start[offset], pattern, temp);
                errs++;
            }
            if (ctrlc())
                return errs;

            anti_pattern = ~pattern;
            start[offset] = anti_pattern;
        }

        /*
         * Check each location for the inverted pattern and zero it.
         */
        for (pattern = 1, offset = 0; offset < num_words; pattern++, offset++) {
            anti_pattern = ~pattern;
            temp = start[offset];
            if (temp != anti_pattern) {
                printf("\nFAILURE (read/write): @ 0x%.8lx:"
                       " expected 0x%.8lx, actual 0x%.8lx)\n",
                       (ulong) &start[offset], anti_pattern, temp);
                errs++;
            }
            if (ctrlc())
                return errs;

            start[offset] = 0;
        }
        printf("errs = %d\n", errs);
    }
}

int do_mem_mtest(int argc, const char **argv)
{
    u_long *addr, *start, *end;
    ulong val;
    ulong readback;
    ulong errs = 0;
    int iterations = 1;
    int iteration_limit;
    ulong incr;
    ulong pattern;

    if (argc > 1)
        start = (ulong *) simple_strtoul(argv[1], NULL, 16);
    else
        start = (ulong *) CONFIG_DRAM_START;

    if (argc > 2)
        end = (ulong *) simple_strtoul(argv[2], NULL, 16);
    else
        end = (ulong *) (CONFIG_DRAM_START + 0x10000000);

    if (argc > 3)
        pattern = (ulong) simple_strtoul(argv[3], NULL, 16);
    else
        pattern = 0;

    if (argc > 4)
        iteration_limit = (ulong) simple_strtoul(argv[4], NULL, 16);
    else
        iteration_limit = 0;

    incr = 1;
    printf("address[%08x-%08x], pattern = %x,incr: %d \n", (u32)(ulong) start, (u32)(ulong) end,
           pattern, incr);

    for (;;) {
        if (iteration_limit && iterations > iteration_limit) {
            printf("Tested %d iteration(s) with %lu errors.\n", iterations - 1, errs);
            return errs;
        }
        ++iterations;

        printf("\rPattern %08lX  Writing..."
               "%12s"
               "\b\b\b\b\b\b\b\b\b\b",
               pattern, "");

        for (addr = start, val = pattern; addr < end; addr++) {
            *addr = val;
            val += incr;
            if (ctrlc())
                return errs;
        }

        puts("Reading...");

        for (addr = start, val = pattern; addr < end; addr++) {
            readback = *addr;
            if (readback != val) {
                printf("\nMem error @ 0x%08lx: "
                       "found %08lx, expected %08lx\n",
                       (ulong) addr, readback, val);
                errs++;
            }
            val += incr;
            if (ctrlc())
                return errs;
        }

        printf(" Incr: %d, Errs = %d\n", incr, errs);
        /*
         * Flip the pattern each time to make lots of zeros and
         * then, the next time, lots of ones.  We decrement
         * the "negative" patterns and increment the "positive"
         * patterns to preserve this feature.
         */
        if (pattern & CONFIG_DRAM_START) {
            pattern = -pattern; /* complement & increment */
        } else {
            pattern = ~pattern;
        }
        incr = -incr;
        if (ctrlc())
            return errs;
    }
    printf("mtest end\n");
    return 0; /* not reached */
}

static u32 ddr_cmd_entry_func(BL_CNXT_T *pCnxt, u32 argc, const char **argv)
{
    u32 ret = -1;

    if(argc <= 1) { 
        printf("Need >=2 paramter\n");
        return RET_FAIL;
     }
     
    argc--;
    argv++;

    switch(argv[0][0]) {
        case 'i':
            printf("Init DDR\n");
            if(strstr(argv[1], "ddr3") != NULL) {
                printf("ddr3 init ");
                ret = ddr3_init();
            } else if(strstr(argv[1], "lpddr2") != NULL) {
                printf("lpddr2 init ");
                ret = lpddr2_init();
            } else if (strstr(argv[1], "lpddr") != NULL) {
                printf("lpddr init ");
                ret = lpddr_init();
            } else if (strstr(argv[1], "fpga") != NULL) {
                printf("fpga-ddr init ");
                ret = ddr_init();
            }
            printf("Done\n");
            break;
        case 'l':
            ret = do_mem_alt_mtest(argc, argv);
            break;
        case 'm':
            ret = do_mem_mtest(argc, argv);
            break;
        case 's':
            ret = do_mem_random(argc, argv);
            break;
        default:
            return RET_FAIL;
    }

    if(ret != 0)
        return RET_FAIL;

    return RET_PASS;
}

static u8 *short_description(void)
{
    return (u8 *) "Case list: ddr mtest/burst operation etc\n";
}

static u8 *long_description(void)
{
    return (u8 *) "Description: The applicaition running command is as follow:\n\
                ddr i [ddr3/lpddr/lpddr2/fpga]: init ddr\n\
                ddr l start_addr end_addr patten: for addr line and data line testing\n\
                ddr m start_addr end_addr patten: for mtest\n\
                ddr s :auto testing \n";
}

static BL_CMD_T *static_cmd_descriptor(BL_CNXT_T *pCnxt)
{
    BL_CMD_T *p_descriptor = &ddr_descriptor;
    memset(p_descriptor, 0, sizeof(BL_CMD_T));

    memcpy(&p_descriptor->name[0], "ddr", BL_CMD_SHORT_NAME_LENGTH);

    p_descriptor->cmd_entry_func = ddr_cmd_entry_func;

    p_descriptor->cmd_description = short_description;
    p_descriptor->cmd_long_description = long_description;

    return p_descriptor;
}

DECLARE_BL_CMD(static_cmd_descriptor);
