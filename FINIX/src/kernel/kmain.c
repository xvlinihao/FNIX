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
#include "fs/myfs.h"
#include "fs/fs_lib.h"
#include "sch/scheduler.h"

multiboot_info_t *mbi;
mode_t mode = USER_MODE;
int command_mode = 0;

void kmain(unsigned long magic, unsigned long addr)
{
    screen_init();
    screen_clear();
    printf("%s",KERNEL_SYMBO);

    if (multiboot_is_valid(magic, addr) == false) {
        PANIC("invalid multiboot");
    }

    
    mbi = (multiboot_info_t*) addr;
    reserved_areas_t reserved = read_multiboot_info(mbi);
    printf("- READ INFORMATION FROM MULTIBOOT TAGS: OK!\n");
    
    //printf("- multiboot_start = 0x%x, multiboot_end = 0x%x\n", reserved.multiboot_start, reserved.multiboot_end);
    //printf("- kernel_start    = 0x%x, kernel_end    = 0x%x\n", reserved.kernel_start, reserved.kernel_end);

    
    isr_init();
    irq_init();
    printf("- INTERRUPTIONS ENABLED!\n");

    // memory
    multiboot_tag_mmap_t *mmap = find_multiboot_tag(mbi->tags, MULTIBOOT_TAG_TYPE_MMAP);
    mmu_init(mmap, reserved);
    printf("- PAGING AND MEMORY ALLOCATOR ENABLED!\n");

    timer_init(50); // 50hz
    printf("- CLOCK ENABLED!\n");
    
    // self-checks
    check_interrupts();
    
    // enable serial port
    serial_init(SERIAL_COM1, SERIAL_SPEED_115200);
    //DEBUG("%s has started", KERNEL_NAME);
    
    keyboard_init();
    printf("- KEYBOARD ROUTINE ENABLED!\n");
    
    // cmos
    cmos_rtc_t rtc = cmos_read_rtc();
    printf("- COMS-RTC ENABLED\n");
    /*
    printf(
        "- today's date and time is %2d/%2d/%2d %2d:%2d:%2d UTC\n",
        rtc.year, rtc.month, rtc.day,
        rtc.hours, rtc.minutes, rtc.seconds
    );
    */

    ata_init();


    scheduler_init();
    
    
    printf("$:");

    while (1) {
        __halt__();
    }
}

void get_commands(void *buf)
{
    keyboard_buffer_t *kb_buf = (keyboard_buffer_t *)buf;
    

//***********************************************************fs*****************************************************
    if(command_mode == 1)
    {
        int n = kb_buf->buffer[0]-'0';
        if(n>=0 && n<=9)
            deletedir(n);
        else
            printf("invalid inpur!\n");
        command_mode = 0;
    }

    else if(fs_strcmp(kb_buf->buffer,COM_BMAP) == 0)
    {
        seebmap();
        printf("\n");
    }
    else if(fs_strcmp(kb_buf->buffer,COM_WRITE1) == 0)
    {
        testwrite1();
    }
    else if(fs_strcmp(kb_buf->buffer,COM_READ1) == 0)
    {
        testread1();
    }
    else if(fs_strcmp(kb_buf->buffer,COM_WRITE2) == 0)
    {
        testwrite2();
    }
    else if(fs_strcmp(kb_buf->buffer,COM_READ2) == 0)
    {
        testread2();
    }
    else if(fs_strcmp(kb_buf->buffer,COM_LS) == 0)
    {
        ls();
    }
    else if(fs_strcmp(kb_buf->buffer,COM_TEST_CREATE) == 0)
    {
        testcreate();
    }
    else if(fs_strcmp(kb_buf->buffer,COM_DELETE_DIR) == 0)
    {
        command_mode = 1;
        printf("enter the number of dir you want to delete(0~9):\n");
    }
//****************************************************************************************************************




//************************************************sch***************************************************************
    else if(fs_strcmp(kb_buf->buffer,COM_HELP) == 0)
        printf("%s\n",HELP_MESSAGE);
    else if(fs_strcmp(kb_buf->buffer,COM_TIME) == 0)
    {
        cmos_rtc_t rtc = cmos_read_rtc();
        printf(
            "today's date and time is %2d/%2d/%2d %2d:%2d:%2d UTC\n",
            rtc.year, rtc.month, rtc.day,
            rtc.hours, rtc.minutes, rtc.seconds
        );
    }
    else if(fs_strcmp(kb_buf->buffer,COM_MMAP) == 0)
    {
        reserved_areas_t reserved = read_multiboot_info(mbi);
        
        printf("- multiboot_start = 0x%x, multiboot_end = 0x%x\n", reserved.multiboot_start, reserved.multiboot_end);
        printf("- kernel_start    = 0x%x, kernel_end    = 0x%x\n", reserved.kernel_start, reserved.kernel_end);

        multiboot_tag_mmap_t *mmap = find_multiboot_tag(mbi->tags, MULTIBOOT_TAG_TYPE_MMAP);
        multiboot_mmap_entry_t *entry;
        for (
            entry = ((multiboot_tag_mmap_t *) mmap)->entries;
            (uint8_t *) entry < (uint8_t *) mmap + mmap->size;
            entry = (multiboot_mmap_entry_t *) ((unsigned long) entry + ((multiboot_tag_mmap_t *) mmap)->entry_size)
        ) 
        {
            uint64_t base_addr = entry->addr;
            uint64_t length = entry->len;
            uint32_t type = entry->type;
            uint32_t reserved = entry->zero;
            printf("- base_addr:0x%x,length:0x%x,type:%x\n",base_addr,length,type);
        }
    }
//************************************************************************************************************    
    else if(fs_strcmp(kb_buf->buffer,COM_BGMD) == 0)
    {
        mode = BACKGROUND_MODE;
        //screen_clear();
        //printf("background mode start\n");
    }
    else if(fs_strcmp(kb_buf->buffer,COM_FORK) == 0)
    {
        fork();
    }
    else if(fs_strcmp(kb_buf->buffer,COM_PS) == 0)
    {
        ps();
    }
    else if(fs_strcmp(kb_buf->buffer,COM_JOB) == 0)
    {
        job();
    }
//****************************************************************************************************************
    
    else
        printf("no such command:%s\nenter help for more information\n",kb_buf->buffer);
}