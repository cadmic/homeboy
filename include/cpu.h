#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>
#include "version.h"

// os.h
#define OS_CACHED_REGION_PREFIX 0x8000
#define OS_BASE_CACHED (OS_CACHED_REGION_PREFIX << 16)

// OSAddress.h
static inline void* OSPhysicalToCached(uint32_t ofs) { return (void*)(ofs + OS_BASE_CACHED); }

inline uint64_t gettick() {
    register uint32_t tbu;
    register uint32_t tbl;
        __asm__ __volatile__ (
            "mftbl %0\n"
            "mftbu %1\n"
            ::
            "r"(tbl), "r"(tbu));
    return (uint64_t)((uint64_t)tbu << 32 | tbl);
}

// cpu.h
#if VC_VERSION == NACE
#define DEVICE_ADDRESS_INDEX_BITS 20
#else
#define DEVICE_ADDRESS_INDEX_BITS 16
#endif

#define DEVICE_ADDRESS_OFFSET_BITS (32 - DEVICE_ADDRESS_INDEX_BITS)

typedef struct CpuBlock CpuBlock;
typedef int (*UnknownBlockCallback)(CpuBlock* pBlock, int bUnknown);

struct CpuBlock {
    /* 0x00 */ struct CpuBlock* pNext;
    /* 0x04 */ uint32_t nSize;
    /* 0x08 */ UnknownBlockCallback pfUnknown;
    /* 0x0C */ uint32_t nAddress0;
    /* 0x10 */ uint32_t nAddress1;
}; // size = 0x14

typedef int (*GetBlockFunc)(void* pObject, CpuBlock* pBlock);

typedef int (*Put8Func)(void* pObject, uint32_t nAddress, int8_t* pData);
typedef int (*Put16Func)(void* pObject, uint32_t nAddress, int16_t* pData);
typedef int (*Put32Func)(void* pObject, uint32_t nAddress, int32_t* pData);
typedef int (*Put64Func)(void* pObject, uint32_t nAddress, int64_t* pData);

typedef int (*Get8Func)(void* pObject, uint32_t nAddress, int8_t* pData);
typedef int (*Get16Func)(void* pObject, uint32_t nAddress, int16_t* pData);
typedef int (*Get32Func)(void* pObject, uint32_t nAddress, int32_t* pData);
typedef int (*Get64Func)(void* pObject, uint32_t nAddress, int64_t* pData);

typedef enum CpuExceptionCode {
    CEC_NONE = -1,
    CEC_INTERRUPT = 0,
    CEC_TLB_MODIFICATION = 1,
    CEC_TLB_LOAD = 2,
    CEC_TLB_STORE = 3,
    CEC_ADDRESS_LOAD = 4,
    CEC_ADDRESS_STORE = 5,
    CEC_BUS_INSTRUCTION = 6,
    CEC_BUS_DATA = 7,
    CEC_SYSCALL = 8,
    CEC_BREAK = 9,
    CEC_RESERVED = 10,
    CEC_COPROCESSOR = 11,
    CEC_OVERFLOW = 12,
    CEC_TRAP = 13,
    CEC_VCE_INSTRUCTION = 14,
    CEC_FLOAT = 15,
    CEC_RESERVED_16 = 16,
    CEC_RESERVED_17 = 17,
    CEC_RESERVED_18 = 18,
    CEC_RESERVED_19 = 19,
    CEC_RESERVED_20 = 20,
    CEC_RESERVED_21 = 21,
    CEC_RESERVED_22 = 22,
    CEC_WATCH = 23,
    CEC_RESERVED_24 = 24,
    CEC_RESERVED_25 = 25,
    CEC_RESERVED_26 = 26,
    CEC_RESERVED_27 = 27,
    CEC_RESERVED_28 = 28,
    CEC_RESERVED_29 = 29,
    CEC_RESERVED_30 = 30,
    CEC_VCE_DATA = 31,
    CEC_COUNT = 32,
} CpuExceptionCode;

typedef enum CpuMode {
    CM_NONE = -1,
    CM_USER = 0,
    CM_SUPER = 1,
    CM_KERNEL = 2,
} CpuMode;

typedef enum CpuSize {
    CS_NONE = -1,
    CS_32BIT = 0,
    CS_64BIT = 1,
} CpuSize;

