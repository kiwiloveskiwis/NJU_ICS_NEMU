#include<stdio.h>
#include<stdlib.h>
#include<nemu.h>
#include<time.h>
#include<misc.h>
#include "../../lib-common/x86-inc/mmu.h"

#define NR_TLB 64
#define TAG_WIDTH 20

typedef struct{
	uint32_t tag : TAG_WIDTH;
	bool valid;
	uint32_t page_addr: 20; 
}TLB;

TLB tlb[NR_TLB];

void init_tlb(){
	int i = 0;
	for(i = 0; i < NR_TLB; i++) {
		tlb[i].valid = false;	
	}
}

hwaddr_t tlb_read(lnaddr_t addr) {
	assert(cpu.cr0.protect_enable && cpu.cr0.paging) ;
	uint32_t tag = addr >> 12;
	uint32_t offset = addr & 0xfff;
	int i;
	for(i = 0; i < NR_TLB; i++) {
		if(tlb[i].valid && tlb[i].tag == tag) {
			return (tlb[i].page_addr << 12) + offset;
		}
	}
	for(i = 0; i < NR_TLB; i++) { // Not found
		if(!tlb[i].valid) break;
	}
	if (i >= NR_TLB) { // No empty
		srand(time(0));
		i = rand() % NR_TLB;
	}	
	tlb[i].valid = true;
	tlb[i].tag = tag;

	uint32_t dir = (addr >> 22) & 0x3ff;
	uint32_t page = (addr >> 12) & 0x3ff;
	hwaddr_t page_directory_addr = (cpu.cr3.page_directory_base << 12) + (dir << 2); 
	// 4 bytes per dir
	PDE page_directory;
	page_directory.val = (uint32_t)hwaddr_read(page_directory_addr, 4);
	Assert(page_directory.present, "%x\n", cpu.eip);

	hwaddr_t page_table_addr = (page_directory.page_frame << 12) + (page << 2);
	PTE  page_table;
	page_table.val = (uint32_t)hwaddr_read(page_table_addr, 4);
	Assert(page_table.present,  "%x\n", cpu.eip);

	tlb[i].page_addr = page_table.page_frame ;
	return (tlb[i].page_addr << 12) + offset;

}

#undef TAG_WIDTH
#undef NR_TLB
