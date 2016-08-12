#include "monitor/monitor.h"
#include "monitor/watchpoint.h"
#include "cpu/helper.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "device/i8259.h"
#include <setjmp.h>

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the ``si'' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

int nemu_state = STOP;

int exec(swaddr_t);
extern bool checkWP();
extern void load_seg_cache(uint8_t sreg);

char assembly[80];
char asm_buf[128];

/* Used with exception handling. */
jmp_buf jbuf;

void raise_intr(uint8_t NO) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 *	 * That is, use ``NO'' to index the IDT. */
	cpu.esp-=4;
	swaddr_write(cpu.esp,4,(uint32_t)cpu.EFLAGS, R_SS);

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,(uint32_t)(uint16_t)cpu.sr[R_CS].val, R_SS);

	cpu.esp-=4;
	swaddr_write(cpu.esp,4,(uint32_t)cpu.eip,R_SS);

	uint32_t gdaddr = cpu.idtr_base + NO * 8; // 8 bytes
	GateDesc gd;

	uint32_t *tmp = (uint32_t *)&gd;
	*tmp = lnaddr_read(gdaddr, 4);
	tmp++;
	*tmp = lnaddr_read(gdaddr + 4, 4);

	Assert(gd.present, "Gate Desc Invalid! eip == 0x%x", cpu.eip);
	uint32_t intr_addr = (gd.offset_31_16 << 16) + gd.offset_15_0;
	cpu.sr[R_CS].val = gd.segment;
	load_seg_cache(R_CS);// Watch out!!!

	cpu.eip = intr_addr;
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}

void print_bin_instr(swaddr_t eip, int len) {
	int i;
	int l = sprintf(asm_buf, "%8x:   ", eip);
	for(i = 0; i < len; i ++) {
		l += sprintf(asm_buf + l, "%02x ", instr_fetch(eip + i, 1));
	}
	sprintf(asm_buf + l, "%*.s", 50 - (12 + 3 * len), "");
}

/* This function will be called when an `int3' instruction is being executed. */
void do_int3() {
	printf("\nHit breakpoint at eip = 0x%08x\n", cpu.eip);
	nemu_state = STOP;
}

/* Simulate how the CPU works. */
void cpu_exec(volatile uint32_t n) {
	if(nemu_state == END) {
		printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
		return;
	}
	nemu_state = RUNNING;

#ifdef DEBUG
	volatile uint32_t n_temp = n;
#endif

	setjmp(jbuf);

	for(; n > 0; n --) {
#ifdef DEBUG
		swaddr_t eip_temp = cpu.eip;
		if((n & 0xffff) == 0) {
			/* Output some dots while executing the program. */
			fputc('.', stderr);
		}
#endif

		/* Execute one instruction, including instruction fetch,
		 * instruction decode, and the actual execution. */
		int instr_len = exec(cpu.eip);

		cpu.eip += instr_len;

#ifdef DEBUG
		print_bin_instr(eip_temp, instr_len);
		strcat(asm_buf, assembly);
		Log_write("%s\n", asm_buf);
		if(n_temp < MAX_INSTR_TO_PRINT) {
			printf("%s\n", asm_buf);
		}
#endif
		bool change = checkWP();
		if(change) nemu_state = STOP;

		if(nemu_state != RUNNING) { return; }

#ifdef HAS_DEVICE
		extern void device_update();
		device_update();
#endif
		if(cpu.INTR & cpu.IF) {
			uint32_t intr_no = i8259_query_intr();
			i8259_ack_intr();
			raise_intr(intr_no);
		}
	}

	if(nemu_state == RUNNING) { nemu_state = STOP; }
}
