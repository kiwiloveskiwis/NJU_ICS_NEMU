#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	DATA_TYPE top_of_stack = swaddr_read(cpu.esp, DATA_BYTE);
	swaddr_write(op_src->addr, DATA_BYTE, top_of_stack);
	cpu.esp += DATA_BYTE;
	print_asm_template1();
}

	make_instr_helper(r)
	make_instr_helper(m)

#include "cpu/exec/template-end.h"

