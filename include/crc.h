#ifndef __CRC_20180706_H__
#define __CRC_20180706_H__

unsigned int crc32_no_comp(unsigned int crc, unsigned char const *p, size_t len);

unsigned int crc32 (unsigned int crc, const unsigned char *p, unsigned int len);

u16 crc16(u8 *buf, int len);

#endif //__CRC_20180706_H__
