#include "huffman.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void open_files(FILE **src_file, char *src_file_name, FILE **dst_file, char *dst_file_name)
{
    *src_file = fopen(src_file_name, "rb");
    *dst_file = fopen(dst_file_name, "wb"); // if file does not exists then creates one, 
                                                // else deletes content
	if (*src_file == NULL || *dst_file == NULL)
	{
		if (*src_file == NULL && *dst_file == NULL){
			printf("Problem with opening files.\n");
			printf("src= %s and dst= %s!\n", src_file_name, dst_file_name);
		}
		else if(*src_file == NULL){
			printf("Problem with opening src file.\n");
			printf("src= %s and dst= %s!\n", src_file_name, dst_file_name);
    		int dst_file_error = fclose(*dst_file);
			if (dst_file_error != 0)
				printf("There is some error, when closing dst file\nerror=%d\n",dst_file_error);
		}
		else{//if(*dst_file == NULL)
			printf("Problem with opening dst file.\n");
			printf("src= %s and dst= %s!\n", src_file_name, dst_file_name);
			int src_file_error = fclose(*src_file);
			if (src_file_error != 0)
				printf("There is some error, when closing src file\nerror=%d\n",src_file_error);
		}
		exit(0);
	}

}

static void close_files(FILE *src_file, FILE *dst_file)
{
    int src_file_error = fclose(src_file);
    int dst_file_error = fclose(dst_file);
	if (src_file_error != 0 || dst_file_error != 0)
	{
		if (src_file_error != 0 && dst_file_error != 0)
			printf("There is some error, when closing files\nsrcerror=%d dsterror=%d\n",src_file_error,dst_file_error);
		else if (src_file_error != 0)
			printf("There is some error, when closing src file\nerror=%d\n",src_file_error);
		else //if (dst_file_error != 0)
			printf("There is some error, when closing dst file\nerror=%d\n",dst_file_error);
		exit(0);
	}
}

int main(int argc, char **argv)
{
	h_error_t result = H_OK;
	if (argc < 4) return 0;
	char *flag = argv[1];
	char *src_file_name = argv[2];
	char *dst_file_name = argv[3];

	FILE *src_file ,*dst_file;

	if (strcmp(flag,"p") == 0){
		open_files(&src_file,src_file_name,&dst_file,dst_file_name);
		result = h_pack(src_file, dst_file);
		if (!result)
			printf("%s is packed successfully into %s!\n", src_file_name, dst_file_name);
			
		else
			printf("There is some error :/");
		close_files(src_file,dst_file);
	}
	else if (strcmp(flag,"u") == 0) {
		open_files(&src_file,src_file_name,&dst_file,dst_file_name);
		result = h_unpack(src_file, dst_file);
		if (!result)
			printf("%s is unpacked successfully into %s!\n", src_file_name, dst_file_name);
			
		else{
			printf("There is some error :/");
		}
		close_files(src_file,dst_file);
	}
	else {
		printf("hufpakk help\n\n");
		printf("Run as followed: \n./hufpakk src_file_address dst_file_address\n\n");
		printf("Prints error messages if someting is wrong.\n");
	}
	return 0;
}
