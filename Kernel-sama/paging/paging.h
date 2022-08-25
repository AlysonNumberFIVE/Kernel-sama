

#ifndef PAGING_H
#define PAGING_H
#include "../libc/mem.h"
#include "kmalloc.h"
#include "../drivers/screen.h"
#include "../cpu/types.h"

typedef union s_page {
    struct {
    u32 present         :1; // P
    u32 read_write      :1; // R/W
    u32 user_supervisor :1; // U/S
    u32 accessed        :1; // A
    u32 dirty           :1; // D
    u32 unused          :7;
    u32 address_frame   :20; // Address
    } fields;
    u32 bits;
}   t_page;
void    *get_physaddr(void *virtualaddr);

void 	init_paging();
void 	print_page_info(u32 virtual_address);

#endif
