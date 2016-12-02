#include "cpu/exec/template-start.h"
#define instr cmps

make_helper(concat3(instr, _, SUFFIX)) {
	int inc;
	DATA_TYPE src, dest;
	inc = cpu.DF ? -DATA_BYTE : DATA_BYTE;
	if(DATA_BYTE != 4) { // 8 or 16
		src = MEM_R(cpu.gpr[R_SI]._16);
	   	dest = MEM_R(cpu.gpr[R_DI]._16);
		cpu.gpr[R_SI]._16 += inc;
		cpu.gpr[R_DI]._16 += inc;
	}
	else {
		src = MEM_R(cpu.gpr[R_ESI]._32);
	   	dest = MEM_R(cpu.gpr[R_EDI]._32);
		cpu.gpr[R_SI]._32 += inc;
		cpu.gpr[R_DI]._32 += inc;
	}
	
	DATA_TYPE result = src - dest;
	cpu.CF = ((unsigned int)src < (unsigned int)dest ); 
	cpu.OF = (MSB(dest) ^ MSB(src)) && (MSB(dest) == MSB(result));
	update_PZS(DATA_BYTE, result);

	// when data_byte = 8?
	return 1;
}
#include "cpu/exec/template-end.h"


