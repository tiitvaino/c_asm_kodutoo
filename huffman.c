// huffman.c
#include "huffman.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// näide: https://www.youtube.com/watch?v=dM6us854Jk0
typedef struct h_tree_t{
    bool is_leaf;
    char symbol;
    int value;
    struct h_tree_t *left_child;
    struct h_tree_t *right_child;
} h_tree_t;

// http://www.cplusplus.com/reference/cstdlib/qsort/
static int tree_compare(const void *a, const void* b)
{
    h_tree_t node_a = *((h_tree_t *) a);
    h_tree_t node_b = *((h_tree_t *) b);
    if (node_a.value == node_b.value){
        return 0;
    }
    // node_a läheb järjekorras ette poole
    else if ((node_a.value > node_b.value)) return -1;
    // node_b läheb järjekorras ette poole
    else return 1;
}

//https://stackoverflow.com/questions/22059189/read-a-file-as-byte-array
static h_error_t make_count_dictionary(dict_count_dict_t *dict, FILE *src_file)
{
    // Loeme puhvrisse, et saaks ka suuremate failidega tegeleda
    size_t buffer_size = 1000;
    char *ptr = malloc(buffer_size * sizeof(char));
    if (ptr == NULL){
        printf("Not enough memory\n");
        return H_ERROR;
    }
    size_t size_of_elements = sizeof(char); // 1

    // Leiame faili pikkuse
    int fseek_val = fseek(src_file,0,SEEK_END);
    long filelen = ftell(src_file);
    rewind(src_file);

    // Loendame failis kõik erinevad baidid ära
    size_t bytes_read = 0;

    while(filelen > 0) 
    {
        // Loeme puhvri jagu baite sisse
        bytes_read = fread(ptr,size_of_elements,buffer_size,src_file);

        // Lisame loetud baidid sõnaraamatusse, kus need loendatakse
        for (size_t i = 0; i < bytes_read && filelen >= 0; i++, filelen--){
            dict_add_count(dict,ptr[i]);
        }
        
        // Kontrollime kas kõik on OK
        if (bytes_read < buffer_size)
        {
            if (filelen != 0){
                printf("Problem with reading file :(\n\tbytes_read: %lu, filelen: %ld\n",bytes_read,filelen);
                return H_ERROR;
            }
        }
    }

    // Liigume faili algusesse tagasi
    free(ptr);
    rewind(src_file);
    return H_OK;
}

static h_error_t make_tree(dict_count_dict_t *dict, h_tree_t *tree)
{
    dict_error_t error = DICT_OK;
    // Siin hakkame hoidma töökäigus tekkinud alampuid
    h_tree_t *sub_trees = (h_tree_t *) malloc(dict->size * sizeof(h_tree_t));//[dict->size];
    int nr_of_subtrees = 0;
    if (sub_trees == NULL){
        printf("Not enough memory\n");
        return H_ERROR;
    }

    // Lisame kõik elemendid eraldi alampuudena massiivi
    while (dict->size >0) {
        dict_count_node_t node = {'\0',0};
        error = dict_pop_count(dict,&node);
        if (error != DICT_OK || node.value == 0){
            printf("Problem with counting symbols from file.:(\n dicterror=%d\n",error);
            return H_ERROR;
        }

        h_tree_t *sub_tree = &sub_trees[nr_of_subtrees++];
        sub_tree->is_leaf = true;
        sub_tree->symbol = node.key;
        sub_tree->value = node.value;
        sub_tree->left_child = NULL;
        sub_tree->right_child = NULL;
    }
    // Käime nii kaua kuni kõik alampuud on kokku lükatud
    do {
        // Sorteerime massiivi nii, et väiksemad arvud oleks tagapool
        qsort(sub_trees,nr_of_subtrees,sizeof(h_tree_t),tree_compare);
        
        h_tree_t child1 = sub_trees[--nr_of_subtrees];
        h_tree_t * left_child = (h_tree_t*) malloc(sizeof(h_tree_t));
        if (left_child == NULL){
            printf("Not enough memory\n");
            free(sub_trees);
            return H_ERROR;
        }
        *left_child = child1;

        h_tree_t child2 = sub_trees[--nr_of_subtrees];
        h_tree_t * right_child = (h_tree_t*) malloc(sizeof(h_tree_t));
        if (right_child == NULL){
            printf("Not enough memory\n");
            free(sub_trees);
            return H_ERROR;
        }
        *right_child = child2;

        h_tree_t *root = &sub_trees[nr_of_subtrees++];
        root->is_leaf = false;
        root->symbol = '\0';
        root->value = left_child->value+right_child->value;
        root->left_child = left_child;
        root->right_child = right_child;

    } while (nr_of_subtrees > 1);
    
    *tree = sub_trees[0];
    free(sub_trees);
    return H_OK;
}

