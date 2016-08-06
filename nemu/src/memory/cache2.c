#include<stdio.h>
#include<stdlib.h>
#include<nemu.h>
#include<misc.h>
#include<time.h>

#define BLOCK_WIDTH 6
#define WAY_WIDTH 4
#define SET_WIDTH 12
#define TAG_WIDTH (32 - SET_WIDTH - BLOCK_WIDTH)

#define BLOCK_SIZE (1 << BLOCK_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)

#define CACHE_LEN BLOCK_SIZE
#define CACHE_MASK (CACHE_LEN - 1)// 111111

extern uint32_t dram_read(hwaddr_t addr,size_t len);
extern void dram_write(hwaddr_t addr,size_t len, uint32_t data);

typedef struct{
	uint32_t valid	: 1;
	uint32_t dirty	: 1;
	uint32_t tag  	: TAG_WIDTH;
	uint8_t content[BLOCK_SIZE];

}Cache_Block_2;


typedef union {
	uint32_t value;
	struct{
		uint32_t blockidx : BLOCK_WIDTH;
		uint32_t setidx : SET_WIDTH;
		uint32_t tag : TAG_WIDTH;
	};
}Cache_Addr_2;

Cache_Block_2 caches2[NR_SET][NR_WAY];


void init_cache_2() {
	int i = 0, j = 0;
	for(; i < NR_SET; i++)
		for(; j < NR_WAY; j++)
			caches2[i][j].valid = false;
}

bool print_cache_2(hwaddr_t addr) {
	Cache_Addr_2 caddr ;
	caddr.value = addr & ~CACHE_MASK; // discard the last 6 bits
	uint32_t set = caddr.setidx;
	int i = 0;
	while(caches2[set][i].valid && i < NR_WAY) {
		if (caches2[set][i].tag == caddr.tag ) {
			int j;
			printf("ADDR %x :", caddr.value);
			for(j = 0; j < CACHE_LEN; j++) {
				if(j && j % 16 == 0) printf("\nADDR %x :", caddr.value + j);
				else if(j && j % 4 == 0) printf(" ");
				printf("%02hhX", *(caches2[set][i].content + j));
			}
			printf("\n");
			return true;
		}
	}
	return false;
}
static void block_read_2(hwaddr_t addr, void *data) {
	Cache_Addr_2 caddr ;
	caddr.value = addr & ~CACHE_MASK; // discard the last 6 bits
	uint32_t set = caddr.setidx;
	//uint32_t offset = addr & CACHE_MASK;  // the last 6 bits
	int i = 0;
	for(i = 0; i < NR_WAY; i++) {
		if (caches2[set][i].valid && caches2[set][i].tag == caddr.tag ) {
			memcpy(data, caches2[set][i].content, BLOCK_SIZE);// found
			return;
		}
		i++;
	}
	if(i >= NR_WAY) {	// no empty slots
		srand(time(0));
		i = rand() % NR_WAY;
	}
	caches2[set][i].valid = true;
	caches2[set][i].dirty = false;
	caches2[set][i].tag = caddr.tag;

	// LOADING DATA
	int j;
	uint32_t loading_temp[BLOCK_SIZE >> 2];
	for(j = 0; j < (BLOCK_SIZE >> 2); j++) {
		loading_temp[j] = dram_read(caddr.value + 4 * j, 4);
	}
	memcpy(caches2[set][i].content, loading_temp, BLOCK_SIZE);
	memcpy(data, caches2[set][i].content , BLOCK_SIZE);
}


uint32_t cache_read_2(hwaddr_t addr, size_t len) { // len is handled in memory.c
	uint32_t offset = addr & CACHE_MASK;  // 0 	~ 111111
	uint8_t temp[2 * BLOCK_SIZE];
	block_read_2(addr, temp);
	if(offset + len > BLOCK_SIZE) {
		block_read_2(addr + BLOCK_SIZE, temp + BLOCK_SIZE);
	}

	uint32_t result = dram_read(addr, len) &  (~0u >> ((4 - len) << 3));
	uint32_t mine = unalign_rw(temp + offset, 4) & (~0u >> ((4 - len) << 3));
	assert(mine == result);
	return unalign_rw(temp + offset, 4) & (~0u >> ((4 - len) << 3));
}

static void block_write_2(hwaddr_t addr, void *data, uint8_t *mask) {
	Cache_Addr_2 temp;
	temp.value = addr & ~CACHE_MASK;
	uint32_t tag = temp.tag;
	uint32_t set = temp.setidx;

	int i = 0;
	for(i = 0; i < NR_WAY;i++) {
		if(caches2[set][i].valid && caches2[set][i].tag == tag){
			memcpy_with_mask(caches2[set][i].content, data, BLOCK_SIZE, mask);
			caches2[set][i].dirty = true;
			return;
		}
	}
	// NOT FOUND
	for(i = 0; i < NR_WAY;i++) {
		if(!caches2[set][i].valid) break;
	}
	if(i >= NR_WAY) {	// no empty slots
		srand(time(0));
		i = rand() % NR_WAY;
		if (caches2[set][i].dirty) {
			int j;
			Cache_Addr_2 replace;
			replace.tag = caches2[set][i].tag;
			replace.setidx = set;
			for(j = 0; j < (BLOCK_SIZE >> 2); j++)
				dram_write(replace.value, 4, *(uint32_t *)(caches2[set][i].content + 4 * j));
		}
	} 

	caches2[set][i].valid = true;
	caches2[set][i].dirty = false;
	caches2[set][i].tag = temp.tag;

	// LOADING DATA
	int j;
	uint32_t loading_temp[BLOCK_SIZE >> 2];
	for(j = 0; j < (BLOCK_SIZE >> 2); j++) { 
		loading_temp[j] = dram_read(temp.value + 4 * j, 4);
	}
	memcpy(caches2[set][i].content, loading_temp, BLOCK_SIZE);
	memcpy(data, caches2[set][i].content , BLOCK_SIZE);

}

void cache_write_2(hwaddr_t addr, size_t len, uint32_t data) {

	uint32_t offset = addr & CACHE_MASK;  // 0 	~ 111111
	uint8_t temp[2 * BLOCK_SIZE];
	uint8_t mask[2 * BLOCK_SIZE];
	memset(mask, 0, 2 * BLOCK_SIZE);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	block_write_2(addr, temp, mask);

	if(offset + len > BLOCK_SIZE) {
		block_write_2(addr + BLOCK_SIZE, temp + BLOCK_SIZE, mask + BLOCK_SIZE);
	}
	dram_write(addr, len, data);
}


#undef BLOCK_WIDTH
#undef WAY_WIDTH
#undef SET_WIDTH
#undef TAG_WIDTH

#undef BLOCK_SIZE
#undef NR_SET
#undef NR_WAY

#undef CACHE_LEN
#undef CACHE_MASK




