#include "cpu/exec/template-start.h"
#define instr movs

make_helper(concat3(instr, _, SUFFIX)) {
	int inc;
	DATA_TYPE src;
	inc = cpu.DF ? -DATA_BYTE : DATA_BYTE;
		src = MEM_R(cpu.gpr[R_ESI]._32);
		MEM_W(cpu.gpr[R_DI]._32, src);
		cpu.gpr[R_SI]._32 += inc;
		cpu.gpr[R_DI]._32 += inc;
	return 1;
}
#include "cpu/exec/template-end.h"



