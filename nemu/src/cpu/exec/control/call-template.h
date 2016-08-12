#include "cpu/exec/template-start.h"
#define instr call

make_helper(concat(call_i_,SUFFIX))
{ 
	int len = concat(decode_i_, SUFFIX)(eip+1);
	cpu.esp -= 4;
	MEM_W(cpu.esp, cpu.eip + len + 1, R_SS);
	cpu.eip += op_src->val;
	print_asm_template1();
	return len + 1;
}

make_helper(concat(call_rm_,SUFFIX)) {
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, cpu.eip + len + 1, R_SS);

	cpu.eip = op_src->val - len - 1;

	print_asm_template1();
	return len + 1;
}
#include "cpu/exec/template-end.h"
