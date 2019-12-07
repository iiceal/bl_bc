#include <common.h>

#ifdef CONFIG_LOAD_XMODEM
static struct tty serial_tty = {
    .tstc = serial_tstc,
    .putc = serial_putc,
    .puts = serial_puts,
    .getc = serial_getc,
};

int load_xmodem(unsigned int load_addr, int load_size)
{
    int ret;
    int len;

    mdelay(1);
    printf("Loading image from uart xModem ...\n");
    xyzModem_init(&serial_tty);
    if (xyzModem_stream_open(&ret)!=0) {
        printf("open error %d\n",-ret);
        return ret;
    }

    len = xyzModem_stream_read((char *)load_addr, load_size, &ret);
    xyzModem_stream_close (&ret);
    xyzModem_stream_terminate (0, &getcxmodem);
    printf("read %d bytes Done\n", len);
    return len;
}

#endif //CONFIG_LOAD_XMODEM 
