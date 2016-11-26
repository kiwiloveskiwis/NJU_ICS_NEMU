#include "cpu/exec/template-start.h"

#define instr test  

static void do_execute() {
	DATA_TYPE_S opl = op_dest->val;
	DATA_TYPE_S opr = op_src->val;
	DATA_TYPE_S result = opl&opr;
	cpu.OF = 0;
	cpu.CF = 0;
	cpu.SF = !!(result>>(DATA_BYTE*8-1));
	cpu.ZF =!result;
	int i, evenjudge;
	for(i = 0; i < 7; i++) { 
		result >>= 1;
	}
	evenjudge = result & 0x1;
	cpu.PF = evenjudge;


	print_asm_template2();	
}	

	make_instr_helper(i2r)
	make_instr_helper(i2rm)
	make_instr_helper(r2rm)
	make_instr_helper(rm2r)
make_instr_helper(i2a)


#include "cpu/exec/template-end.h" 

