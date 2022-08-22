

#include "../paging/kmalloc.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../libc/string.h"
#include "../paging/paging.h"


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

void main() {
	isr_install();
	irq_install();
///	u32 *ptr = (u32*)0x412341243; // unsafe
//	u32 do_page_fault = *ptr;
	paging_entry();

	u32 address = 0;
	u32 *ptr = (u32 *)0x13023;
	// char str[16] = "";
	// hex_to_ascii(str, ptr);
	u32 page_table = (u32)page_dir[0];

	// kprint("\n");
	kprint("\n\n>> ");
	t_page *page = (t_page*)ptr;
	u32 mapping = page->bits >> 8;
	mapping = ((page->bits >> 8) & mapping) & 0xff;
	kprint("mapping is ");
	print_num(mapping);
	kprint("\n");
	print_num(page->bits);
	kprint("\n");
	print_num(page->fields.read_write);
	kprint("\n");
	print_num(page->fields.present);
	kprint("\n");
	print_num(page->fields.read_write);
	kprint("\n");
	print_num(page->fields.user_supervisor);
	kprint("\n");
    print_num(page->fields.accessed);
	kprint("\n");
    print_num(page->fields.dirty);
	// __asm__("mov %0, %%cr2" : "=r" (address));

}















