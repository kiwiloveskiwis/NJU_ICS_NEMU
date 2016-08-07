#include "cpu/exec/template-start.h"
#define instr stos

make_helper(concat3(instr, _, SUFFIX)) {
	 int inc;
	 inc = cpu.DF ? -DATA_BYTE : DATA_BYTE;
	 DATA_TYPE t = REG(R_EAX);
	 MEM_W(cpu.edi, t, R_ES);
	 cpu.edi += inc;
	 return 1;
}
#include "cpu/exec/template-end.h"

