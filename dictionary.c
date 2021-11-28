// dictionary.c

#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>

dict_node_t *find_node(dict_dict_t dict, char key)
{
    dict_node_t *node = NULL;

    for (size_t i = 0; i < dict.size; i++)
    {
        dict_node_t *nodex = &dict.nodes[i];
        if (nodex->key == key){
            node = nodex;
            break;
        }
    }
    return node;
}


dict_error_t dict_create(dict_dict_t * dict) 
{
    dict = malloc(sizeof(dict_dict_t));
    dict->nodes = malloc(100*sizeof(dict_node_t));
    dict->size = 0;

    return DICT_OK;
}


dict_error_t dict_add(dict_dict_t * dict, char symbol)
{
    printf("dict_add\n");// TODO delete later

    dict_node_t *node = find_node(*dict, symbol);
    
    // Lisame uue elemendi sõnastikku
    if (node == NULL){
        // Suurendame vajadusel massiivi
        if (sizeof(dict->nodes) == dict->size){
            dict->nodes = realloc(dict->nodes, dict->size * 2 * sizeof(dict_node_t));
            dict->size *= 2;
        }

        node = &dict->nodes[dict->size++];
        node->key = symbol;
        node->value = 0;
    }

    node->value++;
    
    return DICT_OK;
}

dict_error_t dict_get(dict_dict_t * dict, char symbol, int *value)
{
    printf("dict_get\n");// TODO delete later
    dict_node_t *node = find_node(*dict, symbol);
    value = node->value;

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
    free(dict->nodes);
    free(dict);
    
    return DICT_OK;
}
