#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	swaddr_write(cpu.esp, DATA_BYTE, op_src->val);
	cpu.esp -= DATA_BYTE;
	print_asm_template1();
}

	make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
	make_instr_helper(r)
	make_instr_helper(m)
#endif


#include "cpu/exec/template-end.h"
