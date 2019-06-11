# asm

## boot

0000000000000040 t check_cpuid
0000000000000054 t check_cpuid.no_cpuid
000000000000005b t check_long_mode
0000000000000079 t check_long_mode.no_long_mode
0000000000000031 t check_multiboot
0000000000000039 t check_multiboot.no_multiboot
00000000000000c6 t enable_paging
00000000000000f6 t error
0000000000000000 r gdt64
0000000000000008 a gdt64.code
0000000000000010 a gdt64.data
0000000000000018 r gdt64.pointer
                 U long_mode_start
0000000000002000 b p2_table
0000000000001000 b p3_table
0000000000000000 b p4_table
000000000000007d t set_up_page_tables
00000000000000a9 t set_up_page_tables.map_p2_table
0000000000003000 b stack_bottom
0000000000007000 b stack_top
0000000000000000 T start

## interrupt

//isr interrupt service routine

//irq interrupt request

00000000000001fd T irq0
0000000000000207 T irq1
0000000000000211 T irq2
000000000000021b T irq3
0000000000000225 T irq4
00000000000001c4 t irq_common_stub
                 U irq_handler
0000000000000039 T isr0
0000000000000043 T isr1
00000000000000a6 T isr10
00000000000000b3 T isr11
00000000000000c0 T isr12
00000000000000cd T isr13
00000000000000da T isr14
00000000000000e7 T isr15
00000000000000f4 T isr16
0000000000000101 T isr17
000000000000010e T isr18
000000000000011b T isr19
000000000000004d T isr2
0000000000000128 T isr20
0000000000000135 T isr21
0000000000000142 T isr22
000000000000014f T isr23
000000000000015c T isr24
0000000000000169 T isr25
0000000000000176 T isr26
0000000000000183 T isr27
0000000000000190 T isr28
000000000000019d T isr29
0000000000000057 T isr3
00000000000001aa T isr30
00000000000001b7 T isr31
0000000000000061 T isr4
000000000000006b T isr5
0000000000000075 T isr6
000000000000007f T isr7
000000000000008c T isr8
0000000000000099 T isr9
0000000000000000 t isr_common_stub
                 U isr_handler

## long_mode_init

U kmain
0000000000000000 T long_mode_start

## multiboot_header.asm

0000000000000018 r header_end
0000000000000000 r header_start

# core

## boot (MULTIBOOT_HEADER相关)

0000000000000050 T find_multiboot_tag
                 U _GLOBAL_OFFSET_TABLE_
0000000000000000 r .LC0
000000000000001a r .LC1
0000000000000000 T multiboot_is_valid
                 U printf
0000000000000090 T read_multiboot_info

## check

0000000000000000 T check_interrupts
                 U _GLOBAL_OFFSET_TABLE_
0000000000000000 r .LC0
000000000000001a r .LC1
                 U printf
                 U timer_tick

## cmos

0000000000000030 T cmos_read_rtc
                 U _GLOBAL_OFFSET_TABLE_
                 U memcpy
                 U port_byte_in
                 U port_byte_out
0000000000000400 T read_register
0000000000000430 T rtc_values_are_not_equal
0000000000000000 T update_in_progress

## idt

0000000000001000 C idt
000000000000000a C idt_reg
0000000000000050 T set_idt
0000000000000000 T set_idt_gate

## isr

0000000000000000 T breakpoint_handler
0000000000000000 D exception_messages
0000000000000000 r __func__.1476
0000000000000490 T get_stack
                 U _GLOBAL_OFFSET_TABLE_
0000000000000800 C interrupt_handlers
                 U irq0
                 U irq1
                 U irq2
                 U irq3
                 U irq4
0000000000000360 T irq_disable
0000000000000400 T irq_handler
0000000000000350 T irq_init
                 U isr0
                 U isr1
                 U isr10
                 U isr11
                 U isr12
                 U isr13
                 U isr14
                 U isr15
                 U isr16
                 U isr17
                 U isr18
                 U isr19
                 U isr2
                 U isr20
                 U isr21
                 U isr22
                 U isr23
                 U isr24
                 U isr25
                 U isr26
                 U isr27
                 U isr28
                 U isr29
                 U isr3
                 U isr30
                 U isr31
                 U isr4
                 U isr5
                 U isr6
                 U isr7
                 U isr8
                 U isr9
0000000000000370 T isr_handler
0000000000000030 T isr_init
                 U kernel_panic
0000000000000000 r .LC0
0000000000000000 r .LC1
00000000000000a8 r .LC2
000000000000000f r .LC3
                 U port_byte_out
                 U printf
0000000000000480 T register_interrupt_handler
                 U set_idt
                 U set_idt_gate

## ports

0000000000000000 T port_byte_in
0000000000000010 T port_byte_out
0000000000000020 T port_word_in
0000000000000030 T port_word_out

## timer

                 U _GLOBAL_OFFSET_TABLE_
                 U port_byte_out
                 U register_interrupt_handler
