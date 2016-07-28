#include "cpu/exec/template-start.h"

#define instr movsw
static void do_execute(){
	int result = op_src->val & 0xffff;
	if(result >> 15) result |= 0xffff0000;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r) 
#undef instr

#define instr movsb
static void do_execute(){
	DATA_TYPE result = op_src->val & 0xff;
	if(result >> 31) result |= 0xffffff00;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r) 
#undef instr
#include "cpu/exec/template-end.h"
