// dictionary.h
#include <stdio.h>

typedef enum {
    DICT_OK,
    DICT_EMPTY,
    DICT_MEMORY_ERROR,
    DICT_NO_SUCH_ELEMENT,
    DICT_ELEMENT_EXISTS,
} dict_error_t;

typedef struct {
    char key;
    size_t value;
} dict_count_node_t;

typedef struct {
    char key;
    size_t value;
    int value_len;
} dict_bit_node_t;

typedef struct {
    dict_count_node_t *nodes;
    size_t size;
    size_t hidden_size;
} dict_count_dict_t;

typedef struct {
    dict_bit_node_t *nodes;
    size_t size;
    size_t hidden_size;
} dict_bit_dict_t;

//#############################################################
dict_error_t dict_create_count(
    dict_count_dict_t **dict
);

dict_error_t dict_create_bit(
    dict_bit_dict_t **dict
);

dict_error_t dict_add_count(
    dict_count_dict_t * dict,
    char symbol
);

dict_error_t dict_add_bit(
    dict_bit_dict_t * dict, 
    char symbol, 
    size_t value,
    int value_len
);

dict_error_t dict_get_count(
    dict_count_dict_t * dict,
    char symbol,
    size_t *value
);

dict_error_t dict_get_bit(
    dict_bit_dict_t * dict,
    char symbol,
    size_t * value,
    int * value_len
);

dict_error_t dict_get_key_bit(
    dict_bit_dict_t * dict,
    size_t value,
    int value_len,
    char * symbol
);

dict_error_t dict_pop_count(
    dict_count_dict_t * dict,
    dict_count_node_t * node
);

dict_error_t dict_pop_bit(
    dict_bit_dict_t * dict,
    dict_bit_node_t * node
);

dict_error_t dict_free_count(
    dict_count_dict_t **dict
);

dict_error_t dict_free_bit(
    dict_bit_dict_t **dict
);
