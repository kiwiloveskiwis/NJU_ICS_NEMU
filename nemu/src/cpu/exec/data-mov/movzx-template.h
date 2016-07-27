#include "cpu/exec/template-start.h"

#define instr movzw
static void do_execute(){
	int result = (int)op_src->val;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r) 
#undef instr

#define instr movzb
static void do_execute(){
	DATA_TYPE result = (DATA_TYPE)op_src->val;
	OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r) 
#undef instr
#include "cpu/exec/template-end.h"
