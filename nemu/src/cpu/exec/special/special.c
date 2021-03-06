#include "cpu/exec/helper.h"
#include "monitor/monitor.h"

make_helper(inv) {
	/* invalid opcode */

	uint32_t temp[8];
	temp[0] = instr_fetch(eip, 4);
	temp[1] = instr_fetch(eip + 4, 4);

	uint8_t *p = (void *)temp;
	printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n\n", 
			eip, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

	extern char logo [];
	printf("There are two cases which will trigger this unexpected exception:\n\
1. The instruction at eip = 0x%08x is not implemented.\n\
2. Something is implemented incorrectly.\n", eip);
	printf("Find this eip value(0x%08x) in the disassembling result to distinguish which case it is.\n\n", eip);
	printf("\33[1;31mIf it is the first case, see\n%s\nfor more details.\n\nIf it is the second case, remember:\n\
* The machine is always right!\n\
* Every line of untested code is always wrong!\33[0m\n\n", logo);

	assert(0);
}

extern hwaddr_t page_translate(lnaddr_t addr);
extern lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg);
extern uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg);

make_helper(nemu_trap) {
	print_asm("nemu trap (eax = %d)", cpu.eax);
	unsigned ecx = cpu.ecx;
	unsigned edx = cpu.edx;
	switch(cpu.eax) {
		case 2:
			cpu.eax = cpu.edx;
			while(edx > 0) { // what if for loop?
				printf("%c", swaddr_read(ecx++, 1, R_SS));
				edx--;
			}
			// fwrite((char *)hwaddr, cpu.edx, 1, stdout);
			// printf("%.*s\n", cpu.edx, (char *)hwaddr);  why SegFault?
		   	break;

		default:
			printf("\33[1;31mnemu: HIT %s TRAP\33[0m at eip = 0x%08x\n\n",
					(cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip);
			nemu_state = END;
	}

	return 1;
}

