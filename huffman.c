// huffman.c
#include "huffman.h"
#include "dictionary.h"
//#include <string.h>
#include <stdio.h>
//#include <stdbool.h>
#include <stdlib.h>

h_error_t h_pack(FILE *src_file, FILE *dst_file){
    printf("h_pack \n");// TODO delete later
    dict_dict_t * dict = NULL;
    int n = 1000;
    char buf[1000];

    fgets(buf, n, (FILE*)src_file);
    printf("%s\n",buf);
    
    return H_OK;
}

h_error_t h_unpack(FILE *src_file, FILE *dst_file){
    printf("h_unpack\n"); // TODO delete later

    return H_OK;
}

void h_print_error(h_error_t error){
    printf("h_print_error %d\n", error); // TODO delete later
}

void h_print_help() {
    printf("h_print_help\n");// TODO delete later
}

