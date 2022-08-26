

#include "../heap/heap.h"
#include "../paging/kmalloc.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../libc/string.h"
#include "../paging/paging.h"
#include "../heap/heap.h"

void user_input(char *input) {
	if (strcmp(input, "END") == 0) 
	{
		kprint("Halting CPU");
		asm volatile("hlt");
	}
	 else if (strcmp(input, "MALLOC") == 0) 
	{
        u32 phys_addr;
        u32 page = kmalloc(10000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");
	}
	else if (strlen(input) == 0)
		kprint("$>");
	else 
	{
		kprint("echo :");
		kprint(input);
		kprint("\n$>");
	}
}

extern u32 *page_dir;
extern t_heap_block *heap;

void main() {
	isr_install();
	irq_install();
	memory_set((u8*)heap, 0, sizeof(t_heap_block));

	mallock(42);
	mallock(42);
	mallock(42);
	print_heap();
}














