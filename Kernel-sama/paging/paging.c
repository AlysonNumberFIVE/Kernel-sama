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
u32           frame_count;
extern void load_page_directory(unsigned int*);
extern void enable_paging();
// t_page_dir 	*page_dir;
t_page_dir	*this_dir;


// static void page_fault(registers_t regs) {
// 	kprint("Segfaiult\n");

// 	int present = !(regs.err_code & 0x1);
// 	int rw = regs.err_code & 0x2;
// 	int us = regs.err_code & 0x4;
// 	int reserved = regs.err_code & 0x8;

// 	kprint("Page fault: ");
// 	if (present) kprint("present ");
// 	if (rw) kprint("read-only ");
// 	if (us) kprint("user-mode ");
// 	if (reserved) kprint("rsereved ");
// 	kprint("\n");
// }

// u32 get_physical_page(int directory_index, int page_index) {
// 	return ((page_index * 1024) + directory_index) * 4;
// }

// u32 page_directory[1024]__attribute__((aligned(4096)));

// static void set_frame(u32 frame_addr)
// {
//    u32 frame = frame_addr/0x1000;
//    u32 idx = INDEX_FROM_BIT(frame);
//    u32 off = OFFSET_FROM_BIT(frame);
//    frames[idx] |= (0x1 << off);
// }

// static void clear_frame(u32 frame_addr)
// {
//    u32 frame = frame_addr/0x1000;
//    u32 idx = INDEX_FROM_BIT(frame);
//    u32 off = OFFSET_FROM_BIT(frame);
//    frames[idx] &= ~(0x1 << off);
// }

// static u32 test_frame(u32 frame_addr)
// {
//    u32 frame = frame_addr/0x1000;
//    u32 idx = INDEX_FROM_BIT(frame);
//    u32 off = OFFSET_FROM_BIT(frame);
//    return (frames[idx] & (0x1 << off));
// }

// static u32 first_frame()
// {
//    u32 i, j;
//    for (i = 0; i < INDEX_FROM_BIT(frame_count); i++)
//    {
//        if (frames[i] != 0xFFFFFFFF)
//        {
//            for (j = 0; j < 32; j++)
//            {
//                u32 to_test = 0x1 << j;
//                if ( !(frames[i]&to_test) )
//                    return i*4*8+j;
//            }
//        }
//    }
// }

// void	alloc_frame(t_page *page, int kernelspace, int writeable) {
// 	if (page->fields.address_frame != 0)
// 		return ;
// 	else {
//        u32 idx = first_frame();
//        if (idx == (u32)-1)
//        {
// 			kprint("FAILED\n");
// 			asm("hlt");
//        }
//        set_frame(idx*0x1000);
//        page->fields.present = 1;
//        page->fields.read_write = (writeable)?1:0;
//        page->fields.user_supervisor = (kernelspace)?0:1;
//        page->fields.address_frame = idx;
// 	}
// }


// t_page	*fetch_page(u32 addr, int new_page, t_page_dir *dir) {
// 	addr /= 0x1000;
// 	u32 table_index = addr / 1024;

// 	if (dir->tables[table_index])
// 		return &dir->tables[table_index]->pages[addr % 1024];
// 	else if (new_page) 
// 	{
// 		u32 tmp;
// 		dir->tables[table_index] = (t_page *)kmalloc(sizeof(t_page_table), 1, &tmp);
// 		memory_set(dir->tables[table_index], 0, 0x1000);
// 		dir->physical_address_tables[table_index] = tmp | 0x7;
// 		return &dir->tables[table_index]->pages[addr % 1024];
// 	}
// 	return 0;
// }

u32 *page_dir;

void 	paging_entry() {

	page_dir = (u32 *)kmalloc(sizeof(u32) * 1024, 1, -1);

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
		page.fields.read_write 		= 1;
		page.fields.user_supervisor = 1;
    	page.fields.accessed        = 1; 
    	page.fields.dirty           = 1; 
		page.fields.address_frame   = i;
		page_table[i] = page.bits;// (i * 0x1000) | 3;
		if (i == 0) {
			kprint("page :");
			char string[16] = "";
			hex_to_ascii(tmp, string);
			kprint(string);
			kprint("\n\n");
			print_num((int)page.fields.read_write);
			kprint("\nfirst_page is :");
			print_num((int)page_table[i]);
			kprint("\n");
		}
		// memory_copy((u8*)&first_page_table[i], (u8 *)&page, sizeof(u32));
	}
	page_dir[0] = ((unsigned int)page_table) | 3;
	print_num(page_dir[0]);
	load_page_directory(page_dir);
	enable_paging();
}


void test_page() {	
	// t_page page = (t_page )first_page_table[0];
	// print_num(page.bits);
	kprint("\n");
}


