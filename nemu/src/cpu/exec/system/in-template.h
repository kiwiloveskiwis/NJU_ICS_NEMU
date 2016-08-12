#include "cpu/exec/template-start.h"
#define instr in
make_helper(concat(in_i2a_, SUFFIX)) {
	uint8_t port = instr_fetch(eip + 1, 1);
	cpu.eax = (DATA_TYPE)pio_read(port, DATA_BYTE);
	print_asm_template1();
	return 2;
}

make_helper(concat(in_d2a_, SUFFIX)) {
	uint16_t port = reg_w(R_DX);
	cpu.eax = (DATA_TYPE)pio_read(port, DATA_BYTE);
	print_asm_template1();
	return 1;
}

#include "cpu/exec/template-end.h"
