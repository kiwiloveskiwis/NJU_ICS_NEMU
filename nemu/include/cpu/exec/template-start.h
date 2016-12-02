#include "cpu/exec/helper.h"

#if DATA_BYTE == 1

#define SUFFIX b
#define DATA_TYPE uint8_t
#define DATA_TYPE_S int8_t

#elif DATA_BYTE == 2

#define SUFFIX w
#define DATA_TYPE uint16_t
#define DATA_TYPE_S int16_t

#elif DATA_BYTE == 4

#define SUFFIX l
#define DATA_TYPE uint32_t
#define DATA_TYPE_S int32_t

#else

#error unknown DATA_BYTE

#endif

#define REG(index) concat(reg_, SUFFIX) (index)
#define REG_NAME(index) concat(regs, SUFFIX) [index]

#define MEM_R(addr) swaddr_read(addr, DATA_BYTE)
#define MEM_W(addr, data) swaddr_write(addr, DATA_BYTE, data)

#define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)


#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) - 1))
#define BIT_MASK(bytes) ((0x100 << (8 * (bytes - 1))) - 1)

#define update_adc(num1, num2, cf)\
	update_COPZS(DATA_BYTE, (int64_t) (DATA_TYPE_S) num1 + (DATA_TYPE_S) num2 + cf, \
			(uint64_t) num1 + num2 + cf)

#define update_sbb(num1, num2, cf) \
	update_COPZS(DATA_BYTE, (int64_t) (DATA_TYPE_S) num1 - (DATA_TYPE_S) num2 - cf, \
			(uint64_t) num1 - num2 - cf)
#define update_sub(num1, num2) update_sbb(num1, num2, 0)
#define update_add(num1, num2) update_adc(num1, num2, 0)

#define update_PZS(result) int eventest = (DATA_TYPE) result & 0xff, count = 0; \
			 do{ count += eventest & 1; } while(eventest >>= 1);  \
			cpu.PF = !(count & 1); cpu.ZF = (result == 0); \
			cpu.SF = MSB(result);

#ifndef EFLAGS_FUNC
#define EFLAGS_FUNC
static inline void update_PSZ(size_t len, uint32_t result) {
	int i = result ^ (result >> 4); 
	i ^= i >> 2;
	cpu.PF = ~(i ^ (i >> 1));
	cpu.SF = MSB(result);
	cpu.ZF = !(result & BIT_MASK(len));
}

static inline uint32_t update_COPZS(size_t len, int64_t result, uint64_t uresult) {
	cpu.CF = uresult >> (len * 8);
	cpu.OF = result >> (len * 8 - 1) && !(result >> (len * 8 - 1));
	uint32_t data = (uint32_t)result & BIT_MASK(len);
	update_PSZ(len, data);
	return data;
}
#endif
