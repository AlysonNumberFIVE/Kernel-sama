

#include "../cpu/types.h"

u32 	free_memory = 0x10000;

u32	kmalloc(u32 size, int align, u32 *physical_addr) 
{
	u32 target;

	if (align == 1 && (free_memory & 0xFFFFF000)) {
		free_memory &= 0xFFFFF000;
		free_memory += 0x1000;
	}

	if (physical_addr) 
		*physical_addr = free_memory;
	target = free_memory;
	free_memory += size;
	return target;
} 
