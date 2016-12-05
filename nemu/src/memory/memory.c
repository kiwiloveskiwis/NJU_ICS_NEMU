#include "common.h"
#include "cpu/reg.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "../include/device/mmio.h"

extern uint32_t dram_read(hwaddr_t addr,size_t len);
extern void dram_write(hwaddr_t addr,size_t len, uint32_t data);

uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);
int32_t hwaddr_read(hwaddr_t addr, size_t len);
hwaddr_t tlb_read(lnaddr_t);

/* Memory accessing interfaces */

hwaddr_t page_translate(lnaddr_t addr) {
	if(!cpu.cr0.protect_enable || !cpu.cr0.paging) return addr;
	else {
		hwaddr_t hwaddr = tlb_read(addr);
		return hwaddr;
	}
}
int32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int mm = is_mmio(addr);
	if(mm != -1) return mmio_read(addr, len, mm);
	else return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	// else return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int mm = is_mmio(addr);
	if(mm != -1) mmio_write(addr, len, data, mm);
	else dram_write(addr, len, data);
	// else cache_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	hwaddr_t hwaddr = page_translate(addr);
	return hwaddr_read(hwaddr,len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	hwaddr_t hwaddr = page_translate(addr);
	hwaddr_write(hwaddr,len,data);
}

void load_seg_cache(uint8_t sreg) {
	lnaddr_t off_addr = (lnaddr_t)cpu.gdtr_base + 8 * cpu.sr[sreg].index; 
	SegDesc d;
	uint32_t *tmp = (uint32_t *)&d;
	*tmp = lnaddr_read( off_addr, 4);
	tmp++;
	*tmp = lnaddr_read( off_addr+4, 4);

	cpu.sr[sreg].base = d.base_15_0 + (d.base_23_16 << 16) + (d.base_31_24 << 24);
	cpu.sr[sreg].limit = d.limit_15_0 + (d.limit_19_16 << 16);
	cpu.sr[sreg].cached = true;
	return;
}
lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg) {
	if(cpu.cr0.protect_enable == 1) {
		if(!cpu.sr[sreg].cached)
			load_seg_cache(sreg);
		return addr + cpu.sr[sreg].base;
	}
	else return (lnaddr_t)addr;
 }

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, sreg);
	lnaddr_write(lnaddr, len, data);
}


