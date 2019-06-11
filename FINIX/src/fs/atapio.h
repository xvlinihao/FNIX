#ifndef FS_ATAIO_H
#define FS_ATAIO_H

#include "core/ports.h"
#include "core/isr.h"

void ata_init(void);
uint8_t disk_state(void);
void software_reset(void);
void pio_write_sector(uint64_t buf, uint32_t lba);
void pio_read_sector(uint64_t buf, uint32_t lba);

#endif