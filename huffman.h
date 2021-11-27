// huffman.h

typedef enum{
	H_OK,
	H_ERROR, // TODO
} h_error_t;

h_error_t h_pack(
	String src_file_name,
	String dst_file_name
);

h_error_t h_unpack(
	String src_file_name,
	String dst_file_name
);

void h_print_error(
	h_error_t error
);

void h_print_help(
);
