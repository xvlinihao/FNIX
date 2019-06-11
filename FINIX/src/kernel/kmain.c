#include "kmain.h"
#include <core/isr.h>
#include <core/timer.h>
#include <core/debug.h>
#include <core/check.h>
#include <core/cmos.h>
#include <mmu/mmu.h>
#include <kernel/panic.h>
#include <drivers/screen.h>
#include <drivers/serial.h>
#include <drivers/keyboard.h>
#include <stdio.h>

#include "core/cpu.h"
#include "fs/atapio.h"
#include "core/ports.h"

void print_welcome_messge()
{
    printf("%s\n", KERNEL_ASCII);
    printf("%s %s / Built on: %s at %s\n\n", KERNEL_NAME, KERNEL_VERSION, KERNEL_DATE, KERNEL_TIME);
}

void kmain(unsigned long magic, unsigned long addr)
{
    screen_init();
    screen_clear();


    if (multiboot_is_valid(magic, addr) == false) {
        PANIC("invalid multiboot");
    }

    
    multiboot_info_t *mbi = (multiboot_info_t*) addr;
    reserved_areas_t reserved = read_multiboot_info(mbi);

    
    //printf("- multiboot_start = 0x%x, multiboot_end = 0x%x\n", reserved.multiboot_start, reserved.multiboot_end);
    //printf("- kernel_start    = 0x%x, kernel_end    = 0x%x\n", reserved.kernel_start, reserved.kernel_end);

    
    isr_init();
    irq_init();
    
    printf("- interruptions enabled\n");

    // memory
    multiboot_tag_mmap_t *mmap = find_multiboot_tag(mbi->tags, MULTIBOOT_TAG_TYPE_MMAP);
    mmu_init(mmap, reserved);
    printf("- frame allocator, paging and malloc enabled\n");

    timer_init(50); // 50hz
    printf("- clock (timer) enabled\n");
    
    // self-checks
    check_interrupts();
    
    // enable serial port
    serial_init(SERIAL_COM1, SERIAL_SPEED_115200);
    DEBUG("%s has started", KERNEL_NAME);
    
    keyboard_init();
    printf("- keyboard routine enabled\n");
    
    // cmos
    cmos_rtc_t rtc = cmos_read_rtc();
    
    printf(
        "- today's date and time is %2d/%2d/%2d %2d:%2d:%2d UTC\n",
        rtc.year, rtc.month, rtc.day,
        rtc.hours, rtc.minutes, rtc.seconds
    );
    
    ata_init();
    scheduler_init();
    
    //printf("$:");

    while (1) {
        __halt__();
    }
}