typedef union CpuGpr {
    struct {
        /* 0x0 */ int8_t _0s8;
        /* 0x1 */ int8_t _1s8;
        /* 0x2 */ int8_t _2s8;
        /* 0x3 */ int8_t _3s8;
        /* 0x4 */ int8_t _4s8;
        /* 0x5 */ int8_t _5s8;
        /* 0x6 */ int8_t _6s8;
        /* 0x7 */ int8_t int8_t;
    };
    struct {
        /* 0x0 */ int16_t _0s16;
        /* 0x2 */ int16_t _1s16;
        /* 0x4 */ int16_t _2s16;
        /* 0x6 */ int16_t int16_t;
    };
    struct {
        /* 0x0 */ int32_t _0s32;
        /* 0x4 */ int32_t int32_t;
    };
    struct {
        /* 0x0 */ int64_t int64_t;
    };
    struct {
        /* 0x0 */ uint8_t _0u8;
        /* 0x1 */ uint8_t _1u8;
        /* 0x2 */ uint8_t _2u8;
        /* 0x3 */ uint8_t _3u8;
        /* 0x4 */ uint8_t _4u8;
        /* 0x5 */ uint8_t _5u8;
        /* 0x6 */ uint8_t _6u8;
        /* 0x7 */ uint8_t uint8_t;
    };
    struct {
        /* 0x0 */ uint16_t _0u16;
        /* 0x2 */ uint16_t _1u16;
        /* 0x4 */ uint16_t _2u16;
        /* 0x6 */ uint16_t uint16_t;
    };
    struct {
        /* 0x0 */ uint32_t _0u32;
        /* 0x4 */ uint32_t uint32_t;
    };
    struct {
        /* 0x0 */ uint64_t uint64_t;
    };
} CpuGpr;

typedef union CpuFpr {
    struct {
        /* 0x0 */ float _0f32;
        /* 0x4 */ float f32;
    };
    struct {
        /* 0x0 */ double f64;
    };
    struct {
        /* 0x0 */ int32_t _0s32;
        /* 0x4 */ int32_t int32_t;
    };
    struct {
        /* 0x0 */ int64_t int64_t;
    };
    struct {
        /* 0x0 */ uint32_t _0u32;
        /* 0x4 */ uint32_t uint32_t;
    };
    struct {
        /* 0x0 */ uint64_t uint64_t;
    };
} CpuFpr;

typedef struct CpuDevice {
    /* 0x00 */ int32_t nType;
    /* 0x04 */ void* pObject;
    /* 0x08 */ int32_t nOffsetAddress;
    /* 0x0C */ Get8Func pfGet8;
    /* 0x10 */ Get16Func pfGet16;
    /* 0x14 */ Get32Func pfGet32;
    /* 0x18 */ Get64Func pfGet64;
    /* 0x1C */ Put8Func pfPut8;
    /* 0x20 */ Put16Func pfPut16;
    /* 0x24 */ Put32Func pfPut32;
    /* 0x28 */ Put64Func pfPut64;
#if IS_OOT
    /* 0x2C */ GetBlockFunc pfGetBlock;
    /* 0x30 */ uint32_t nAddressVirtual0;
    /* 0x34 */ uint32_t nAddressVirtual1;
    /* 0x38 */ uint32_t nAddressPhysical0;
    /* 0x3C */ uint32_t nAddressPhysical1;
#elif IS_MM
    /* 0x2C */ uint32_t nAddressPhysical;
    /* 0x30 */ uint32_t nSize;
#endif
} CpuDevice; // size = 0x40

typedef struct CpuJump {
    /* 0x0 */ int32_t nOffsetHost;
    /* 0x4 */ int32_t nAddressN64;
} CpuJump; // size = 0x8

// cpu_callerID
typedef struct CpuCallerID {
    /* 0x0 */ int32_t N64address;
    /* 0x4 */ int32_t GCNaddress;
} CpuCallerID; // size = 0x8

typedef struct CpuFunction CpuFunction;

// cpu_function
struct CpuFunction {
    /* 0x00 */ void* pnBase;
    /* 0x04 */ void* pfCode;
    /* 0x08 */ int32_t nCountJump;
    /* 0x0C */ CpuJump* aJump;
    /* 0x10 */ int32_t nAddress0;
    /* 0x14 */ int32_t nAddress1;
    /* 0x18 */ CpuCallerID* block;
    /* 0x1C */ int32_t callerID_total;
    /* 0x20 */ int32_t callerID_flag;
    /* 0x24 */ uint32_t nChecksum;
    /* 0x28 */ int32_t timeToLive;
    /* 0x2C */ int32_t memory_size;
    /* 0x30 */ int32_t heapID;
    /* 0x34 */ int32_t heapWhere;
    /* 0x38 */ int32_t treeheapWhere;
    /* 0x3C */ CpuFunction* prev;
    /* 0x40 */ CpuFunction* left;
    /* 0x44 */ CpuFunction* right;
}; // size = 0x48

