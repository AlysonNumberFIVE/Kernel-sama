

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

	u32 *ptr = (u32*)0x12341223423523; // unsafe
	u32 do_page_fault = *ptr;

	char str[50];
	u32 address = 0;

	__asm__("mov %0, %%cr2" : "=r" (address));
	int_to_ascii(address, str);
	kprint("\noffending string is ");
	kprint(str);
}















