#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	 DATA_TYPE inc_adr = op_src->val ;
	 cpu.eip += inc_adr;
	 cpu.esp -= DATA_BYTE;
	 print_asm("call\t0x%x", cpu.eip + 1 + DATA_BYTE);
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
