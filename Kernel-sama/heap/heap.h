
#ifndef HEAP_H
#define HEAP_H

#define MAX_HEAP_LENGTH 32
#define PAGESIZE 4096
#define ONE_EIGTH 512

#include "../cpu/types.h"
#include "../paging/paging.h"
#include "../libc/string.h"

typedef struct s_heap {
    u32 in_use;
    u32 start_address;
    u32 end_address;
    u32 memory_size;
    u32 *data;

    struct s_heap *next;    
}   t_heap_block;

void	*mallock(size_t size);
void    print_heap();


#endif
