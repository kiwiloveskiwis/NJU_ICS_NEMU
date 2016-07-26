#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	 DATA_TYPE inc_adr = op_src->val ;
	 cpu.eip += inc_adr;
	 print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
