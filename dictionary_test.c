// dictionary_test.c
# include "dictionary.h"
#include <stdio.h>
#include <stdbool.h>

char c_s[3] = {'x','a','b'};
int c_s_nr[3] = {12,3,2};

static void OK(int test_nr){
    printf("%d: OK\n",test_nr);
};

static void FAIL(int test_nr){
    printf("%d: FAIL\n",test_nr);
};

static void test_create(dict_count_dict_t **dict)
{
    int test_nr = 1;
    dict_error_t error = DICT_OK;
    if ( error = dict_create_count(dict)) FAIL(test_nr);
    else if ((*dict) != NULL && (*dict)->size == 0) OK(test_nr);
    else FAIL(test_nr);
    return;
}

static void test_add_and_get(dict_count_dict_t *dict)
{
    int test_nr = 2;
    dict_error_t error = DICT_OK;
    char c;
    int nr_c;
    size_t nr_c_got;

    for (size_t i = 0; i < sizeof(c_s); i++)
    {
        c = c_s[i];
        nr_c = c_s_nr[i];
        for (size_t j = 0; j < nr_c; j++)
            if (error = dict_add_count(dict,c)){
                FAIL(test_nr);
                return;
            }
        if(error = dict_get_count(dict,c,&nr_c_got)){
            FAIL(test_nr);
            return;
        }
        
        if (nr_c != nr_c_got){ 
            FAIL(test_nr);
            return;
        }   
    }

    if (dict->size != 3){ 
        FAIL(test_nr);
        return;
    }

    OK(test_nr);
    return;
}

static void test_pop(dict_count_dict_t *dict)
{
    int test_nr = 3;
    dict_error_t error = DICT_OK;
    dict_count_node_t node_test = {'\0',0};

    for (size_t i = 0; i < sizeof(c_s); i++)
    {
        if (error = dict_pop_count(dict,&node_test)){
            FAIL(test_nr);
            return;
        }
        if (&node_test == NULL || node_test.key != c_s[i] || 
            node_test.value != c_s_nr[i] || dict->size != sizeof(c_s)-1-i) {
            FAIL(test_nr);
            return;
        }
    }

    error = dict_pop_count(dict,&node_test);
    if (error != DICT_EMPTY || dict->size != 0)  FAIL(test_nr);
    else OK(test_nr);
    return;
}

static void test_free(dict_count_dict_t **dict)
{
    dict_error_t error = DICT_OK;
    int test_nr = 4;
    if( error = dict_free_count(dict)) FAIL(test_nr);
    else if ((*dict) != NULL) FAIL(test_nr);
    else OK (test_nr);
}

int main()
{
    dict_count_dict_t *dict = NULL;
    // test nr 1
    test_create(&dict);
    // test nr 2
    test_add_and_get(dict);
    // test nr 3
    test_pop(dict);
    // test nr 4
    test_free(&dict);
}
