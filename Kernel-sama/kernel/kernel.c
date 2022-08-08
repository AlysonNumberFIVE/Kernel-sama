

#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../libc/string.h"



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

	kprint("Take it away Kernel-sama!");
}
