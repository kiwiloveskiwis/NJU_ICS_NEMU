
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };


/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	union{
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];
		/* Do NOT change the order of the GPRs' definitions. */
		struct {
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		};
	};
	swaddr_t eip;

	union {
		uint32_t EFLAGS;
		struct {
			uint32_t CF		:1;
			uint32_t		:1;
			uint32_t PF		:1;
			uint32_t		:1;
			uint32_t AF		:1;
			uint32_t 		:1;
			uint32_t ZF		:1;
			uint32_t SF		:1;
			uint32_t TF		:1;
			uint32_t IF		:1;
			uint32_t DF		:1;
			uint32_t OF		:1;
			uint32_t IOPL	:2;
			uint32_t NT		:1;
			uint32_t		:1;
			uint32_t RF		:1;
			uint32_t VM		:1; // 18 bits taken
		};
	};





} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

#define BIT_MASK(bytes) ((0x100 << (8 * (bytes - 1))) - 1)

static inline void update_PSZ(size_t len, uint32_t result) {
	int i = result ^ (result >> 4); 
	i ^= i >> 2;
	cpu.PF = ~(i ^ (i >> 1));
	cpu.SF = result >> ((len << 3) - 1);
	cpu.ZF = !(result & BIT_MASK(len));
}

static inline uint32_t update_COPZS(size_t len, int64_t result, uint64_t uresult) {
	cpu.CF = uresult >> (len * 8);
	cpu.OF = (result >> (len * 8 - 1)) && ~(result >> (len * 8 - 1));
	uint32_t data = (uint32_t)result & BIT_MASK(len);
	update_PSZ(len, data);
	return data;
}

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
