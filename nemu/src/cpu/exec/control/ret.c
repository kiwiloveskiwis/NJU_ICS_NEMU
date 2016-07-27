#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

make_helper(ret) {
	cpu.eip = swaddr_read(cpu.esp, 4);
	cpu.eip -= 1;
	cpu.esp += 4;
	print_asm("ret");
	return 0;

}
