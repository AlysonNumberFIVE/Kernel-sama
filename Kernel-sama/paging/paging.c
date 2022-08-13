#include "paging.h"
#include "../cpu/types.h"
#include "../libc/string.h"
#include "../cpu/isr.h"
#include "../libc/function.h"

extern void load_page_directory(unsigned int*);
extern void enable_paging();

void	activate_paging(u32 page_dir) {

	__asm__ ("mov %0, %%cr3" : : "r" (page_dir));
	// enable paging
	u32 address;
	char str2[50];
	__asm__("mov %%cr0, %0" : "=r" (address));
	int_to_ascii(address, str2);
	kprint("\nstring from eax is ");
	kprint(str2);
	kprint("\n");

	__asm__(
           // "mov %eax, %cr3\n\t"
            "mov %cr0, %eax\n\t"
            "or %eax, 0x8000000\n\t"
            "mov %eax, %cr0"
	);

	char str3[50];
	address = 0;
	__asm__("mov %%eax, %0" : "=r" (address));
	int_to_ascii(address, str3);
	kprint("\nstring from cr3 is ");
	kprint(str3);
	kprint("\n");
	kprint("activation ongoing" );
}

static void page_fault(registers_t regs) {
	kprint("Segfaiult\n");

	int present = !(regs.err_code & 0x1);
	int rw = regs.err_code & 0x2;
	int us = regs.err_code & 0x4;
	int reserved = regs.err_code & 0x8;

	kprint("Page fault: ");
	if (present) kprint("present ");
	if (rw) kprint("read-only ");
	if (us) kprint("user-mode ");
	if (reserved) kprint("rsereved ");
	kprint("\n");
}


u32 page_directory[1024]__attribute__((aligned(4096)));

void	 init_paging() {
	register_interrupt_handler(14, page_fault);
	
	// u32 page_directory[1024]__attribute__((aligned(4096)));
	int i;
	for(i = 0; i < 1024; i++)
		page_directory[i] = 0x00000002;

	u32 first_page_table[1024] __attribute__((aligned(4096)));
	for(i = 0; i < 1024; i++)
		first_page_table[i] =  (i * 0x1000) | 1;

	page_directory[0] = ((unsigned int)first_page_table) | 3;

	//init_paging(&page_directory);
	// activate_paging(*page_directory);
	//load_page_directory((u32)page_directory);
	load_page_directory(page_directory);
	enable_paging();
}
