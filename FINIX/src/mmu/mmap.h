#ifndef MMU_MMAP_H
#define MMU_MMAP_H

#include <core/boot.h>

#define MMAP_GET_NUM    0
#define MMAP_GET_ADDR   1

typedef uint64_t physical_address_t;
typedef uint64_t frame_t;

typedef union CR0 {
    uint32_t packed;
    struct {
        uint8_t PE : 1;
        uint8_t MP : 1;
        uint8_t EM : 1;
        uint8_t TS : 1;
        uint8_t ET : 1;
        uint8_t NE : 1;
        uint16_t reserved_1 : 10;
        uint8_t WP : 1;
        uint8_t reserved_2 : 1;
        uint8_t AM : 1;
        uint16_t reserved_3 : 10;
        uint8_t NW : 1;
        uint64_t CD : 1;
        uint8_t PG : 1;
    };
} __attribute__((packed)) CR0_t;

typedef union CR4 {
    uint64_t packed;
    struct {
        uint8_t VME : 1;
        uint8_t PVI : 1;
        uint8_t TSD : 1;
        uint8_t DE : 1;
        uint8_t PSE : 1;
        uint8_t PAE : 1;
        uint8_t MCE : 1;
        uint8_t PGE : 1;
        uint8_t PCE : 1;
        uint8_t OSFXSR : 1;
        uint8_t OSXMMEXCPT : 1;
        uint8_t UMIP : 1;
        uint8_t reserved_1 : 1;
        uint8_t VMXE : 1;
        uint8_t SMXE : 1;
        uint8_t reserved_2 : 1;
        uint8_t PCIDE : 1;
        uint8_t OSXSAVE : 1;
        uint8_t reserved : 1;
        uint8_t SMEP : 1;
        uint8_t SMAP : 1;
        uint64_t reserved_3 : 42;
    };
} __attribute__((packed)) CR4_t;

typedef union CR3 {
    uint64_t packed;
    struct {
        uint8_t reserved_1 : 3;
        uint8_t PWT : 1;
        uint8_t PCD : 1;
        uint8_t reserved_2 : 7;
        uint64_t ADDR : 40;
        uint16_t ZERO : 12;
    };
} __attribute__((packed)) CR3_t;


void mmap_init(multiboot_tag_mmap_t *mmap, reserved_areas_t reserved);
frame_t mmap_allocate_frame();
physical_address_t mmap_read(frame_t request, uint8_t mode);
frame_t frame_containing_address(uint64_t addr);
uint64_t frame_starting_address(frame_t frame);
void mmap_deallocate_frame(frame_t frame);

#endif
