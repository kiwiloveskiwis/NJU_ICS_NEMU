#include "trap.h"
#include <string.h>

char *s[] = {
	"a", 
	"aa",
	"aaa",
	", World!\n",
	"Hello, World!\n",
};

char str1[] = "Hello";
char str[20];

int main() {
		
	nemu_assert(strcmp( strcat(strcpy(str, str1), s[3]), s[4]) == 0);


	return 0;
}
