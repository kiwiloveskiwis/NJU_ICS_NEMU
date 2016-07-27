#include "cpu/exec/template-start.h"

#define instr jmp
static void do_execute(){
	if(op_src->type == OP_TYPE_IMM) {	
		int inc_adr = op_src->val ;
		cpu.eip += inc_adr;
		if(DATA_BYTE == 2) cpu.eip = ((cpu.eip + DATA_BYTE + 1) & 0x0000ffff) - DATA_BYTE - 1;
		// TODO : right way?
		print_asm("jmp\t$0x%x", cpu.eip + DATA_BYTE + 1);
	} else { //reg or mem
		int off;
		if(op_src->type == OP_TYPE_REG) off = 3;
		else off = 2;
		cpu.eip = op_src->val - off;
		print_asm("jmp $0x%x", cpu.eip + off);
	}
	
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
#endif


#include "cpu/exec/template-end.h"


