#ifndef TEST_QSPI__H__
#define TEST_QSPI__H__

typedef enum {
    QSPI_TEST_INIT,
    QSPI_TEST_RESET,
    QSPI_TEST_READ_ID,

    QSPI_TEST_READ,
    QSPI_TEST_READ_FAST,
    QSPI_TEST_READ_DUAL,
    QSPI_TEST_READ_QUAD,
    QSPI_TEST_READ_DUAL_IO,
    QSPI_TEST_READ_QUAD_IO,

    QSPI_TEST_AHB_READ,
    QSPI_TEST_AHB_READ_FAST,
    QSPI_TEST_AHB_READ_DUAL,
    QSPI_TEST_AHB_READ_QUAD,
    QSPI_TEST_AHB_READ_DUAL_IO,
    QSPI_TEST_AHB_READ_QUAD_IO,

    QSPI_TEST_DMA_READ,
    QSPI_TEST_DMA_READ_FAST,
    QSPI_TEST_DMA_READ_DUAL,
    QSPI_TEST_DMA_READ_QUAD,
    QSPI_TEST_DMA_READ_DUAL_IO,
    QSPI_TEST_DMA_READ_QUAD_IO,

    QSPI_TEST_PROGRAM,
    QSPI_TEST_DMA_PROGRAM,
    QSPI_TEST_AHB_PROGRAM,

    QSPI_TEST_ERASE_SECT,
    QSPI_TEST_ERASE_ALL,

    QSPI_TEST_PROTECT_SECT,
    QSPI_TEST_UPGRADE_FLASH,

    QSPI_TEST_UPGRADE_VERSION,

    QSPI_TEST_AUTO,
   
    QSPI_TEST_BLOCK_LOCK,
    QSPI_TEST_BLOCK_UNLOCK,
    QSPI_GET_LOCK_STATUS,
    QSPI_GLOBAL_LOCK,
    QSPI_GLOBAL_UNLOCK,

    QSPI_TEST_POWER_DOWN,

    QSPI_TEST_ALL,
    QSPI_PERF,

    QSPI_TEST_UNKNOWN = -1,
} QSPI_TEST_TYPE_T;

#endif
