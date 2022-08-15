

#ifndef KMALLOC_H
#define KMALLOC_H

#include "../cpu/types.h"

u32 	kmalloc(u32 size, int align, u32 *physical_addr);

#endif
