#include "cpu/exec/template-start.h"

#define instr jmp
static void do_execute(){
	if(op_src->type == OP_TYPE_IMM) {	
		DATA_TYPE inc_adr = op_src->val ;
		cpu.eip += inc_adr;
		if(DATA_BYTE == 2) cpu.eip &= 0x0000ffff;
		print_asm("jmp $0x%x", cpu.eip + DATA_BYTE + 1);
	} else {
		// jmp_rm_v, mod==3
		if(DATA_BYTE == 2) cpu.eip = (op_src->val & 0x0000ffff) - DATA_BYTE;
		else cpu.eip = op_src->val - DATA_BYTE;
	}
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
#endif


#include "cpu/exec/template-end.h"


