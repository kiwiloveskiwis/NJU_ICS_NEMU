#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

make_helper_v(pop_r)
make_helper_v(pop_rm)

make_helper(popad) {
#define popto(reg) reg = swaddr_read(cpu.esp, 4, R_SS); cpu.esp +=4;
	popto(cpu.edi)
	popto(cpu.esi)
	popto(cpu.ebp)
	cpu.esp += 4; // throw
	popto(cpu.ebx)
	popto(cpu.edx)
	popto(cpu.ecx)
	popto(cpu.eax)
#undef popto
	return 1;
}	