static h_error_t make_byte_dictionary_help_r(dict_bit_dict_t *dict, h_tree_t *tree, int bit_array_len, size_t bit_array)
{
    if(tree->is_leaf){
        dict_add_bit(dict,tree->symbol,bit_array,bit_array_len);
        free(tree);
        return H_OK;
    }

    h_tree_t *left_child = tree->left_child;
    h_tree_t *right_child = tree->right_child;

    int new_bit_array_len = bit_array_len + 1;
    size_t new_left_bit_array = bit_array << 1;
    size_t new_right_bit_array = bit_array << 1 | 1;

    make_byte_dictionary_help_r(dict,left_child, new_bit_array_len, new_left_bit_array);
    make_byte_dictionary_help_r(dict,right_child, new_bit_array_len, new_right_bit_array);

    //free(tree);
    return H_OK;
}

static void print_tree(h_tree_t *tree, int level){
    if (!(tree->is_leaf)){
        print_tree(tree->left_child,level+1);
    }
    for (size_t i = 0; i < level; i++)
    {
        printf("  ");
    }
    if (tree->is_leaf)
        printf("leaf node %c: %d\n", tree->symbol, tree->value);
    else
        printf("internal node: %d\n", tree->value);
    if (!(tree->is_leaf)){
        print_tree(tree->right_child,level+1);
    }
}

static h_error_t make_byte_dictionary(dict_bit_dict_t *dict, h_tree_t *root)
{
    //print_tree(root,0);
    make_byte_dictionary_help_r(dict,root,0,0);
    return H_OK;
}

static h_error_t pack_and_write(dict_bit_dict_t *dict, FILE *src_file, FILE *dst_file)
{
    // Loeme puhvrisse, et saaks ka suuremate failidega tegeleda
    size_t buffer_size = 1000;
    char *ptr = malloc(buffer_size * sizeof(char));
    if (ptr == NULL){
        printf("Not enough memory\n");
        return H_ERROR;
    }
    int size_of_elements = sizeof(char); // 1

    // Leiame faili pikkuse
    int fseek_val = fseek(src_file,0,SEEK_END);
    long filelen = ftell(src_file);
    rewind(src_file);

    size_t written = 0;
    //Kirjutame faili pikkuse
    written = fwrite(&filelen, sizeof(filelen), 1, dst_file);
    if (written != 1){
        printf("Problem with writing into the file. :(\n");
        free(ptr);
        return H_ERROR;
    }
    //Kirjutame sõnaraamatu
    written = fwrite(dict, sizeof(dict_bit_dict_t), 1, dst_file);
    if (written != 1){
        printf("Problem with writing into the file. :(\n");
        free(ptr);
        return H_ERROR;
    }
    written = fwrite(dict->nodes, sizeof(dict_bit_node_t), dict->size, dst_file);
    if (written != dict->size){
        printf("Problem with writing into the file. :(\n");
        free(ptr);
        return H_ERROR;
    }

    // Prindime sõnaraamatu välja
    //for (size_t i = 0; i < dict->size; i++)
    //{
    //    dict_bit_node_t *nodex = &dict->nodes[i];
    //    printf("%c: %lx, %d\n", nodex->key,nodex->value,nodex->value_len);
    //}
    
    size_t bytes_read = 1;
    char bit_buffer_value = 0;
    int bit_buffer_size = sizeof(bit_buffer_value)*8;
    int bit_buffer_written=0;
    //Kirjutame pakitud teksti ja loeme algse teksti
    while(filelen > 0) 
    {
        // Loeme puhvri jagu baite sisse
        bytes_read = fread(ptr,size_of_elements,buffer_size,src_file);
        
        // Kodeerime loetud baidid ja kirjutame välja
        for (size_t i = 0; i < bytes_read && filelen >= 0; i++, filelen--){
            size_t value = 0;
            int value_len = 0;
            dict_get_bit(dict,ptr[i],&value,&value_len);
            if (bit_buffer_written + value_len > bit_buffer_size){
                int free_space_size = bit_buffer_size-bit_buffer_written;
                int leftover_bits = value_len-free_space_size;
                bit_buffer_value = bit_buffer_value | (value>>leftover_bits);

                // kirjutame välja
                written = fwrite(&bit_buffer_value, sizeof(bit_buffer_value), 1, dst_file);
                if (written != 1){
                    printf("Problem with writing into the file. :(\n");
                    free(ptr);
                    return H_ERROR;
                }
                // kanname ülejäänud bitid üle
                size_t mask = 0;
                for (size_t i = leftover_bits; i < sizeof(size_t); i++)
                    mask = (mask << 8)|0xff;
                mask = mask << leftover_bits;

                bit_buffer_value = ~mask & value;
                bit_buffer_written = leftover_bits;
                bit_buffer_value = bit_buffer_value << (sizeof(bit_buffer_value)*8-leftover_bits);
            }
            else{
                int shift =sizeof(bit_buffer_value)*8-value_len-bit_buffer_written;
                bit_buffer_value = (value << shift) | bit_buffer_value;
                bit_buffer_written+=value_len;
            }
        }
        
        // Kontrollime kas kõik on OK
        if (bytes_read < buffer_size)
        {
            if (filelen != 0)
            {
                printf("Problem with reading file :(\n\tbytes_read: %lu, filelen: %ld\n",bytes_read,filelen);
                return H_ERROR;
            }
        }
    }

    // Kirjutame viimased bitid ka ära
    if (bit_buffer_written!= 0){
        int nr_bytes = sizeof(bit_buffer_value);
        int bytes_to_write = bit_buffer_written/8;
        int remain_bits = bit_buffer_written % 8;
        if (remain_bits!= 0){
            bytes_to_write++;
        }
        char bytes[bytes_to_write];
        for (size_t i = 0; i < bytes_to_write; i++)
            bytes[bytes_to_write-1-i] = (bit_buffer_value >> 8*i) & 0xFF;


        written = fwrite(bytes, sizeof(char), bytes_to_write, dst_file);
        if (written != bytes_to_write){
            printf("Problem with writing into the file. :(\n");
            free(ptr);
            return H_ERROR;
        }
    }
    // Liigume faili algusesse tagasi
    rewind(src_file);
    //free(ptr);
    return H_OK;
}

