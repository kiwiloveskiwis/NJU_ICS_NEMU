#include "cpu/exec/template-start.h"

#define instr movzx

//   1001c9:	0f b6 45 0c				movzbl 0xc(%ebp),%eax	string.c
//   8048841:	0f b6 c0				movzbl %al,%eax
static void do_execute(){
	op_src->val = (DATA_TYPE) op_src->val;

	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}
make_instr_helper(rm2r) 
#include "cpu/exec/template-end.h"
