#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct _list {
        char* key;
        int counter;
        struct _list* next;

} List;

List** ht = NULL;
size_t htsize = 0; // Table Size
size_t htfill = 0; // Number of non-null buckets
size_t htelems =0; // Total number of elements


size_t hashfunc(const char* key, size_t htsize) {
        size_t h = 0x73f8e5a39fe37a1b;
        while(*key) {
                h = h << 29 | h >> 35;
                h+= (*key)*65537;
                key++;
        }
        return h % htsize;
}

// counter increment
int ht_inc(const char* key, int* counter);


// counter decrement
int ht_dec(const char* key, int* counter);


// set counter to a specific value
int ht_set(const char* key, int counter);


// get counter value
int ht_get(const char* key, int* counter);

// delete counter
int ht_del(const char* key);

void _ht_set(List** newht, size_t newsize, List* el) {
        size_t h = hashfunc(el->key, newsize);
        if (!newht[h] ) htfill++;
        el->next = newht[h];
        newht[h] = el;
}
void _rehash(List** oldht, List** newht, size_t newsize) {
        size_t i;
        htfill = 0;
        for( i = 0; i< htsize; i++ ) {
                List* el = ht[i];
                while (oldht[i]) {
                        oldht[i] = el->next;
                        _ht_set(newht, newsize, el);
                }
        }
}


void _ensure_ht_size() {
        if (htfill*10 > 7*htsize) {
                // rehash needed
                List** newht = calloc(sizeof(List*), htsize*2);
                _rehash(ht, newht, htsize*2);
                ht = newht;
                htsize = htsize*2;
        }
}


int  _ht_get_counter(const char* key, int** counter) {
        size_t h = hashfunc(key, htsize);
        List * el;
        printf("Hash: %lu, el: %p\n", h, ht[h]);
        for ( el = ht[h]; el; el = el->next ) {
                if ( !strcmp(key, el->key) ) {
                        *counter = &el->counter;
                        return 0;
                }
        }
        printf("Ensuring ht size\n");
        _ensure_ht_size();
        if (!el) {
                printf("No el found, creating...\n");
                if ( !ht[h] ) htfill++;
                htelems++;
                el = malloc(sizeof(List));
                *el = (List){ strdup(key), 0 , ht[h]};
                ht[h] = el;
                *counter = &el->counter;
        }
        return 0;
}


int ht_inc(const char* key, int *counter) {
        int *cntr;
        if (!_ht_get_counter(key, &cntr))
                *counter = (*cntr)++;
        return 0;
}


int ht_dec(const char* key, int *counter ) {
        int *cntr;
        if ( !_ht_get_counter(key, &cntr))
                *counter = (*cntr)--;
        return 0;
}

int ht_set(const char* key, int value) {
        int *cntr;
        if ( !_ht_get_counter(key, &cntr))
                *cntr = value;
        return 0;
}

int ht_get(const char* key, int *value) {
        int *cntr;
        if ( !_ht_get_counter(key, &cntr))
                *value = *cntr;
        return 0;
}

List* _list_del(List* el, const char* key) {
        if ( ! el ) return NULL;
        if ( !strcmp(key, el->key) ) {
                List* next = el->next;
                free(el);
                return next;
                }
        
        free(ht);
        ht = NULL;
        htsize = 0;
        htelems = 0;
        htfill = 0;
}


int _load_db(const char* filename) {
        FILE* f;
        char key[1024];
        int counter;
        if ( f = fopen(filename, "r") ) {
                while (fscanf(f, "%s %d", key, &counter) == 2)
                        ht_set(key, counter);
                fclose(f);
                return 0;
        }
        return -1;
}

int _dump_db(const char* filename) {
        FILE* f;
        if ( f = fopen(filename, "w+") ){
                size_t i;
                for ( i = 0 ; i < htsize; i++ ) {
                        List* el;
                        for( el = ht[i]; el; el= el->next ) {
                                fprintf(f, "%s %d\n", el->key, el->counter);
                        }
                }
                fclose(f);
                return 0;
        }
        return -1;
}
                                                                                                                                                                                         133,1         40%


