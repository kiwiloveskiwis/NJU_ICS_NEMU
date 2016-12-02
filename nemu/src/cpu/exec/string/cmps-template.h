#include "cpu/exec/template-start.h"
#define instr cmps

make_helper(concat3(instr, _, SUFFIX)) {
	int inc;
	inc = cpu.DF ? -DATA_BYTE : DATA_BYTE;
	cpu.esi += inc; 
	cpu.edi += inc;
	update_sub(MEM_R(cpu.esi), MEM_R(cpu.edi));
	

	// when data_byte = 8?
	return 1;
}
#include "cpu/exec/template-end.h"


