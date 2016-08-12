#include "cpu/exec/template-start.h"
#include "../include/device/port-io.h"
#define instr out
make_helper(concat(out_a2i_, SUFFIX)) {
	uint8_t port = instr_fetch(eip + 1, 1);
	pio_write(port, DATA_BYTE, cpu.eax);
	print_asm_template1();
	return 2;
}

make_helper(concat(out_a2d_, SUFFIX)) {
	uint16_t port = reg_w(R_DX);
	pio_write(port, DATA_BYTE, cpu.eax);
	print_asm_template1();
	return 1;
}

#include "cpu/exec/template-end.h"
