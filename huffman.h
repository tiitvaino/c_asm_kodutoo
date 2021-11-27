// huffman.h

typedef enum{
	H_OK,
	H_ERROR, // TODO
} h_error_t;

h_error_t h_pack(
	char * src_file_name,
	char * dst_file_name
);

h_error_t h_unpack(
	char * src_file_name,
	char * dst_file_name
);

void h_print_error(
	h_error_t error
);

void h_print_help(
);
