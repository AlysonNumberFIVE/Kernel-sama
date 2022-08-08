

u32 free_mem_addr = 0x10000;

u32 kmalloc(u32 size, int align, u32 *physical_address)
{
	if (align == 1 && (free_mem_addr & 0xFFFFF000))
	{
		free_mem_addr &= 0xFFFFF000;
		free_mem_addr += 0x1000;
	}
	if (physical_address)
	{
		*physical_address = free_mem_addr;
	}
	u32 tmp = free_mem_addr;
	free_mem_addr += size;
	return tmp;
}


