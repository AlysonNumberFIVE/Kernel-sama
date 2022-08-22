

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
//    u32 write_through   :1; // PWT
//    u32 cache_disable   :1; // PCD
    u32 accessed        :1; // A
    u32 dirty           :1; // D
    u32 unused          :7;
    u32 address_frame   :20; // Address
    } fields;
    u32 bits;
}   t_page;

typedef union s_page_table {
    t_page pages[1024];
}   t_page_table;

typedef struct s_page_dir {
    t_page_table *tables[1024];
    u32 physical_address_tables[1024];
    u32 physical_address;
}   t_page_dir;
void 	paging_entry();

typedef union s_page_r {
    struct {
        u32 address_frame   :20; // Address
        u32 unused          :7;
        u32 dirty           :1;
        u32 accessed        :1;
        u32 user_supervisor :1;
        u32 read_write      :1;
        u32 present         :1;
    } fields;
    u32 bits;
}   t_page_r;


void init_paging();
u32 get_physical_page(int directory_index, int page_index);
void test_page();

#endif
