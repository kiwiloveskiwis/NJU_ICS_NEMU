#include "cpu/exec/template-start.h"
#define instr lods

make_helper(concat3(instr, _, SUFFIX)) {
	concat(reg_, SUFFIX)(R_EAX) = MEM_R(cpu.esi);
	int inc = cpu.DF ? -DATA_BYTE : DATA_BYTE;
	cpu.esi += inc;
	print_asm_template1();
	return 1;
}
#include "cpu/exec/template-end.h"

