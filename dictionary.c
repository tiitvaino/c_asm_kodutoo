// dictionary.c

#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>

static dict_node_t *find_node(dict_dict_t dict, char key)
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


dict_error_t dict_create(dict_dict_t **dict) 
{
    printf("dict_create\n");// TODO delete later
    *dict = (dict_dict_t *) malloc(sizeof(dict_dict_t));
    if (*dict == NULL) return DICT_MEMORY_ERROR;
    (*dict)->nodes = (dict_node_t *) malloc(100*sizeof(dict_node_t));
    if ((*dict)->nodes == NULL) return DICT_MEMORY_ERROR;
    (*dict)->size = 0;

    return DICT_OK;
}

// http://www.cplusplus.com/reference/cstdlib/qsort/
static int dict_compare(const void *a, const void* b)
{
    dict_node_t node_a = * ((dict_node_t *) a);
    dict_node_t node_b = * ((dict_node_t *) b);
    if (node_a.value == node_b.value){
        printf("There is some error in dict elements: %c:%d;%c:%d;", node_a.key, node_a.value, node_b.key, node_b.value);
        return 0;
    }
    // node_a läheb järjekorras ette poole
    else if ((node_a.value < node_b.value)) return -1;
    // node_b läheb järjekorras ette poole
    else return 1;
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
            if (dict->nodes == NULL) return DICT_MEMORY_ERROR;
            dict->size *= 2;
        }

        node = &dict->nodes[dict->size++];
        node->key = symbol;
        node->value = 0;
    }

    node->value++;
    qsort(dict->nodes,dict->size,sizeof(dict_node_t),dict_compare);
    
    return DICT_OK;
}

dict_error_t dict_get(dict_dict_t * dict, char symbol, int *value)
{
    printf("dict_get\n");// TODO delete later
    dict_node_t *node = find_node(*dict, symbol);
    if (node == NULL){
        *value = -1;
        return DICT_NO_SUCH_ELEMENT;
    }
    *value = node->value;

    return DICT_OK;
}

dict_error_t dict_pop(dict_dict_t * dict, dict_node_t *node)
{
    printf("dict_pop\n");// TODO delete later
    // Viimane element on suurim sorteerimiste tõttu.
    if (dict->size > 0)
        *node = (dict->nodes[--(dict->size)]); // free funktsioon või realloc teeb puhtaks
    else{
        node->key ='\0';
        node->value = 0;
        return DICT_EMPTY;
    }

    return DICT_OK;
}

dict_error_t dict_free( dict_dict_t **dict)
{
    printf("dict_free\n");// TODO delete later
    free((*dict)->nodes);
    (*dict)->nodes = NULL;
    free(*dict);
    *dict = NULL;
    return DICT_OK;
}
