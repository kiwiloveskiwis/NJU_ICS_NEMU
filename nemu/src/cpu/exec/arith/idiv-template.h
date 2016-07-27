#include "cpu/exec/template-start.h"

#define instr idiv

static void do_execute() {
	int32_t a = reg_w(R_EAX);
	int32_t b = op_src->val;
	REG(R_EAX) = a / b;
	if(a % b !=0) REG(R_EDX) = a % b;

	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
