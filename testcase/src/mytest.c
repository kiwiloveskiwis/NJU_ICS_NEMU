#include "trap.h"
#include <string.h>



int main() {
    char s1[] = "a";
    char * s2 = "b";

    char str[20]; // = s1
    char * test = strcpy(str, s1);
    char * s= strcat(test, s2); //strcmp(s, "ab") == 0
    
    int c = strcmp(s, "ab");
    nemu_assert(c == 0);	


	HIT_GOOD_TRAP;
	return 0;
}

