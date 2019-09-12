#ifndef __SDIO_H_
#define __SDIO_H_

struct sdio_slot {
    volatile struct host_register_set *host_io;
    volatile struct slot_register_set *io;
    unsigned char id;
    unsigned char LowVoltage;
    unsigned char DeviceType;
    unsigned char inserted;
    unsigned char SupportedBusWidths, SpecVersNumb;
    unsigned char DeviceCapacity, IsSelected;
    unsigned short RCA;
    unsigned int cmd;
    unsigned int response[4];

    unsigned short blkcnt;
    unsigned short blksize;
};

#endif /* __SDIO_H_ */
