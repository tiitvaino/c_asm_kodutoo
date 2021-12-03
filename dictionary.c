// dictionary.c

#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_HIDDEN_SIZE 100

static dict_count_node_t *find_count_node(dict_count_dict_t dict, char key)
{
    dict_count_node_t *node = NULL;
    for (size_t i = 0; i < dict.size; i++)
    {
        dict_count_node_t *nodex = &dict.nodes[i];
        if (nodex->key == key){
            node = nodex;
            break;
        }
    }
    
    return node;
}

static dict_bit_node_t *find_bit_node(dict_bit_dict_t dict, char key)
{
    dict_bit_node_t *node = NULL;
    
    for (size_t i = 0; i < dict.size; i++)
    {
        dict_bit_node_t *nodex = &dict.nodes[i];
        if (nodex->key == key){
            node = nodex;
            break;
        }
    }
    return node;
}

dict_error_t dict_create_count(dict_count_dict_t **dict) 
{
    *dict = (dict_count_dict_t *) malloc(sizeof(dict_count_dict_t));
    if (*dict == NULL) return DICT_MEMORY_ERROR;
    (*dict)->hidden_size = INITIAL_HIDDEN_SIZE;

    (*dict)->nodes = (dict_count_node_t *) malloc(INITIAL_HIDDEN_SIZE*sizeof(dict_count_node_t));
    

    if ((*dict)->nodes == NULL) return DICT_MEMORY_ERROR;
    (*dict)->size = 0;

    return DICT_OK;
}

dict_error_t dict_create_bit(dict_bit_dict_t **dict) 
{
    *dict = (dict_bit_dict_t *) malloc(sizeof(dict_bit_dict_t));
    if (*dict == NULL) return DICT_MEMORY_ERROR;
    (*dict)->hidden_size = INITIAL_HIDDEN_SIZE;

    (*dict)->nodes = (dict_bit_node_t *) malloc(INITIAL_HIDDEN_SIZE*sizeof(dict_bit_node_t));
    

    if ((*dict)->nodes == NULL) return DICT_MEMORY_ERROR;
    (*dict)->size = 0;

    return DICT_OK;
}

// http://www.cplusplus.com/reference/cstdlib/qsort/
static int dict_count_compare(const void *a, const void* b)
{
    dict_count_node_t node_a = * ((dict_count_node_t *) a);
    dict_count_node_t node_b = * ((dict_count_node_t *) b);
    if (node_a.value == node_b.value)
        return 0;
    // node_a läheb järjekorras ette poole
    else if ((node_a.value < node_b.value)) return -1;
    // node_b läheb järjekorras ette poole
    return 1;
}

dict_error_t dict_add_count(dict_count_dict_t * dict, char symbol)
{
    dict_count_node_t *node = find_count_node(*dict, symbol);
    
    // Lisame uue elemendi sõnastikku
    if (node == NULL){
        // Suurendame vajadusel massiivi
        if (dict->hidden_size == dict->size){
            dict->hidden_size *= 2;
            dict->nodes = (dict_count_node_t *)realloc(dict->nodes, dict->hidden_size * 2 * sizeof(dict_count_node_t));
            if (dict->nodes == NULL) return DICT_MEMORY_ERROR;
        }


        node = &(dict->nodes[(dict->size)++]);
        node->key = symbol;
        node->value = 0;
    }

    node->value++;
    qsort(dict->nodes,dict->size,sizeof(dict_count_node_t),dict_count_compare);
    return DICT_OK;
}

dict_error_t dict_add_bit(dict_bit_dict_t * dict, char symbol, size_t value, int value_len)
{
    dict_bit_node_t *node = find_bit_node(*dict, symbol);
    if (node != NULL){
        return DICT_ELEMENT_EXISTS;
    }
    // Lisame uue elemendi sõnastikku
    // Suurendame vajadusel massiivi
    if (dict->hidden_size == dict->size){
        dict->hidden_size *= 2;
        dict->nodes = (dict_bit_node_t *) realloc(dict->nodes, dict->hidden_size * 2 * sizeof(dict_bit_node_t));
        if (dict->nodes == NULL) return DICT_MEMORY_ERROR;
    }

    node = &dict->nodes[(dict->size)++];
    node->key = symbol;
    node->value = value;
    node->value_len = value_len;

    return DICT_OK;
}

dict_error_t dict_get_count(dict_count_dict_t * dict, char symbol, size_t *value)
{
    dict_count_node_t *node = find_count_node(*dict, symbol);
    if (node == NULL){
        *value = -1;
        return DICT_NO_SUCH_ELEMENT;
    }
    *value = node->value;

    return DICT_OK;
}

dict_error_t dict_get_bit(dict_bit_dict_t * dict, char symbol, size_t *value, int * value_len)
{
    dict_bit_node_t *node = find_bit_node(*dict, symbol);
    if (node == NULL){
        *value = 0;
        *value_len = 0;
        return DICT_NO_SUCH_ELEMENT;
    }
    *value = node->value;
    *value_len = node->value_len;

    return DICT_OK;
}

dict_error_t dict_get_key_bit( dict_bit_dict_t * dict, size_t value, int value_len, char * key)
{
    dict_bit_node_t *node = NULL;
    
    for (size_t i = 0; i < dict->size; i++)
    {
        dict_bit_node_t *nodex = &dict->nodes[i];
        if (nodex->value == value && nodex->value_len == value_len){
            node = nodex;
            break;
        }
    }
    
    if (node == NULL){
        *key = 0;
        return DICT_NO_SUCH_ELEMENT;
    }
    *key = node->key;
    return DICT_OK;
}

dict_error_t dict_pop_count(dict_count_dict_t * dict, dict_count_node_t *node)
{
    if (dict->size > 0)
        *node = (dict->nodes[--(dict->size)]);
    else{
        node->key ='\0';
        node->value = 0;
        return DICT_EMPTY;
    }

    return DICT_OK;
}

dict_error_t dict_pop_bit(dict_bit_dict_t * dict, dict_bit_node_t *node)
{
    if (dict->size > 0)
        *node = (dict->nodes[--(dict->size)]);
    else{
        node->key ='\0';
        node->value = 0;
        node->value_len = 0;
        return DICT_EMPTY;
    }

    return DICT_OK;
}

dict_error_t dict_free_count(dict_count_dict_t **dict)
{
    free((*dict)->nodes);
    (*dict)->nodes = NULL;
    free(*dict);
    *dict = NULL;
    return DICT_OK;
}

dict_error_t dict_free_bit(dict_bit_dict_t **dict)
{
    free((*dict)->nodes);
    (*dict)->nodes = NULL;
    free(*dict);
    *dict = NULL;
    return DICT_OK;
}