h_error_t h_pack(FILE *src_file, FILE *dst_file)
{
    
    // Loendame sümbolid alus failis ära
    static dict_count_dict_t * dict = NULL;
    if (dict_create_count(&dict)){
        printf("Not enough memory\n");
        return H_ERROR;
    }

    if (make_count_dictionary(dict,src_file)){
        dict_free_count(&dict);
        return H_ERROR;
    }

    // Teeme loenduse põhjal huffmani puu
    h_tree_t *tree = (h_tree_t *) malloc(sizeof(h_tree_t));
    if (tree == NULL){
        printf("Not enough memory\n");
        dict_free_count(&dict);
        return H_ERROR;
    }

    if (make_tree(dict, tree)){
        dict_free_count(&dict);
        free(tree);
        return H_ERROR;
    }

    // Tõlgime huffmani puu sõnaraamatuks sümbolist -> bitireani
    static dict_bit_dict_t *char_byte_dict = NULL;
    if(dict_create_bit(&char_byte_dict)){
        printf("Not enough memory\n");
        dict_free_count(&dict);
        dict_free_bit(&char_byte_dict);
        free(tree);
        return H_ERROR;
    }

    if (make_byte_dictionary(char_byte_dict, tree)){
        dict_free_count(&dict);
        dict_free_bit(&char_byte_dict);
        free(tree);
        return H_ERROR;
    }

    // Sõnaraamatu abil tõlgime alusfaili uude faili
    if (pack_and_write(char_byte_dict,src_file,dst_file)){
        dict_free_count(&dict);
        dict_free_bit(&char_byte_dict);
        free(tree);
        return H_ERROR;
    }

    // Vabastame mälu
    dict_free_count(&dict);
    dict_free_bit(&char_byte_dict);
    free(tree);
    return H_OK;
}

