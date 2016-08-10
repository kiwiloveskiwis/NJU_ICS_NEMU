#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;
	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f; // all the same for exec in testcases
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	/* Load each program segment */
	int i = 0;
	ph = (Elf32_Phdr *) (void *)(buf + elf->e_phoff); 
	for(; i < elf->e_phnum; i++) {
		ph = (Elf32_Phdr *) (void *)(buf + elf->e_phoff + i * elf->e_phentsize); 

		if(ph->p_type == PT_LOAD) {
			uint32_t loaded = 0;
			uint32_t unit = 1 << 12;
			uint32_t start = ph->p_vaddr & ~0xfff;
			uint32_t end = (ph->p_memsz + ph->p_vaddr);
			while(end - start - loaded > unit) {
				uint32_t hwaddr = mm_malloc(start + loaded, unit);
				ramdisk_read((uint8_t *)hwaddr, ph->p_offset, unit);
				loaded += unit;
			}

			/*  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
			 *  LOAD           0x000000 0x08048000 0x08048000 0x001f0 0x001f0 R E 0x1000
			 *  LOAD           0x000200 0x08049200 0x08049200 0x1d4c0 0x27100 RW  0x1000
			 */
			break;	
			

			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			// ramdisk_read((uint8_t *)ph->p_vaddr, ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);  
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
			// memset((void *)(ph->p_vaddr + ph->p_filesz), 0, (ph->p_memsz - ph->p_filesz));



#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif
	assert(entry == 0x80480a6); // 0x8048xxx 
	return entry;
}
