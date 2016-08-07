#include "common.h"
#include "cpu/reg.h"
#include "../../lib-common/x86-inc/mmu.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t cache_read(hwaddr_t addr, size_t len);
void cache_write(hwaddr_t addr, size_t len, uint32_t data);

static lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg);
static void load_sreg_cache(uint8_t sreg);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	cache_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	lnaddr_write(lnaddr, len, data);
}


static lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {
	lnaddr_t result = addr;
	if(cpu.cr0.protect_enable) {
		if(!cpu.sr[sreg].cached)
			load_sreg_cache(sreg);
		result += cpu.sr[sreg].base;
	}
	return result;
}

static void load_sreg_cache(uint8_t sreg) {
	lnaddr_t seg_desc_addr = cpu.gdtr_base + 8 * cpu.sr[sreg].index;
	SegDesc desc = *(SegDesc *)seg_desc_addr;
	
	cpu.sr[sreg].base = (desc.base_31_24 << 24) + (desc.base_23_16 << 16) + desc.base_15_0;
	cpu.sr[sreg].limit = desc.limit_15_0 + (desc.limit_19_16 << 16);
	cpu.sr[sreg].cached = true;	
}


