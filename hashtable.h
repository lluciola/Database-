#define HASHTABLE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct ht_list_s {
    char *key;
    int value;
    struct ht_list_s *next;
} ht_list;

void list_insert(ht_list **l, int val, const char *key);

ht_list * list_search(ht_list *l, const char *key);

void list_remove(ht_list **l);

void list_print(const ht_list *l);

typedef struct ht_s {
    ht_list **buckets;
    size_t buckets_count;
    size_t buckets_filled;
    size_t (*hash_function) (size_t size, const char *key);
} ht;

void ht_delete(ht *table, const char* key);

int _dump_db(const char* filename, ht* table);

//int ht_add_from_file(const char* filename, ht *table);

int _load_db(const char* filename, ht* table);

ht *ht_create(size_t start_size,
              size_t (*hash_function) (size_t size, const char *key));

void __ht_ensure_size(ht *table);

void __ht_rehash(ht *table);

void ht_insert(ht *table, int val, const char *key);

ht_list *ht_search(const ht *table, const char *key);

void ht_remove(ht **table);

void ht_print(const ht *table);

void manual();
