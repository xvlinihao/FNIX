#ifndef MMU_MALLOCATE_H
#define MMU_MALLOCATE_H

#include <stdint.h>

uint64_t malloc(uint64_t size);
void malloc_init();

typedef union vir_addr {
    uint64_t packed;
    struct {
        uint16_t expanding :16;
        uint64_t page :36;
        uint16_t other :12;
    };
} __attribute__((packed)) vir_addr_t;

#endif