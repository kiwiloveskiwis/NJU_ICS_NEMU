#include<stdio.h>
#include<stdlib.h>
#include<nemu.h>
#include<misc.h>
#include<time.h>

#define BLOCK_WIDTH 6
#define WAY_WIDTH 3
#define SET_WIDTH 7
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
	uint32_t tag  	: TAG_WIDTH;
	uint8_t content[BLOCK_SIZE];

}Cache_Block;


typedef union {
	uint32_t value;
	struct{
		uint32_t blockidx : BLOCK_WIDTH;
		uint32_t setidx : SET_WIDTH;
		uint32_t tag : TAG_WIDTH;
	};

}Cache_Addr;

Cache_Block caches[NR_SET][NR_WAY];


void init_cache() {
	int i = 0, j = 0;
	for(; i < NR_SET; i++) 
		for(; j < NR_WAY; j++) 
			caches[i][j].valid = false;
}

static void block_read(hwaddr_t addr, void *data) {
	Cache_Addr caddr ;
	caddr.value = addr & ~CACHE_MASK; // discard the last 6 bits
	Log("%x, %x, %x", addr, ~CACHE_MASK, addr & ~CACHE_MASK);

	uint32_t set = caddr.setidx;
	uint32_t offset = addr & CACHE_MASK;  // the last 6 bits
	int i = 0;
	while(caches[set][i].valid && i < NR_WAY) {
		if (caches[set][i].tag == caddr.tag ) {
			memcpy(data, caches[set][i].content + offset, BLOCK_SIZE);// found
			return;
		}
		i++;
	}
	if(i >= NR_WAY) {	// no empty slots
		srand(time(0));
		i = rand() % 8;
	}
	caches[set][i].valid = true;
	caches[set][i].tag = caddr.tag;
	Log("%x, %x, %x", addr, caddr.value, caddr.value + BLOCK_SIZE);
	// LOADING DATA
	int j;
	uint32_t loading_temp[BLOCK_SIZE >> 2];
	for(j = 0; j < (BLOCK_SIZE >> 2); j++) 
		loading_temp[j] = dram_read(caddr.value + 4 * j, 4);
	Log("Here!");
	memcpy(caches[set][i].content, loading_temp, BLOCK_SIZE);
	memcpy(data, caches[set][i].content + offset, BLOCK_SIZE);
}


uint32_t cache_read(hwaddr_t addr, size_t len) { // len is handled in memory.c
	Log("reading cache...");
	uint32_t offset = addr & CACHE_MASK;  // 0 	~ 111111
	uint8_t temp[2 * BLOCK_SIZE];

	block_read(addr, temp);
	if(offset + len > BLOCK_SIZE) {
		block_read(addr + BLOCK_SIZE, temp + BLOCK_SIZE);
	}

	return unalign_rw(temp + offset, 4) & (~0u >> ((4 - len) << 3));
	Log("reading Done.");
}




static void block_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Log("writing cache...");
	Cache_Addr temp;
	temp.value = addr & ~CACHE_MASK;
	uint32_t tag = temp.tag;
	uint32_t set = temp.setidx;

	int i = 0;
	for(;i < NR_WAY;i++) {
		if(caches[set][i].valid && caches[set][i].tag == tag){
			memcpy_with_mask(caches[set][i].content, data, BLOCK_SIZE, mask);
			return;
		}
	}
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {

	uint32_t offset = addr & CACHE_MASK;  // 0 	~ 111111
	uint8_t temp[2 * BLOCK_SIZE];
	uint8_t mask[2 * BLOCK_SIZE];    
	memset(mask, 0, 2 * BLOCK_SIZE);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	block_write(addr, temp, mask);

	if(offset + len > BLOCK_SIZE) {
		block_write(addr + BLOCK_SIZE, temp + BLOCK_SIZE, mask + BLOCK_SIZE);
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


