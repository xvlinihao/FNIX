#include "mallocate.h"
#include "paging.h"
#include "mmap.h"

#define page_mask 0x0000fffffffff000

uint64_t next_free_page = 0x200;

uint64_t malloc(uint64_t size)
{
	uint64_t address;
	int page_request = size/PAGE_SIZE + ((size%PAGE_SIZE)>0);
	uint64_t next_free_page_t = next_free_page;
	for(int i=0;i<page_request;i++)
	{
		map(next_free_page_t++,0x3);
	}
	address = (next_free_page<<12)&page_mask;
	next_free_page+=page_request;
	return address;
}

void malloc_init()
{
	
}