#include "cpu/exec/template-start.h"

#define instr je
static void do_execute(){
	if(!cpu.ZF) return;
	DATA_TYPE inc_adr = op_src->val ;
	cpu.eip += inc_adr;
	print_asm_template1();
}

make_instr_helper(i)
#undef instr


#include "cpu/exec/template-end.h"

