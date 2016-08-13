#include "common.h"
#include "string.h"

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] __attribute__((used)) = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* TODO: implement a simplified file system here. */

typedef struct {
	bool opened;
	uint32_t offset;
} Fstate;

Fstate files[NR_FILES + 3];

static inline int min(int a, int b) {
	return a < b ? a : b;
}

int fs_open(const char *pathname, int flags) {	
	/* 在我们的实现中可以忽略flags */
	int i;
	for(i = 3; i < NR_FILES + 3; i++) {
		if (!strcmp(file_table[i - 3].name, pathname)) { // found
				files[i].opened = true;
				files[i].offset = 0;
				return i;
		}
	}
	return 0;
}

int fs_read(int fd, void *buf, int len){
	if(fd < 3 || !files[fd].opened) {
		Log("fs_read failed! fd = %d", fd);
		return -1;
	}
	uint32_t start = file_table[fd - 3].disk_offset + files[fd].offset;
	int readlen = min(len, file_table[fd - 3].size - files[fd].offset);
	ide_read(buf, start, readlen);
	return readlen;
}

int fs_write(int fd, void *buf, int len){
	if(fd < 3 || !files[fd].opened) {
		Log("fs_write failed! fd = %d", fd);
		return -1;
	}
	uint32_t start = file_table[fd - 3].disk_offset + files[fd].offset;
	int writelen = min(len, file_table[fd - 3].size - files[fd].offset);
	ide_write(buf, start, writelen);
	return writelen;
}

int fs_lseek(int fd, int offset, int whence) {
	if(fd < 3 || !files[fd].opened) {
		Log("fs_lseek failed! fd = %d", fd);
		return -1;
	}
	switch(whence) {
		case SEEK_SET : files[fd].offset = offset;
		case SEEK_CUR : files[fd].offset += offset;
		case SEEK_END : files[fd].offset = file_table[fd - 3].size - offset;
	}
	return files[fd].offset;

}

int fs_close(int fd){
	if(fd < 3 || !files[fd].opened) {
		Log("fs_close failed! fd = %d", fd);
		return -1;
	}
	files[fd].opened = false;
	return 0;
}





