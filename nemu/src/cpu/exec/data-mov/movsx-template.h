#include "cpu/exec/template-start.h"

#define instr movsw
static void do_execute(){
	int result = op_src->val & 0xffff;
	result = ((result << 16) >> 16);
	OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r) 
#undef instr

#define instr movsb
static void do_execute(){
	int shift = DATA_BYTE - 8;
	DATA_TYPE result = op_src->val & 0xff;
	result = ((result << shift) >> shift);
	OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r) 
#undef instr
#include "cpu/exec/template-end.h"
