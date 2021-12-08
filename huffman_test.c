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
	char *src_file_name = "proov.txt";//"anton_hansen_tammsaare_tode_ja_oigus_i.txt";// Natuke tummisem, toimub mälu sodimine kahjuks
	char *packed_file_name = "proov.hup";
    char *unpacked_file_name = "proov1.txt";

	FILE *src_file ,*dst_file;

    open_files(&src_file,src_file_name,&dst_file,packed_file_name);
    result = h_pack(src_file, dst_file);
    if (!result)
        printf("OK!:packing\n");
    else{
        printf("FAIL!:packing\n");
        exit(0);
    }
    close_files(src_file,dst_file);

    open_files(&src_file,packed_file_name,&dst_file,unpacked_file_name);
    result = h_unpack(src_file, dst_file);
    if (!result)
        printf("OK!:unpacking\n");
    else{
        printf("FAIL!:unpacking\n");
        exit(0);
    }
    close_files(src_file,dst_file);

    // Kontrollime kas sisu on sama
    src_file = fopen(src_file_name, "rb");
    dst_file = fopen(unpacked_file_name, "rb"); 
    // Loeme puhvrisse, et saaks ka suuremate failidega tegeleda
    size_t buffer_size = 1000;
    char *ptr_1 = malloc(buffer_size * sizeof(char));
    if (ptr_1 == NULL){
        printf("Not enough memory\n");
        return H_ERROR;
    }
    char *ptr_2 = malloc(buffer_size * sizeof(char));
    if (ptr_2 == NULL){
        printf("Not enough memory\n");
        free(ptr_2);
        return H_ERROR;
    }

    //Võrdleme failide pikkusi
    fseek(src_file,0,SEEK_END);
    long filelen_1 = ftell(src_file);
    rewind(src_file);
    fseek(dst_file,0,SEEK_END);
    long filelen_2 = ftell(dst_file);
    rewind(dst_file);
    if(filelen_1 != filelen_2){
        printf("FAIL! files lenght does not match\n");
        printf("\t len1=%ld len2=%ld \n",filelen_1, filelen_2);
        close_files(src_file,dst_file);
        exit(0);
    }

    size_t size_of_elements = sizeof(char); // 1
    size_t bytes_read_1 = 0;
    size_t bytes_read_2 = 0;
    // Võrdleme sisu
    while(filelen_1 > 0) 
    {
        // Loeme puhvri jagu baite sisse
        bytes_read_1 = fread(ptr_1,size_of_elements,buffer_size,src_file);
        bytes_read_2 = fread(ptr_2,size_of_elements,buffer_size,dst_file);
        
        if(bytes_read_1 != bytes_read_2){
            printf("FAIL! Read different amount of bytes\n");
            close_files(src_file,dst_file);
            exit(0);
        }

        // Kontrollime sümboleid
        for (size_t i = 0; i < bytes_read_1 && filelen_1 >= 0; i++, filelen_1--, filelen_2--){
            if(ptr_1[i] != ptr_2[i]){
                for (size_t j = 0; j < i; j++)
                {
                    printf("%c",ptr_1[j]);
                }
                printf("\n");
                
                char c1 = ptr_1[i];
                char c2 = ptr_2[i];
                printf("FAIL! Difference in symbols\n");
                close_files(src_file,dst_file);
                exit(0);
            }
        }
    }
    printf("OK!: File content does not change\n");
    free(ptr_1);
    free(ptr_2);
    close_files(src_file,dst_file);

	return 0;
}
