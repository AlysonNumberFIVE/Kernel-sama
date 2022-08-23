#include "paging.h"
#include "../cpu/types.h"
#include "../libc/string.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "../libc/function.h"

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

extern u32    free_memory;
u32           mem_end_page = 0x1000000;
u32           *frames;
u32			  current_page = 1;
extern void load_page_directory(unsigned int*);
extern void enable_paging();


u32 *page_dir;

void	set_table() {
	u32 *page_table = (u32 *)kmalloc(sizeof(u32) * 1024, 1, (u32 *)-1);
	int i = 0;
	while (i < 1024) {
		t_page page;

		memory_set((u8 *)&page, 0, sizeof(t_page));
		page.fields.present 		= 0;
		page.fields.read_write 		= 0;
		page.fields.user_supervisor = 0;
    	page.fields.accessed        = 0; 
    	page.fields.dirty           = 0; 
		page.fields.address_frame   = i;
		page_table[i] = page.bits;
		i++;
	}

	page_dir[current_page] = ((unsigned int)page_table) | 3; 
	current_page++;
}

void 	page_fault(registers_t regs) {
   u32 faulting_address;
   asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

   int present   = !(regs.err_code & 0x1); 
   int rw = regs.err_code & 0x2;           
   int us = regs.err_code & 0x4;           
   int reserved = regs.err_code & 0x8; 
  // int id = regs.err_code & 0x10;

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

void 	print_page_info(u32 virtual_address) 
{
	u32 	mapping;

	u32 *ptr = (u32 *)virtual_address;
	t_page *page = (t_page *)ptr;
	mapping = page->bits >> 8;
	print_num(mapping);
	kprint("\n");
	mapping = ((page->bits >> 8) & mapping) & 0xff;
	page->bits = mapping;
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

void 	paging_entry() {

	page_dir = (u32 *)kmalloc(sizeof(u32) * 1024, 1, (u32 *)-1);

	int i = 0;
	while (i < 1024) {
		page_dir[i] = 0x00000002; 
		i++;
	}

	u32 tmp;
	u32 *page_table = (u32 *)kmalloc(sizeof(u32) * 1024, 1, &tmp);
	for(i = 0; i < 1024; i++) {
		t_page page;

		memory_set((u8 *)&page, 0, sizeof(t_page));
		page.fields.present 		= 1;
		page.fields.read_write 		= 0;
		page.fields.user_supervisor = 0;
    	page.fields.accessed        = 1; 
    	page.fields.dirty           = 0; 
		page.fields.address_frame   = i;
		page_table[i] = page.bits;
	}
	page_dir[0] = ((unsigned int)page_table) | 3;
	while (current_page < 1024) {
		set_table();
	}
	load_page_directory(page_dir);
	enable_paging();
	register_interrupt_handler(14, page_fault);
}


