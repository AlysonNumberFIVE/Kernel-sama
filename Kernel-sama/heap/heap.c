
#include "heap.h"

u32 free_mem = 0x120000;
u32 to_use =   0x200000;
t_heap_block *heap = 0x0;

void    print_heap() {
    t_heap_block *trav = heap;
    kprint("print_heap()\n");
    int i = 0;
    while (trav->in_use == 1) {
        trav = trav->next;
        i++;
    }
    print_num(i);
    kprint(" . exxiting");
}

t_heap_block *new_heap(u32 start_address, u32 end_address, u32 memory_size, u32 *data) {
    
    t_heap_block *curr = (t_heap_block *)free_mem;

    curr->start_address = start_address;
    curr->in_use = 1;
    print_num(curr->start_address);
    curr->end_address = end_address;
    curr->memory_size = memory_size;
    curr->data = data;

    curr->next =(t_heap_block*)(free_mem + sizeof(t_heap_block));
    curr->next->in_use = 0;
    free_mem += sizeof(t_heap_block);
    return (curr);
}

t_heap_block *add_heap(u32 start_address, u32 end_address, u32 memory_size, u32 *data) {
    t_heap_block *curr = heap;
    int i = 0;

    while (curr->next->in_use == 1) {
        curr = curr->next;
        i++;
    } 
    kprint("add_heap: ");
    print_num(i);
    kprint("\n");
    curr->next = new_heap(start_address, end_address, memory_size, data);
    return curr;
}

t_heap_block *dynamic_allocator(u32 start_address, u32 end_address, u32 memory_size, u32 *data) {
    t_heap_block *curr = heap;

    if (curr->in_use == 0) {
        heap = new_heap(start_address, end_address, memory_size, data); 
    } else {
        curr = add_heap(start_address, end_address, memory_size, data);
        curr = heap;
    }
    return (curr);
}

void	*mallock(size_t size) {
    u32   size_to_map;
    if (size < PAGESIZE) {
        size_to_map = PAGESIZE;
    } else {
        u32 leftover = size % PAGESIZE;
        size_to_map = size - leftover;
        if (leftover > 0) {
            size_to_map += PAGESIZE;
        }        
    }

    t_heap_block *ret = dynamic_allocator(to_use, to_use + size_to_map, size_to_map, (u32 *)to_use); 
    void *block = (u32 *)to_use;
    to_use += size_to_map;
    return block;
}

