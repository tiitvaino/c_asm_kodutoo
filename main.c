#include "huffman.h"
#include <string.h>
#include <stdio.h>

static void open_files(FILE *src_file, char *src_file_name, FILE *dst_file, char *dst_file_name){
    src_file = fopen(src_file_name, "r");
    dst_file = fopen(dst_file_name, "w"); // if does not exists then creates one, 
                                                // else deletes content
	
}

static void *close_files(FILE *src_file, FILE *dst_file){
    int src_file_error = fclose(src_file);
    int dst_file_error = fclose(dst_file);
}

int main(int argc, char **argv){
	h_error_t result = H_OK;
	printf("%d\n", argc);
	char *flag = "p";argv[1];
	char *src_file_name = "./proov.txt";//argv[2];
	char *dst_file_name = "./proov.hup";//argv[3];

	FILE *src_file ,*dst_file;
    
	
	if (strcmp(flag,"p") == 0){
		open_files(src_file,src_file_name,dst_file,dst_file_name);
		result = h_pack(src_file, dst_file);
		if (! result)
			printf("%s is packed successfully into %s!\n", src_file_name, dst_file_name);
			
		else
			printf("There is some error :/");
		close_files(src_file,dst_file);
	}
	else if (strcmp(flag,"u") == 0) {
		result = h_unpack(src_file, dst_file);
		if (! result)
			printf("%s is packed successfully into %s!\n", src_file_name, dst_file_name);
			
		else{
			printf("There is some error :/");
			h_print_error(result);
		}
	}
	else {
		h_print_help();
	}
	return 0;
}