h_error_t h_unpack(FILE *src_file, FILE *dst_file)
{
    // Loeme puhvrisse, et saaks ka suuremate failidega tegeleda
    size_t buffer_size = 1000;
    char *ptr_in = malloc(buffer_size * sizeof(char));
    if (ptr_in == NULL){
        printf("Not enough memory\n");
        return H_ERROR;
    }
    int ptr_in_idx = 0;
    char *ptr_out = malloc(buffer_size * sizeof(char));
    if (ptr_out == NULL){
        printf("Not enough memory\n");
        free(ptr_in);
        return H_ERROR;
    }
    int ptr_out_idx = 0;
    size_t size_of_elements = sizeof(char); // 1
    int written = 0;

    // Loema pakitud faili pikkuse
    fseek(src_file,0,SEEK_END);
    long filelen = ftell(src_file);
    rewind(src_file);
    // Tühja faili kontroll
    if (filelen == 0)
        return H_OK;

    // Pakime faili lahti
    size_t bytes_read = 0;
    long packed_filelen = 0;
    dict_bit_dict_t *dict = (dict_bit_dict_t *) malloc(sizeof(dict_bit_dict_t));
    // Loeme päris failipikkuse
    size_t x_read = 0;
    x_read = fread(&packed_filelen, sizeof(packed_filelen), 1, src_file);
    if (x_read != 1){
        printf("File is corrupted!\n");
        free(ptr_out);
        free(ptr_in);
        free(dict);
        return H_ERROR;
    }
    bytes_read += x_read * sizeof(packed_filelen);
    // Loeme sõnaraamatu
    x_read = fread(dict, sizeof(dict_bit_dict_t), 1, src_file);
    if (x_read != 1){
        printf("File is corrupted!\n");
        free(ptr_in);
        free(ptr_out);
        free(dict);
        return H_ERROR;
    }
    bytes_read += x_read * sizeof(dict_bit_dict_t);
    x_read = fread(dict->nodes, sizeof(dict_bit_node_t), dict->size, src_file);
    if (x_read != dict->size){
        printf("File is corrupted!\n");
        free(ptr_in);
        free(ptr_out);
        free(dict);
        return H_ERROR;
    }
    bytes_read += x_read * sizeof(dict_bit_node_t);
    dict->hidden_size = dict->size;

    //Prindime saadud sõnaraamatu välja
    //for (size_t i = 0; i < dict->size; i++)
    //{
    //    dict_bit_node_t *nodex = &dict->nodes[i];
    //    printf("%c: %lx, %d\n", nodex->key,nodex->value,nodex->value_len);
    //}

    filelen -= bytes_read;
    bytes_read = 0;

    size_t bit_buffer = 0;
    int bit_buffer_read = 0;
    //Loeme pakitud ja kirjutame lahtipakitud teksti
    while(filelen > 0) 
    {
        // Loeme puhvri jagu baite sisse
        x_read = fread(ptr_in,size_of_elements,buffer_size,src_file);
        bytes_read += x_read;
        filelen -= x_read;

        ptr_in_idx = 0;
        // Dekodeerime loetud baidid ja kirjutame välja
        while (ptr_in_idx < x_read)
        {
            if (bit_buffer_read == 0){
                bit_buffer = (ptr_in[ptr_in_idx++] & 0xff);
                bit_buffer_read +=8;
            }

            bool found = false;
            char symbol = '\0';
            // Otsime sobivat sümbolit
            for (int i = 0; !found && i < dict->size && i < bit_buffer_read; i++)
            {
                int value_len = 1+i;
                size_t value = bit_buffer >> bit_buffer_read-1-i;
                dict_error_t dict_error = dict_get_key_bit(dict, value, value_len, &symbol);
                // Kas leidsime sobiva sümboli
                if (dict_error == DICT_OK){
                    size_t mask = 0;
                    for (size_t i = 0; i < sizeof(size_t); i++)
                        mask = (mask << 8)|0xff;
                    mask = mask << bit_buffer_read-1-i;
                    bit_buffer &= ~mask;// Jätame väiksema väärtusega bitid alles
                    bit_buffer_read = bit_buffer_read-1-i;
                    found = true;
                    break;
                }
            } 
            // Lisame veel bitte kui ei leitud sümbolit
            if (!found) {
                bit_buffer = (bit_buffer << 8) | (ptr_in[ptr_in_idx++] & 0xff);
                bit_buffer_read +=8;
            }
            else{
                found = false;
                // Lisame leitud sümboli puhvrisse kui on loogiline (Lõpus võivad mõned bitid üle jääda)
                if (packed_filelen > 0) {
                    ptr_out[ptr_out_idx++] = symbol;
                    packed_filelen--;
                }
                // Kui puhver täis, kirjutame välja
                if (ptr_out_idx == buffer_size) {
                    written = fwrite(ptr_out,sizeof(char),ptr_out_idx,dst_file);
                    if (written != ptr_out_idx){
                        printf("Problem with writing into the file. :(\n");
                        free(ptr_in);
                        free(ptr_out);
                        free(dict);
                    }
                    ptr_out_idx = 0;
                }
            }
        }
        // Kontrollime kas kõik on OK
        if (bytes_read < buffer_size)
        {
            if (filelen != 0) 
            {
                printf("File is corrupted!\n");
                free(ptr_in);
                free(ptr_out);
                free(dict);
                return H_ERROR;
            }
        }
    }


    // Kirjutame veel välja, mis kirjutamata jäi
    if(ptr_out_idx > 0){
        written = fwrite(ptr_out,sizeof(char),ptr_out_idx,dst_file);
        if (written != ptr_out_idx){
            printf("Problem with writing into the file. :(\n");
            free(ptr_in);
            free(dict);
            return H_ERROR;
        }
    }

    // Liigume faili algusesse tagasi
    rewind(src_file);
    free(ptr_in);
    free(ptr_out);
    free(dict);
    return H_OK;
}