// cpu_treeRoot
typedef struct CpuTreeRoot {
    /* 0x00 */ uint16_t total;
    /* 0x04 */ int32_t total_memory;
    /* 0x08 */ int32_t root_address;
    /* 0x0C */ int32_t start_range;
    /* 0x10 */ int32_t end_range;
    /* 0x14 */ int32_t cache_miss;
    /* 0x18 */ int32_t cache[20];
    /* 0x68 */ CpuFunction* left;
    /* 0x6C */ CpuFunction* right;
    /* 0x70 */ int32_t kill_limit;
    /* 0x74 */ int32_t kill_number;
    /* 0x78 */ int32_t side;
    /* 0x7C */ CpuFunction* restore;
    /* 0x80 */ int32_t restore_side;
} CpuTreeRoot; // size = 0x84

// _CPU_ADDRESS
typedef struct CpuAddress {
    /* 0x0 */ int32_t nN64;
    /* 0x4 */ int32_t nHost;
    /* 0x8 */ CpuFunction* pFunction;
} CpuAddress; // size = 0xC

typedef struct CpuCodeHack {
    /* 0x0 */ uint32_t nAddress;
    /* 0x4 */ uint32_t nOpcodeOld;
    /* 0x8 */ uint32_t nOpcodeNew;
} CpuCodeHack; // size = 0xC

// cpu_optimize
typedef struct CpuOptimize {
    /* 0x00 */ uint32_t validCheck;
    /* 0x04 */ uint32_t destGPR_check;
    /* 0x08 */ int32_t destGPR;
    /* 0x0C */ int32_t destGPR_mapping;
    /* 0x10 */ uint32_t destFPR_check;
    /* 0x14 */ int32_t destFPR;
    /* 0x18 */ uint32_t addr_check;
    /* 0x1C */ int32_t addr_last;
    /* 0x20 */ uint32_t checkType;
    /* 0x24 */ uint32_t checkNext;
} CpuOptimize; // size = 0x28

typedef struct Cpu Cpu;
typedef int32_t (*CpuExecuteFunc)(Cpu* pCPU, int32_t nCount, int32_t nAddressN64, int32_t nAddressGCN);

#if IS_OOT
// _CPU
struct Cpu {
    /* 0x00000 */ int32_t nMode;
    /* 0x00004 */ int32_t nTick;
    /* 0x00008 */ int64_t nLo;
    /* 0x00010 */ int64_t nHi;
    /* 0x00018 */ int32_t nCountAddress;
    /* 0x0001C */ int32_t iDeviceDefault;
    /* 0x00020 */ uint32_t nPC;
    /* 0x00024 */ uint32_t nWaitPC;
    /* 0x00028 */ uint32_t nCallLast;
    /* 0x0002C */ CpuFunction* pFunctionLast;
    /* 0x00030 */ int32_t nReturnAddrLast;
    /* 0x00034 */ int32_t survivalTimer;
    /* 0x00038 */ uint32_t nTickLast;
    /* 0x0003C */ uint32_t nRetrace;
    /* 0x00040 */ uint32_t nRetraceUsed;
    /* 0x00044 */ CpuGpr aGPR[32];
    /* 0x00144 */ CpuFpr aFPR[32];
    /* 0x00244 */ uint64_t aTLB[48][5];
    /* 0x009C4 */ int32_t anFCR[32];
    /* 0x00A44 */ int64_t anCP0[32];
    /* 0x00B44 */ CpuExecuteFunc pfStep;
    /* 0x00B48 */ CpuExecuteFunc pfJump;
    /* 0x00B4C */ CpuExecuteFunc pfCall;
    /* 0x00B50 */ CpuExecuteFunc pfIdle;
    /* 0x00B54 */ CpuExecuteFunc pfRam;
    /* 0x00B58 */ CpuExecuteFunc pfRamF;
    /* 0x00B64 */ CpuDevice* apDevice[256];
    /* 0x00F64 */ uint8_t aiDevice[1 << DEVICE_ADDRESS_INDEX_BITS];
    /* 0x10F64 */ void* gHeap1;
    /* 0x10F68 */ void* gHeap2;
    /* 0x10F6C */ uint32_t aHeap1Flag[192];
    /* 0x1126C */ uint32_t aHeap2Flag[13];
    /* 0x1129C */ void* gHeapTree;
    /* 0x112A0 */ uint32_t aHeapTreeFlag[125];
    /* 0x11494 */ CpuTreeRoot* gTree;
    /* 0x11498 */ CpuAddress aAddressCache[256];
    /* 0x12098 */ int32_t nCountCodeHack;
    /* 0x1209C */ CpuCodeHack aCodeHack[32];
    /* 0x1221C */ uint32_t nFlagRAM;
    /* 0x12220 */ uint32_t nFlagCODE;
    /* 0x12224 */ uint32_t nCompileFlag;
    /* 0x12228 */ int32_t unk_12228;

