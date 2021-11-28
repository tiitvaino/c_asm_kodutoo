#include "huffman.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv){
	h_error_t result = H_OK;
	printf("%d\n", argc);
	char *flag = "p";argv[1];
	char *file_1 = "./proov.txt";//argv[2];
	char *file_2 = "./proov.hup";//argv[3];
	
	if (strcmp(flag,"p") == 0){
		result = h_pack(file_1, file_2);
		if (! result)
			printf("%s is packed successfully into %s!\n", file_1, file_2);
			
		else
			printf("There is some error :/");
	}
	else if (strcmp(flag,"u") == 0) {
		result = h_unpack(file_1, file_2);
		if (! result)
			printf("%s is packed successfully into %s!\n", file_1, file_2);
			
		else{
			printf("There is some error :/");
			h_print_error(result);
		}
	}
	else {
		h_print_help();
	}
}