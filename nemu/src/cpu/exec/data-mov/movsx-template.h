#include "cpu/exec/template-start.h"

#define instr movsw
static void do_execute(){
	int result = (int)op_src->val;
	result = ((result << 16) >> 16);
	OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r) 
#undef instr

#define instr movsb
static void do_execute(){
	int shift = DATA_BYTE - 8;
	DATA_TYPE result = (DATA_TYPE)op_src->val;
	result = ((result << shift) >> shift);
	OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r) 
#undef instr
#include "cpu/exec/template-end.h"