    //! TODO: fix match issue with OSAlarm
    // /* 0x12230 */ OSAlarm alarmRetrace;
    /* 0x12230 */ int32_t alarmRetrace[12];

    /* 0x1225C */ int32_t unk_1225C;
    /* 0x12260 */ int32_t unk_12260;
    /* 0x12264 */ int32_t unk_12264;
    /* 0x12268 */ int32_t unk_12268;
    /* 0x1226C */ int32_t unk_1226C;
    /* 0x12270 */ CpuOptimize nOptimize;
    /* 0x12298 */ int64_t nTimeRetrace;
    uint8_t pad[0x30];
}; // size = 0x122D0

#elif IS_MM

//! TODO: figure out this struct on MM
//! (for now nothing is used so it doesn't matter except the size)
struct Cpu {
    uint32_t            status;                     /* 0x00000 */
    char                unk_0x04[0x14];             /* 0x00004 */
    struct System      *sys;                        /* 0x00018 */
    char                unk_0x1C[0x4];              /* 0x0001C */
    uint32_t            lo[2];                      /* 0x00020 */
    uint32_t            hi[2];                      /* 0x00028 */
    uint32_t            cache_cnt;                  /* 0x00030 */
    uint8_t             phys_ram_dev_idx;           /* 0x00034 */
    uint32_t            pc;                         /* 0x00038 */
    char                unk_0x3C[0x10];             /* 0x0003C */
    CpuFunction        *working_node;               /* 0x0004C */
    char                unk_0x50[4];                /* 0x00050 */
    int                 timer;                      /* 0x00054 */
    union {
        uint64_t        gpr64[32];
        uint32_t        gpr[64];
    };                                              /* 0x00058 */
    union {
        double          fpr64[32];
        float           fpr[64];
    };                                              /* 0x00158 */
    char                unk_0x258[0x780];           /* 0x00258 */
    uint32_t            FSCR[32];                   /* 0x009D8 */
    uint32_t            cp0[64];                    /* 0x00A58 */
    void               *exec_opcode_func;           /* 0x00B58 */
    void               *exec_jump_func;             /* 0x00B5C */
    void               *exec_call_func;             /* 0x00B60 */
    void               *exec_idle_func;             /* 0x00B64 */
    void               *exec_load_store_func;       /* 0x00B68 */
    void               *exec_fp_load_store_func;    /* 0x00B6C */
    uint32_t            time_hi;                    /* 0x00B70 */
    uint32_t            time_lo;                    /* 0x00B74 */
    char                unk_0xB78[8];               /* 0x00B78 */
    CpuDevice          *cpu_devs[256];              /* 0x00B80 */
    uint8_t             dev_idx[0x10000];           /* 0x00F80 */
    void               *sm_blk_code;                /* 0x10F80 */
    void               *lg_blk_code;                /* 0x10F84 */
    uint32_t            sm_blk_alloc[256];          /* 0x10F88 */
    uint32_t            lg_blk_alloc[13];           /* 0x11388 */
    CpuTreeRoot        *tree_ctx;                   /* 0x113BC */
    char                unk_0x113C0[0xDC0];         /* 0x113C0 */
    uint32_t            known_regs;                 /* 0x12180 */
    char                unk_0x12184[8];             /* 0x12184 */
    uint32_t            jr_is_ra;                   /* 0x1218C */
    char                unk_0x12190[0x18];          /* 0x12190 */
    uint32_t            prev_loadstore_base;        /* 0x121A8 */
    char                unk_0x121AC[0x14];          /* 0x121AC */
};                                                  /* 0x121C0 */

#endif

#endif
