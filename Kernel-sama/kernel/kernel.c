

#include "../paging/kmalloc.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../libc/string.h"
#include "../paging/paging.h"


void user_input(char *input) {
	if (strcmp(input, "END") == 0) {
		kprint("Halting CPU");
		asm volatile("hlt");
	}
	if (strcmp(input, "MALLOC") == 0) 
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
	if (strlen(input) == 0)
	{	
		kprint("$>");
		return;
	}
	else {
		kprint("echo :");
		kprint(input);
		kprint("\n$>");
	}
//	kprint("\n> ");
}


void main() {
	isr_install();
	irq_install();

//	u32 *ptr = (u32*)0x412341243; // unsafe
//	u32 do_page_fault = *ptr;


	test_page();


	char str[50];
	u32 address = 0;


	__asm__("mov %0, %%cr2" : "=r" (address));
	int_to_ascii(address, str);
	kprint("\noffending string is ");
	kprint(str);
}















