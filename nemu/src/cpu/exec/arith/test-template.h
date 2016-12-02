#include "cpu/exec/template-start.h"

#define instr test  

static void do_execute() {
	DATA_TYPE result = op_dest->val & op_src->val;
//	OPERAND_W(op_dest, result); RESULT DISCARDED!!!!
	cpu.CF = 0;
	cpu.OF = 0;
	update_PZS(DATA_BYTE, result);
	print_asm_template2();	
}	

	make_instr_helper(i2r)
	make_instr_helper(i2rm)
	make_instr_helper(r2rm)
	make_instr_helper(rm2r)
make_instr_helper(i2a)


#include "cpu/exec/template-end.h" 

