#include "cpu/exec/template-start.h"

#define instr ret
static void do_execute(){
	cpu.eip = swaddr_read(cpu.esp, 4) - 1 - DATA_BYTE;
	cpu.esp += 4;
	DATA_TYPE inc_adr = op_src->val ;
	cpu.esp += inc_adr;
	print_asm("ret 0x%x", inc_adr);
}

make_instr_helper(i)
#undef instr


#include "cpu/exec/template-end.h"


