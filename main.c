# include "huffman.h"
# include <stdio.h>

int main(void *args){
	h_error_t result = H_OK;
	String flag = args[1];
	String file_1 = args[2];
	String file_2 = args[3];
	
	if (equal(flag,"p")){
		result = h_pack(file_1, file_2);
		if (! result)
			print_f("%s is packed successfully into %s!\n", file_1, file_2);
			
		else
			print_f("There is some error :/");
	}
	else if (equal(flag,"u")) {
		result = h_unpack(file_1, file_2);
		if (! result)
			print_f("%s is packed successfully into %s!\n", file_1, file_2);
			
		else{
			print_f("There is some error :/");
			h_print_error(result);
		}
	}
	else {
		h_print_help();
	}
}