#include "HashTable.h"
#include "Adresses.h"

//============================================================================================================

void list_insert(ht_list **l, int val, const char *key) 
{
    ht_list *new_element = malloc(sizeof(ht_list));
    *new_element = (ht_list) {strdup(key), val, *l};
    *l = new_element;
    return;
}

//============================================================================================================

ht_list * list_search(ht_list *l, const char *key) 
{
    if(l == NULL) 
    {
        return NULL;
    }

    if(!strcmp(l->key, key)) 
    {
        return l;
    }

    return list_search(l->next, key);
}

//============================================================================================================

void list_remove(ht_list **l) 
{
    if(*l == NULL) 
    {
        return;
    }

    list_remove(&(*l)->next);

    if((*l)->key != NULL) 
    {
        free((*l)->key);
    }

    free(*l);
    *l = NULL;
    return;
}

//============================================================================================================

void list_print(const ht_list *l, char* resp) 
{
    if(!l) {
        strcpy(resp, "\n");
        return;
    }
    strcpy(resp, l->value);
    strcpy(resp, l->key);
    list_print(l->next, resp);
    return;
}

//============================================================================================================

int _dump_db(const char* filename, ht* table, char* resp) 
{
      FILE* f;

      if (fopen(filename, "r") == NULL)
      {
            fclose(f);
            strcpy(resp, "Error code: 1. Problem with file opening!");
      }

      else
      {
            for (size_t i = 0 ; i < table->buckets_count; i++ )
            {
                  ht_list* el = table->buckets[i];

                  while(el != NULL) 
                  {
                        strcpy(resp, el->key);
                        strcpy(resp, el->value);
                        el = el->next;
                  }
            }
            fclose(f);
            strcpy(resp, "DUMPED successfully\n");
            return 0;
        }
}

//============================================================================================================

int _load_db(const char* filename, ht* table, char* resp) 
{
        FILE* f;
        char key[100];
        int counter;

        if (fopen(filename, "r") == NULL)
        {
            fclose(f);
            strcpy(resp, "Error code: 1. Problem with file opening!");
        }

        else 
        {
            while (fscanf(f, "%s %d", key, &counter) == 2)
                  ht_insert(table, counter, key);
            fclose(f);
            strcpy(resp, "LOADED successfully\n");
        }
}

//============================================================================================================

ht * ht_create(size_t start_size,
               size_t (*hash_func) (size_t size, const char *key)) {
    ht *ret = malloc(sizeof(ht));
    *ret = (ht) {calloc(start_size, sizeof(ht_list *)), start_size, 0, hash_func};
    return ret;
}


//============================================================================================================

void __ht_ensure_size(ht *table) 
{
    if(table->buckets_count * 7 < table->buckets_filled * 10) 
    {
        __ht_rehash(table);
    }

    return;
}

//============================================================================================================

void __ht_rehash(ht *table) 
{
    ht *new_table = ht_create(table->buckets_count * 2, table->hash_function);

    for(int i = 0; i < table->buckets_count; ++i) 
    {
        ht_list *elem = table->buckets[i];
        while(elem) 
        {
            ht_insert(new_table, elem->value, elem->key);
            free(elem->key);
            elem->key = NULL;
            ht_list *next_elem = elem->next;
            elem = next_elem;
        }
        list_remove(&table->buckets[i]);
    }

    free(table->buckets);
    table->buckets_count *= 2;
    table->buckets_filled = new_table->buckets_filled;
    table->buckets = new_table->buckets;
    free(new_table);

    return;
}

//============================================================================================================

void ht_insert(ht *table, int val, const char *key) 
{
    ht_list *search_result = ht_search(table, key);

    if(search_result) 
    {
        search_result->value = val;
        return;
    }

    int bucket_index = table->hash_function(table->buckets_count, key);

    if(table->buckets[bucket_index] == NULL) {
        ++table->buckets_filled;
    }

    list_insert(&(table->buckets[bucket_index]), val, key);

    __ht_ensure_size(table);

    return;
}

//============================================================================================================

ht_list* ht_search(const ht *table, const char* key) 
{
    int bucket_index = table->hash_function(table->buckets_count, key);
    return list_search(table->buckets[bucket_index], key);
}

//============================================================================================================

void ht_remove(ht **table) 
{
    ht *_table = *table;

    for(size_t i = 0; i < _table->buckets_count; ++i) 
    {
        list_remove(&_table->buckets[i]);
    }

    free(_table->buckets);
    free(_table);
    *table = NULL;
    return;
}

//============================================================================================================

void ht_delete(ht* table, const char* key)
{
        ht_list* list = ht_search(table, key);
        list_remove(&list);
}

//============================================================================================================

void ht_print(const ht *table, char* resp) 
{
    for(size_t i = 0; i < table->buckets_count; ++i) 
    {
        strcpy(resp, i);
        list_print(table->buckets[i], resp);
    }
    return;
}

//============================================================================================================
/*
int read_chars(char ch)
{   

      if()
      {
            printf("Error code: 3. Incorrect character input.\n");
      }
}
*/
//============================================================================================================

int incr_el(ht* table, const char* key){
    ht_list* el = ht_search(table, key);
    if(el)
        el->value++;
        return 0;
    return 1;
}

//=============================================================================================================

int decr_el(ht* table, const char* key){
    ht_list* el = ht_search(table, key);
    if(el)
        el->value--;
        return 0;
    return 1;
}

//==============================================================================================================

void manual(char* resp)
{
        strcpy(resp, "'Q'- quit;\n'S' - search for key;\n'D' - delete an element;\n'A' - add an element;\n'L' -load elements from a file;\n'P' - print the database;\n'M' - dump all changes;\n'I' - increase the value;\n'E' - decrease the value\n");
}