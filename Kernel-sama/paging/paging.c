#include "paging.h"
#include "../cpu/types.h"
#include "../libc/string.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "../libc/function.h"

extern void load_page_directory(unsigned int*);
extern void enable_paging();

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

u32 get_physical_page(int directory_index, int page_index) {
	return ((page_index * 1024) + directory_index) * 4;
}

u32 page_directory[1024]__attribute__((aligned(4096)));

void	 init_paging() {
	register_interrupt_handler(14, page_fault);
	
	int i;
	for(i = 0; i < 1024; i++)
		page_directory[i] = 0x00000002;

	u32 first_page_table[1024] __attribute__((aligned(4096)));
	for(i = 0; i < 1024; i++) {
		t_page page;
		memory_set((u8 *)&page, 0, sizeof(t_page));
		page.fields.present 		= 1;
		page.fields.read_write 		= 1;
		page.fields.user_supervisor = 1;
    	page.fields.accessed        = 1; 
    	page.fields.dirty           = 0; 
		page.fields.address_frame   = i;
		first_page_table[i] 		= page.bits; // (i * 0x1000) | 3;
	
		kprint("page :");
		print_num((int)page.bits);
		kprint("\nfirst_page is :");
		print_num((int)first_page_table[i]);
		kprint("\n");
		// memory_copy((u8*)&first_page_table[i], (u8 *)&page, sizeof(u32));
	}
	page_directory[0] = ((unsigned int)first_page_table) | 3;
	load_page_directory(page_directory);
	enable_paging();
}

void test_page() {	
	// t_page page = (t_page )first_page_table[0];
	// print_num(page.bits);
	kprint("\n");
}


