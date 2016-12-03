#include "cpu/exec/template-start.h"
#define instr scas

make_helper(concat3(instr, _, SUFFIX)) {
	update_sub(cpu.eax, MEM_R(cpu.edi, R_ES));

	int inc = cpu.DF ? -DATA_BYTE : DATA_BYTE;
	cpu.edi += inc;
	print_asm("scas" str(SUFFIX));
	return 1;
}
#include "cpu/exec/template-end.h"

