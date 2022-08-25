
#include "heap.h"


u32             page_table_index = 0;
u32             page_index = 0;
extern          u32 *page_dir;

u32             get_latest_page() {
    if (page_index == 1024) {
        page_index = 0;
        page_table_index++;
    }
    u32     *this_table = (u32 *)page_dir[page_table_index];
    u32     page = (u32)((u32 *)this_table + (sizeof(u32) * page_index));
    page_index++;
    kprint("page index is ");
    print_num(page_index);
    page_table_index++;
    kprint("\n");
    return page;
}


