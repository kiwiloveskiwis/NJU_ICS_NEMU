#ifndef __iret_h__
#define __iret_h__
#include <setjmp.h>
#include "cpu/exec/helper.h"
extern jmp_buf jbuf;
make_helper(iret) {
	cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;
	cpu.sr[R_CS].val = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;
	cpu.EFLAGS = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;
	print_asm_template1();
	longjmp(jbuf, 1);
	
	return 0; 
}




#endif
