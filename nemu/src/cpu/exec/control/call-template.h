#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	 DATA_TYPE inc_adr = op_src->val ;
	 cpu.esp -= DATA_BYTE;
	 swaddr_write(cpu.esp, DATA_BYTE, cpu.eip + DATA_BYTE + 1);
	 cpu.eip += inc_adr;
	 print_asm("call\t0x%x", cpu.eip + 1 + DATA_BYTE);
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
