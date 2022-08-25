
#ifndef HEAP_H
#define HEAP_H

#define MAX_HEAP_LENGTH 32

#include "../cpu/types.h"
#include "../paging/paging.h"

typedef struct s_heap {
    u32 in_use;
    u32 start_address;
    u32 end_address;
    u32 memory_size;
    u32 *data;    
}   t_heap_block;

t_heap_block    heap[MAX_HEAP_LENGTH];

u32             get_latest_page();
void	        print_fields(t_page *page);
#endif
