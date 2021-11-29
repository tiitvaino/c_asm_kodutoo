// dictionary.h

typedef enum {
    DICT_OK,
    DICT_EMPTY,
    DICT_NO_SUCH_ELEMENT,
    DICT_MEMORY_ERROR,
} dict_error_t;

typedef struct {
    char key;
    int value;
} dict_node_t;

typedef struct {
    dict_node_t *nodes;
    int size;
} dict_dict_t;

//#############################################################
dict_error_t dict_create(
    dict_dict_t **dict
);

dict_error_t dict_add(
    dict_dict_t * dict,
    char symbol
);

dict_error_t dict_get(
    dict_dict_t * dict,
    char symbol,
    int *value
);

dict_error_t dict_pop(
    dict_dict_t * dict,
    dict_node_t * node
);

dict_error_t dict_free(
    dict_dict_t **dict
);
