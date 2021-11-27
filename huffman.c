// huffman.c
#include "huffman.h"
#include "dictionary.h"
//#include <string.h>
#include <stdio.h>
//#include <stdbool.h>
//#include <stdlib.h>



h_error_t h_pack(char *src_file_name, char *dst_file_name){
    printf("h_pack %s to %s\n", src_file_name, dst_file_name);// TODO delete later
    dict_dict_t * dict = NULL;
    

}

h_error_t h_unpack(char *src_file_name, char *dst_file_name){
    printf("h_unpack %s to %s\n", src_file_name, dst_file_name);// TODO delete later
}

void h_print_error(h_error_t error){
    printf("h_print_error %d\n", error);// TODO delete later
}

void h_print_help() {
    printf("h_print_help\n");// TODO delete later
}

