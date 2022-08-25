#include "paging.h"
#include "../cpu/types.h"
#include "../libc/string.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "../libc/function.h"

extern void load_page_directory(unsigned int*);
extern void enable_paging();

u32 *page_dir;

// void	set_table() {
// 	u32 *page_table = (u32 *)kmalloc(sizeof(u32) * 1024, 1, (u32 *)-1);
// 	int i = 0;
// 	while (i < 1024) {
// 		page_table[i] = (i * 0x1000) | 3;
// 		i++;
// 	}

// 	page_dir[current_page] = ((unsigned int)page_table) | 3;
// 	if (current_page == 1) {
// 		kprint("page_dir[current_page] ");
// 		print_num(page_dir[current_page]);
// 		kprint("\n");
// 	}
// 	// print_num(page_dir[current_page]); 
// 	// kprint("\n");
// 	current_page++;
// }

void 	page_fault(registers_t regs) {
   u32 faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   int present   = !(regs.err_code & 0x1); 
   int rw = regs.err_code & 0x2;           
   int us = regs.err_code & 0x4;           
   int reserved = regs.err_code & 0x8; 

   kprint("Page fault ( ");
   if (present) kprint("present ");
   if (rw) kprint("read-only ");
   if (us) kprint("user-mode ");
   if (reserved) kprint("rserved ");

   kprint(" )\n");
   kprint("Address ");
   print_num(faulting_address);
   kprint("\n");
}

void	print_fields(t_page *page) {

	u32 mapping = page->bits >> 8;
	mapping = ((page->bits >> 8) & mapping) & 0xff;
	page->bits = mapping;

	kprint("Total value is ");
	print_num(page->bits);
	kprint("\n");

	kprint("present : ");
	print_num(page->fields.present);
	kprint("\n");
	kprint("read_write : ");
	print_num(page->fields.read_write);
	kprint("\n");

	kprint("user_supervisor : ");
	print_num(page->fields.user_supervisor);
	kprint("\n");

	kprint("accessed : ");
    print_num(page->fields.accessed);
	kprint("\n");

	kprint("dirty : ");
    print_num(page->fields.dirty);
	kprint("\n");	
}

void 	print_page_info(u32 virtual_address) 
{
	u32 	mapping;

	u32 *ptr = (u32 *)virtual_address;
	t_page *page = (t_page *)ptr;
	page->bits = mapping;
	print_fields(page);
}

void 	init_paging() {
	page_dir 			= (u32 *)kmalloc(sizeof(u32) * 1024, 1, (u32 *)-1);
	u32	current_page 	= 0;
	int i 				= 0;
	
	while (i < 1024) {
		page_dir[i] = 0x00000002; 
		i++;
	}

	while (current_page < 1024) {
		u32 *page_table = (u32 *)kmalloc(sizeof(u32) * 1024, 1, (u32 *)-1);
		
		for(i = 0; i < 1024; i++) {
			page_table[i] = (i * 0x1000) | 3;
		}

		page_dir[current_page] = ((unsigned int)page_table) | 3;
		current_page++;
	}

	load_page_directory(page_dir);
	enable_paging();
	register_interrupt_handler(14, page_fault);
}


