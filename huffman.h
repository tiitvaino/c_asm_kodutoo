// huffman.h
#include <stdio.h>

typedef enum{
	H_OK,
	H_ERROR,
} h_error_t;

h_error_t h_pack(
	FILE * src_file,
	FILE * dst_file
);

h_error_t h_unpack(
	FILE * src_file,
	FILE * dst_file
);
