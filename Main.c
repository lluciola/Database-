#include "HashTable.h"
#include "Adresses.h"
#include<stdio.h>

ht* table = NULL;


//============================================================================================================

size_t hashfunc(size_t htsize, const char* key) 
{
        size_t h = 0x73f8e5a39fe37a1b;
        while(*key) 
        {
            h ^= h << 13 | h >> 51;
            h += (*key)*0xa5a15f11a1f1af31;
            h = h << 49 | h>>15;
            key++;
        }
        return h % htsize;
}

void db_init() {
	printf("INitializing database...\n");
	table = ht_create(1000, hashfunc);
	printf("Loading database from file...\n");
	_load_db("database.txt", table);
	printf("Done\n");
}
//============================================================================================================



int db_interpret(char* buf, size_t len, char** resp_buf) {
	char key[100];

	int value;

	char cmd = buf[0];
	buf += 1;
	printf("Command %c, rest: '%s'\n", cmd, buf);
	switch(cmd) {
		case 'H':
			return manual(resp_buf);
		case 'Q':
			return asprintf(resp_buf, "Bye!\n");
		case 'S':
			if ( sscanf(buf, "%s", key) == 1) {
				printf("Reading key '%s'\n", key);
				ht_list* res = ht_search(table, key);
				if(res == NULL)
					return asprintf(resp_buf, "Empty\n");
				else
					return asprintf(resp_buf, "%s %d\n", res->key, res->value);
			}
			return asprintf(resp_buf, "ERROR: wrong key\n");
		case 'A': 
			if (sscanf(buf, "%s%d", key, &value) == 2 ) {
				printf("Inserting %s with %d\n", key, value);
				ht_insert(table, value, key);
				return asprintf(resp_buf, "Added successfully\n");
			}
			return asprintf(resp_buf, "Cannot insert\n");


		case 'D':
			if ( sscanf(buf, "%s", key) != 1) return asprintf(resp_buf, "ERROR: wrong key\n");
			if (ht_delete(table, key))
				return asprintf(resp_buf, "OK\n");
			return asprintf(resp_buf, "ERROR\n");

		case 'M':
			_dump_db("database.txt", table);
			return asprintf(resp_buf, "OK\n");

		case 'I':
			if ( sscanf(buf, "%s", key) == 1 ) {
				if(incr_el(table, key))
					return asprintf(resp_buf, "Element not found\n");
				return asprintf(resp_buf, "OK\n");
			}

			return asprintf(resp_buf, "ERROR: wrong key\n");
		case 'E':
			if ( sscanf(buf, "%s", key) != 1) return asprintf(resp_buf, "ERROR: wrong key\n");
			if(decr_el(table, key))
				return asprintf(resp_buf, "Element not found");
			return asprintf(resp_buf, "OK\n");
		case ' ': case '\n': case '\0':
			break;
		default:
			return asprintf(resp_buf, "Unknown command\n");
	}
}
