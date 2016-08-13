#include "irq.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
uint8_t read_byte(uint32_t offset);
void serial_printc(char ch);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

char write_char;
void do_syscall(TrapFrame *tf) {
	int ecx = tf->ecx;
	int edx = tf->edx;
	Log("ecx=0x%x, edx=0x%x", ecx, edx);
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

		case SYS_brk: sys_brk(tf); break;
		case SYS_write: 
			while(edx > 0) { // what if for loop?
				asm("movl (%ecx), %ebx");
				asm("movb %ebx, write_char");
				serial_printc(write_char);
				edx--;
				ecx++;
			}
			tf->eax = tf->edx; // SYS_call return value: length of string
			break;


		/* TODO: Add more system calls. */

		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

