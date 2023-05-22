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

int list_print(const ht_list *l, char** resp) 
{
	char* buf;
	int rel;
    if(!l) 
        return 0;
   	rel = asprintf(&buf, "%s%s %d\n", *resp, l->key, l->value);
	free(*resp);
	*resp = buf;
    return rel+list_print(l->next, resp);
}

//============================================================================================================

int _dump_db(const char* filename, ht* table) {
	FILE* f;

	if ((f = fopen(filename, "w")) == NULL) {
		printf("Cannot open database file for dumping %s\n", filename);
		return -1;
	}
	for (size_t i = 0 ; i < table->buckets_count; i++ )
	{
		for (ht_list* el = table->buckets[i]; el; el=el->next) 
			fprintf(f, "%s %d\n", el->key, el->value);

	}
	fclose(f);
	printf("DUmped successfully\n");
	return 0;
}

//============================================================================================================

int _load_db(const char* filename, ht* table) 
{
        FILE* f;
        char key[100];
        int counter;

        if ((f = fopen(filename, "r")) == NULL) {
			printf("Cannot open database file %s\n", filename);
			return -1;
        }

		while (fscanf(f, "%s %d", key, &counter) == 2)
			  ht_insert(table, counter, key);
		fclose(f);
		return 0;
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

int ht_delete(ht* table, const char* key)
{
        ht_list* list = ht_search(table, key);
		if ( !list) return -1;
        list_remove(&list);
		return 0;
}

//============================================================================================================

int ht_print(const ht *table, char** resp) 
{
	int sz = 0;
    for(size_t i = 0; i < table->buckets_count; ++i) 
    {
        char* buf;
		sz += asprintf(&buf, "%s%lu ", *resp, i);
		free(*resp);
        sz += list_print(table->buckets[i], &buf);
		*resp = buf;
    }
    return sz;
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

int manual(char** resp)
{
   asprintf(resp, "'Q'- quit;\n'S' - search for key;\n'D' - delete an element;\n'A' - add an element;\n'L' -load elements from a file;\n'P' - print the database;\n'M' - dump all changes;\n'I' - increase the value;\n'E' - decrease the value\n");
}
