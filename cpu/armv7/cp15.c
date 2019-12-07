#include <common.h>


unsigned long ttb_addr = CONFIG_TTB_BASE;
static BOOL mmu_enable_flag = false;
/*      
 * CR1 bits (CP#15 CR1)
 */
#define CR_M    (1 << 0)        /* MMU enable                           */
#define CR_A    (1 << 1)        /* Alignment abort enable               */
#define CR_C    (1 << 2)        /* Dcache enable                        */
#define CR_W    (1 << 3)        /* Write buffer enable                  */
#define CR_P    (1 << 4)        /* 32-bit exception handler             */
#define CR_D    (1 << 5)        /* 32-bit data address range            */
#define CR_L    (1 << 6)        /* Implementation defined               */
#define CR_B    (1 << 7)        /* Big endian                           */
#define CR_S    (1 << 8)        /* System MMU protection                */
#define CR_R    (1 << 9)        /* ROM MMU protection                   */
#define CR_F    (1 << 10)       /* Implementation defined               */
#define CR_Z    (1 << 11)       /* Implementation defined               */
#define CR_I    (1 << 12)       /* Icache enable                        */
#define CR_V    (1 << 13)       /* Vectors relocated to 0xffff0000      */
#define CR_RR   (1 << 14)       /* Round Robin cache replacement        */
#define CR_L4   (1 << 15)       /* LDR pc can set T bit                 */
#define CR_DT   (1 << 16)
#define CR_IT   (1 << 18)
#define CR_ST   (1 << 19)
#define CR_FI   (1 << 21)       /* Fast interrupt (lower latency mode)  */
#define CR_U    (1 << 22)       /* Unaligned access operation           */
#define CR_XP   (1 << 23)       /* Extended page tables                 */
#define CR_VE   (1 << 24)       /* Vectored interrupts                  */
#define CR_EE   (1 << 25)       /* Exception (Big) Endian               */
#define CR_TRE  (1 << 28)       /* TEX remap enable                     */
#define CR_AFE  (1 << 29)       /* Access flag enable                   */
#define CR_TE   (1 << 30)       /* Thumb exception enable               */

enum dcache_option {
    DCACHE_OFF = 0x12,
    DCACHE_WRITETHROUGH = 0x1a,
    DCACHE_WRITEBACK = 0x1e,
    DCACHE_WRITEALLOC = 0x16,
};

/* Size of an MMU section */
enum {
    MMU_SECTION_SHIFT = 20,
    MMU_SECTION_SIZE = 1 << MMU_SECTION_SHIFT,
};

static inline unsigned int get_dfsr(void)
{
    unsigned int val;
    asm("mrc p15, 0, %0, c5, c0, 0  @ get dfsr" : "=r"(val) : : "cc");
    return val;
}

static inline unsigned int get_actl(void)
{
    unsigned int val;
    asm("mrc p15, 0, %0, c1, c0, 1  @ get actl" : "=r"(val) : : "cc");
    return val;
}

static inline unsigned int get_ttbr0(void)
{
    unsigned int val;
    asm("mrc p15, 0, %0, c2, c0, 0  @ get ttbr0" : "=r"(val) : : "cc");
    return val;
}

static inline unsigned int get_cr(void)
{
    unsigned int val;
    asm("mrc p15, 0, %0, c1, c0, 0  @ get CR" : "=r"(val) : : "cc");
    return val;
}

static inline unsigned int get_scr(void)
{
    unsigned int val;
    asm("mrc p15, 0, %0, c1, c1, 0  @ get SCR" : "=r"(val) : : "cc");
    return val;
}

#define isb() ({ asm volatile("isb" : : : "memory"); })

static inline void set_cr(unsigned int val)
{
    asm volatile("mcr p15, 0, %0, c1, c0, 0 @ set CR" : : "r"(val) : "cc");
    isb();
}

void set_section_dcache(int section, enum dcache_option option)
{
    unsigned int *page_table = (unsigned int *) ttb_addr;
    unsigned int value;

    value = (section << MMU_SECTION_SHIFT) | (3 << 10);
    value |= option;
    page_table[section] = value;
}

void modify_section_dcache(int va, int pa, enum dcache_option option)
{
    unsigned int *page_table = (unsigned int *) ttb_addr;
    unsigned int value;

    value = (pa << MMU_SECTION_SHIFT) | (3 << 10);
    value |= option;
    page_table[va] = value;
}

void dram_bank_mmu_setup(void)
{
    unsigned int start = CONFIG_DRAM_START;
    unsigned int size = CONFIG_DRAM_SIZE;
    int i;

    for (i = start >> 20; i < (start >> 20) + (size >> 20); i++) {
        set_section_dcache(i, DCACHE_WRITEBACK);
    }
}

void iram_mmu_setup(void)
{
    unsigned int start = CONFIG_SRAM_START;
    unsigned int size = CONFIG_SRAM_SIZE;
    int i;
    int section_start = start >> 20;
    int section_num = (start >> 20) + ((size + 0xFFFFF) >> 20);

    for (i = section_start; i < section_num; i++) {
        set_section_dcache(i, DCACHE_WRITEBACK);
    }
}

void bb_iram_mmu_setup(void)
{
    unsigned int start = CONFIG_BB_MEM_START;
    unsigned int size = CONFIG_BB_MEM_SIZE;
    int i;
    int section_start = start >> 20;
    int section_num = (start >> 20) + ((size + 0xFFFFF) >> 20);

    for (i = section_start; i < section_num; i++) {
        set_section_dcache(i, DCACHE_WRITEBACK);
    }
}

BOOL is_mmu_enable(void)
{
    return mmu_enable_flag;
}

#ifdef MMU_DEBUG
#define mmu_debug(fmt, args...)  printf("MMU: " fmt "\n", ##args);
#else
#define mmu_debug(fmt, args...)
#endif


void mmu_setup(void)
{
    int i;
    unsigned int reg;

    reg = get_cr(); /* get control reg. */
    if (reg & CR_M) {
        mmu_debug("mmu had configed completed\n");
        return;
    }

    mmu_debug("mmu setup\n");
    /* Set up an identity-mapping for all 4GB, rw for everyone */
    for (i = 0; i < 4096; i++)
        set_section_dcache(i, DCACHE_OFF);

    dram_bank_mmu_setup();
    iram_mmu_setup();

#ifdef CONFIG_BB_MMU_ENABLE
    bb_iram_mmu_setup();
#endif
    set_section_dcache(0, DCACHE_WRITEBACK);
    modify_section_dcache(3, 2, DCACHE_OFF);
    modify_section_dcache(4, 2, DCACHE_WRITEBACK);
    modify_section_dcache(5, 2, DCACHE_OFF);

    /* Copy the page table address to cp15 */
    asm volatile("mcr p15, 0, %0, c2, c0, 0" : : "r"(ttb_addr) : "memory");
    /* Set the access control to all-supervisor */
    asm volatile("mcr p15, 0, %0, c3, c0, 0" : : "r"(~0));

    /* and enable the mmu */
    reg = get_cr(); /* get control reg. */
    set_cr(reg | CR_M | CR_C | CR_I | CR_Z);

    mmu_enable_flag = true;
    mmu_debug("mmu setup exit.., VA_3M = PA_2M\n");
}
