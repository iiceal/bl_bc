#include "common.h"
#include "sdio.h"

extern int sd_init(void);
int read_card_status(void);
int read_SD_status(void);
int read_sd_scr(void);
int read_sd_csd(void);
int read_sd_cid(void);
int send_sd_command(int cmd, const int argument);

int sd_read(const int addr, const int count);
int sd_write(const int addr, const int count);
int sd_erase(const int addr, const int count);
int sd_dma_program(int address, char *buf, int count, int dir);
int sd_dma_transfer(int address, char *buf, int count, int dir);
int sd_adma_write(const int addr, const int count);
int sd_adma_read(const int addr, const int count);

int sd_cmd_entry_funcing(u32 argc, const char **argv)
{
    int addr = 0x80000;
    int count = 0x40;
    static int init_flag = 0;

    if (argc < 2)
        return (-1);

    if (argc > 2) {
        addr = simple_strtoul(argv[2], NULL, 16);
    }
    if (argc > 3) {
        count = simple_strtoul(argv[3], NULL, 16);
        if (count > 0x100)
            count = 0x100;
    }

    if (strcmp(argv[1], "init") == 0) {
        sd_init();
    }

    if (strcmp(argv[1], "st") == 0) {
        read_card_status();
    }

    if (strcmp(argv[1], "sdst") == 0) {
        read_SD_status();
    }

    if (strcmp(argv[1], "scr") == 0) {
        read_sd_scr();
    }

    if (strcmp(argv[1], "csd") == 0) {
        read_sd_csd();
    }

    if (strcmp(argv[1], "cid") == 0) {
        read_sd_cid();
    }

    if (strcmp(argv[1], "cmd") == 0) {
        send_sd_command(addr, count);
    }

    if (strcmp(argv[1], "erase") == 0) {
        sd_erase(addr, count);
    }

    if (strcmp(argv[1], "write") == 0) {
        sd_write(addr, count);
    }

    if (strcmp(argv[1], "read") == 0) {
        sd_read(addr, count);
    }

    if (strcmp(argv[1], "sdmaw") == 0) {
        sd_dma_program(addr, 0, count, 0);
    }

    if (strcmp(argv[1], "sdmar") == 0) {
        sd_dma_transfer(addr, 0, count, 0);
    }

    if (strcmp(argv[1], "dmaw") == 0) {
        sd_adma_write(addr, count);
    }

    if (strcmp(argv[1], "dmar") == 0) {
        sd_adma_read(addr, count);
    }

    if (strcmp(argv[1], "auto") == 0) {
        if (init_flag == 0) {
            sd_init();
            init_flag = 1;
        }
        sd_write(addr, count);
        sd_read(addr, count);
    }

    if (strcmp(argv[1], "dmaauto") == 0) {
        if (init_flag == 0) {
            sd_init();
            init_flag = 1;
        }
        sd_adma_write(addr, count);
        sd_adma_read(addr, count);
    }

    if (strcmp(argv[1], "sdmaauto") == 0) {
        if (init_flag == 0) {
            sd_init();
            init_flag = 1;
        }
        sd_dma_program(addr, 0, count, 0);
        sd_dma_transfer(addr, 0, count, 0);
    }

    return 0;
}
