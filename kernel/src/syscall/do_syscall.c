#include "irq.h"
#include "../../nemu/src/device/sdl.h"
#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
uint8_t read_byte(uint32_t offset);
void serial_printc(char ch);
void keyboard_event();

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

char sys_char;
int ecx;
void do_syscall(TrapFrame *tf) {
	ecx = tf->ecx;
	int edx = tf->edx;
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;
		case 1: // KEYBORAD_IRQ
			sys_char = in_byte(0x60);
			Log("key pressed!");
			assert(0);
			break;
		case SYS_brk: sys_brk(tf); break;
		case SYS_write: 
			while(edx > 0) { // what if for loop?
				asm("movl ecx, %ecx");
				asm("movl (%ecx), %edx");
				asm("movb %dl, sys_char");
				serial_printc(sys_char);
				edx--;
				ecx++;
			}
			tf->eax = tf->edx; // SYS_call return value: length of string
			break;


		/* TODO: Add more system calls. */

		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

