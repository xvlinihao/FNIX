#include "atapio.h"
#include <stdlib.h>

static void ata_callback(stack_t *stack)
{
	UNUSED(*stack);
}

void ata_init(void)
{
    register_interrupt_handler(IRQ14, ata_callback);
}

waitdisk(void) 
{
    while ((port_byte_in(0x1F7) & 0xC0) != 0x40)
        /* do nothing */;
}

uint8_t disk_state(void)
{
	port_byte_in(0x3f6);
	port_byte_in(0x3f6);
	port_byte_in(0x3f6);
	port_byte_in(0x3f6);
	printf("disk_state:0x%x\n",port_byte_in(0x3f6));
	return port_byte_in(0x3f6);
}
void software_reset(void)
{
	port_byte_out(0x3f6,0x04);
    port_byte_out(0x3f6,0);
    printf("software_reset!\n");
    disk_state();
}
void pio_write_sector(uint64_t buf, uint32_t lba)
{
    port_byte_out(0x1F2, 1);                        
    port_byte_out(0x1F3, lba & 0xFF);
    port_byte_out(0x1F4, (lba >> 8) & 0xFF);
    port_byte_out(0x1F5, (lba >> 16) & 0xFF);
    port_byte_out(0x1F6, ((lba >> 24) & 0xF) | 0xe0);

    port_byte_out(0x1f7,0x30);

    waitdisk();

    for(int i = 0;i<256;i++, buf+=2)
    {
        port_word_out(0x1f0,*(uint16_t *)buf);
    }
}
void pio_read_sector(uint64_t buf, uint32_t lba)
{
	port_byte_out(0x1F2, 1);                        
    port_byte_out(0x1F3, lba & 0xFF);
    port_byte_out(0x1F4, (lba >> 8) & 0xFF);
    port_byte_out(0x1F5, (lba >> 16) & 0xFF);
    port_byte_out(0x1F6, ((lba >> 24) & 0xF) | 0xe0);

    port_byte_out(0x1f7,0x20);

    waitdisk();
    
    for (int i = 0; i < 256; i ++, buf += 2)
    {
        *(uint16_t *)buf = port_word_in(0x1f0);
    }
}