0000000000000000 B tick
0000000000000000 t timer_callback
0000000000000020 T timer_init
0000000000000010 T timer_tick

# drivers

## keyboard

                 U _GLOBAL_OFFSET_TABLE_
0000000000000000 t keyboard_callback
0000000000000040 T keyboard_init
0000000000000000 r .LC0
                 U port_byte_in
                 U printf
                 U register_interrupt_handler

## screen

0000000000000030 T color_scheme
0000000000000008 C framebuffer
                 U _GLOBAL_OFFSET_TABLE_
                 U memcpy
0000000000000110 T move_cursor
                 U port_byte_out
00000000000000c0 T screen_clear
0000000000000004 C screen_col
0000000000000000 T screen_init
0000000000000280 T screen_print
0000000000000004 C screen_row
0000000000000001 C screen_scheme
0000000000000150 T screen_write
0000000000000040 T screen_write_at
                 U strlen

## serial

                 U _GLOBAL_OFFSET_TABLE_
                 U port_byte_in
                 U port_byte_out
0000000000000000 T serial_init
0000000000000120 T serial_is_transmit_fifo_empty
00000000000001c0 T serial_print
0000000000000080 T serial_printf
0000000000000160 T serial_read
0000000000000140 T serial_received
0000000000000190 T serial_write
                 U strlen
                 U vprintf

# include

# kernel

## kmain

                 U check_interrupts
                 U cmos_read_rtc
                 U find_multiboot_tag
0000000000000000 r __func__.1652
                 U _GLOBAL_OFFSET_TABLE_
                 U irq_init
                 U isr_init
                 U kernel_panic
                 U keyboard_init
0000000000000050 T kmain
0000000000000000 r .LC0
0000000000000000 r .LC1
0000000000000070 r .LC10
00000000000000a0 r .LC11
0000000000000057 r .LC12
0000000000000070 r .LC13
0000000000000089 r .LC14
00000000000000d0 r .LC15
00000000000000f8 r .LC16
00000000000000a5 r .LC17
0000000000000004 r .LC2
000000000000000d r .LC3
0000000000000019 r .LC4
000000000000001f r .LC5
0000000000000026 r .LC6
0000000000000043 r .LC7
0000000000000044 r .LC8
0000000000000040 r .LC9
                 U mmu_init
                 U multiboot_is_valid
                 U printf
0000000000000000 T print_welcome_messge
                 U read_multiboot_info
                 U screen_clear
                 U screen_init
                 U serial_init
                 U timer_init

## panic

                 U _GLOBAL_OFFSET_TABLE_
                 U irq_disable
0000000000000000 T kernel_panic
0000000000000000 r .LC0
                 U printf
                 U vprintf

# libc

## ctype

0000000000000000 T isdigit

## mem

0000000000000000 T memcpy

## stdio

                 U _GLOBAL_OFFSET_TABLE_
                 U isdigit
                 U itoa
00000000000002e0 T printf
0000000000000000 T putchar
0000000000000020 T puts
                 U screen_print
                 U screen_write
                 U serial_print
                 U serial_write
                 U strlen
                 U ulltoa
0000000000000040 T vprintf

## stdlib

                 U _GLOBAL_OFFSET_TABLE_
0000000000000000 T itoa
                 U reverse
0000000000000090 T ulltoa

## string

0000000000000030 T reverse
0000000000000000 T strlen

# mmu

## mmap

00000000000001a0 T frame_containing_address
00000000000001b0 T frame_starting_address
0000000000000008 C kernel_end
0000000000000008 C kernel_start
0000000000000008 C memory_area
0000000000000120 T mmap_allocate_frame
00000000000001c0 T mmap_deallocate_frame
00000000000000b0 T mmap_init
0000000000000100 T mmap_read
0000000000000000 t mmap_read.part.0
0000000000000008 C multiboot_end
0000000000000008 C multiboot_start
0000000000000004 C next_free_frame

## mmu

                 U _GLOBAL_OFFSET_TABLE_
                 U mmap_init
0000000000000000 T mmu_init
                 U paging_init

## paging

                 U frame_containing_address
                 U frame_starting_address
                 U _GLOBAL_OFFSET_TABLE_
00000000000002a0 T identity_map
00000000000002d0 T map
0000000000000240 T map_page_to_frame
                 U mmap_allocate_frame
                 U mmap_deallocate_frame
                 U mmap_read
0000000000000380 T next_table_address
00000000000001c0 T next_table_create
0000000000000330 T p1_index
0000000000000320 T p2_index
0000000000000310 T p3_index
0000000000000300 T p4_index
0000000000000010 T page_containing_address
0000000000000370 T page_starting_address
0000000000000000 T paging_init
0000000000000340 T pointed_frame
00000000000001a0 T set_addr_mask
00000000000000c0 T translate
0000000000000020 T translate_page
00000000000000e0 T unmap

