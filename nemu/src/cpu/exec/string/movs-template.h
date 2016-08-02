#include "cpu/exec/template-start.h"
#define instr movs

make_helper(concat3(instr, _, SUFFIX)) {
	int inc;
	DATA_TYPE src;
	inc = cpu.DF ? -DATA_BYTE : DATA_BYTE;
	if(DATA_BYTE == 2 ){ // 8 or 16 // when data_byte = 8?
		src = MEM_R(cpu.gpr[R_SI]._16);
		MEM_W(cpu.gpr[R_DI]._16, src);
		cpu.gpr[R_SI]._16 += inc;
		cpu.gpr[R_DI]._16 += inc;
	}
	else  {
		src = MEM_R(cpu.gpr[R_ESI]._32);
		MEM_W(cpu.gpr[R_DI]._32, src);
		cpu.gpr[R_SI]._32 += inc;
		cpu.gpr[R_DI]._32 += inc;
	}
	print_asm("movs %%ds:(%%esi),%%es:(%%edi)");
	return 1;
}
#include "cpu/exec/template-end.h"



