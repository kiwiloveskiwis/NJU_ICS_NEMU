#include "cpu/exec/template-start.h"

#define instr call
// DATA_BYTE can only be 2 or 4
static void do_execute(){
	if(op_src->type == OP_TYPE_IMM) {	
		int len = concat(decode_i_, SUFFIX)(cpu.eip + 1);
		cpu.esp -= 4;
		MEM_W(cpu.esp, cpu.eip + len + 1, R_SS);
		cpu.eip += op_src->val;
		print_asm_template1();
	}
	else {
		int off = concat(decode_rm_, SUFFIX)(cpu.eip + 1) + 1;
		cpu.esp -= DATA_BYTE;
		MEM_W(cpu.esp, cpu.eip + off, R_SS);
		cpu.eip = op_src->val - off;
	}
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
