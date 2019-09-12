#ifndef __HG_TYPE_H__
#define __HG_TYPE_H__
typedef unsigned char           BOOL;
#ifndef bool
typedef unsigned char           bool;
#endif
typedef unsigned char		u8;
typedef unsigned short          u16;
typedef unsigned int		u32;
typedef char                    s8;
typedef short                   s16;
typedef int                     s32;
typedef unsigned long long      u64;
typedef long long               s64;


typedef unsigned char           U8;
typedef unsigned short          U16;
typedef unsigned int            U32;
typedef char                    S8;
typedef short                   S16;
typedef int                     S32;
typedef unsigned long long      U64;
typedef long long               S64;

typedef char                    INT8;
typedef short                   INT16;
typedef int                     INT32;

typedef volatile unsigned char      REG8;
typedef volatile unsigned short     REG16;
typedef volatile unsigned int       REG32;

typedef unsigned int                size_t;

#ifndef uint32_t
typedef unsigned int                uint32_t;
#endif

typedef unsigned long               UINT32;
typedef unsigned short              UINT16;
typedef unsigned char               UINT8;
typedef unsigned long long          UINT64;
typedef long long                   INT64;

#define false 0
#define true 1


#ifndef uintptr_t
typedef unsigned long int uintptr_t;
#endif

#define INT_MAX     ((int)(~0U>>1))
#define	TRUE	(1 == 1)
#define	FALSE	(0 == 1)

#ifndef NULL
#define NULL                        0
#endif 

#define PACKED      __attribute__((packed))
#define ALIGNED(a)  __attribute__((aligned(a)))


#endif // __HG_TYPE_H__

