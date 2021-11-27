// dictionary.c

#include "dictionary.h"
#include <stdio.h>

dict_error_t dict_create(dict_dict_t * dict) {
    dict = NULL;
    return DICT_OK;
}

dict_error_t dict_add(dict_dict_t * dict, char symbol)
{
    printf("dict_add\n");// TODO delete later
    return DICT_OK;
}

dict_error_t dict_get(dict_dict_t * dict, char symbol, int *value)
{
    printf("dict_get\n");// TODO delete later

    return DICT_OK;
}

dict_error_t dict_pop(dict_dict_t * dict, dict_node_t * node)
{
    printf("dict_pop\n");// TODO delete later
    return DICT_OK;
}

dict_error_t dict_free( dict_dict_t * dict)
{
    printf("dict_free\n");// TODO delete later

    return DICT_OK;
}
