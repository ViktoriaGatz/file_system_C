#include <stdio.h>

int main(int argc, char **argv) {
	int i = 0;
	printf("Программа запущена и получила строку : ");
	while(argv[i] != NULL) {
  		printf("%s ",argv[i]);
		i++;
	}
	printf("\n");
//	while(1){}
  	return 0;
}
