#include "paging.h"
#include <mmu/mmap.h>
#include <core/debug.h>

uint64_t p4_index(frame_t frame);
uint64_t p3_index(frame_t frame);
uint64_t p2_index(frame_t frame);
uint64_t p1_index(frame_t frame);
frame_t pointed_frame(page_entry_t entry);
void set_addr_mask(page_entry_t *entry, uint64_t addr);
page_table_t next_table_create(page_table_t in, uint64_t index);
page_table_t next_table_address(page_table_t table, uint32_t index);


void paging_init()
{
    /*
    page_table_t p1_t = P1_TABLE;
    for(int i=0;i<512;i++)
        p1_t[i].packed = (i<<12 | 0x07);
    /*
    uint64_t t4;
    uint64_t t3;
    uint64_t t2;
    uint64_t t1;
    t4 = *((uint64_t *)P4_TABLE);
    t3 = *((uint64_t *)P3_TABLE);
    t2 = *((uint64_t *)P2_TABLE);
    t1 = *((uint64_t *)P1_TABLE);
    printf("- P4_TABLE[0]:0x%x\n", t4);
    printf("- P3_TABLE[0]:0x%x\n", t3);
    printf("- P2_TABLE[0]:0x%x\n", t2);
    printf("- P1_TABLE[0]:0x%x\n", t1);
    */
    /*
    page_table_t p3 = next_table_create((page_table_t)P4_TABLE, 0);
    page_table_t p2 = next_table_create((page_table_t)(P3_TABLE | (uint64_t)p3), 0);
    page_table_t p1 = next_table_create((page_table_t)(P4_TABLE | (uint64_t)p2), 0);
    for(int i = 0;i<512;i++)
    {
        p1[i].packed = (0x000ffffffffff03 | (i<<8));
    }
    */
}

page_t page_containing_address(uint64_t addr)//虚拟地址->虚拟地址页地址部分
{
    return addr / PAGE_SIZE;
}

physical_address_t translate(virtual_address_t addr)
{
    page_t virt = page_containing_address(addr);

    //uint64_t offset = virt % PAGE_SIZE;//???
    uint64_t offset = addr % PAGE_SIZE;

    return (physical_address_t) (page_starting_address((uint64_t) translate_page(virt)) + offset);
}

frame_t translate_page(page_t page)//利用p4_table的递归, 翻译虚拟地址
{
    page_table_t p3 = next_table_address((page_table_t)P4_TABLE, p4_index(page));
    if (!p3) {
        printf("1");
        return 0;
    }

    // TODO: huge page

    page_table_t p2 = next_table_address(p3, p3_index(page));
    if (!p2) {
        printf("2");
        return 0;
    }

    page_table_t p1 = next_table_address(p2, p2_index(page));
    if (!p1) {
        printf("3");
        return 0;
    }

    page_entry_t entry = p1[p1_index(page)];
    //printf("%X\n",entry);
    return pointed_frame(entry);
}

void map_page_to_frame(page_t page, frame_t frame, uint8_t flags)
{
    page_table_t p3 = next_table_create((page_table_t)P4_TABLE, p4_index(page));
    //printf("p3 = 0x%X\n", p3);

    page_table_t p2 = next_table_create((page_table_t)p3, p3_index(page));
    //printf("p2 = 0x%X\n", p2);

    page_table_t p1 = next_table_create((page_table_t)p2, p2_index(page));
    //printf("p1 = 0x%X\n", p1);

    set_addr_mask(&p1[p1_index(page)], frame);
    //printf("%X\n",frame);
    //p1[p1_index(page)].packed |= flags;
    p1[p1_index(page)].present = 1;
    p1[p1_index(page)].writable = 1;
    p1[p1_index(page)].huge_page = 0;
}

void identity_map(frame_t frame, uint8_t flags)
{
    page_t page = page_containing_address(frame_starting_address(frame));
    map_page_to_frame(page, frame, flags);
}

void map(page_t page, uint8_t flags)
{
    frame_t frame = mmap_allocate_frame();
    map_page_to_frame(page, mmap_read(frame, MMAP_GET_ADDR), flags);
    //printf("maped page%X to frame%X\n",page,frame);
}

void unmap(page_t page)
{
    page_table_t p3 = next_table_address((page_table_t)P4_TABLE, p4_index(page));
    page_table_t p2 = next_table_address(p3, p3_index(page));
    page_table_t p1 = next_table_address(p2, p2_index(page));

    // TODO: free p(1,2,3) table if empty

    page_entry_t entry = p1[p1_index(page)];

    if (!entry.present) {
        DEBUG("%s", "ERROR");
        return;
    }

    p1[p1_index(page)].present = 0;
    mmap_deallocate_frame(pointed_frame(entry));

    // flush the translation lookaside buffer
    // cf. http://os.phil-opp.com/modifying-page-tables.html#unmap
    __asm__("invlpg (%0)" ::"r" (page_starting_address(page)) : "memory");
    DEBUG("unmaped page%x\n",page);
}

void set_addr_mask(page_entry_t *entry, uint64_t addr)
{
    uint64_t mask = 0xfff0000000000fff;
    //entry->packed  = 0;
    entry->packed &= mask;
    entry->packed |= addr;
}

page_table_t next_table_create(page_table_t table, uint64_t index)
{
    page_entry_t location = table[index];
    page_table_t res;

    DEBUG("index = %L - start", index);

    if (!location.present) {
        frame_t frame = mmap_allocate_frame();
        physical_address_t addr = mmap_read(frame, MMAP_GET_ADDR);

        DEBUG("index = %L, frame = %d, addr = 0x%X", index, frame, addr);

        set_addr_mask(&table[index], addr);
        table[index].present = 1;
        table[index].writable = 1;
        table[index].huge_page = 0;
    }

    res = next_table_address(table, index);

    return res;
}

uint64_t p4_index(frame_t frame)
{
    return (frame >> 27) & 0777;
}

uint64_t p3_index(frame_t frame)
{
    return (frame >> 18) & 0777;
}

uint64_t p2_index(frame_t frame)
{
    return (frame >> 9) & 0777;
}

uint64_t p1_index(frame_t frame)
{
    return (frame >> 0) & 0777;
}

frame_t pointed_frame(page_entry_t entry)
{
    if (entry.present) {
        uint64_t e = entry.packed;
        e &= 0x000ffffffffff000;

        return frame_containing_address(e);
    }

    return 0;
}

uint64_t page_starting_address(page_t page)
{
    return page * PAGE_SIZE;
}

page_table_t next_table_address(page_table_t table, uint32_t index)
{
    if (table[index].present && !table[index].huge_page) {
        uint64_t table_address = (uint64_t) table;

        return (page_table_t) ((table_address << 9) | (index << 12));
    }

    return 0;
}
