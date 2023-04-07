#include <stdio.h>
#include "hashtable.h"
#include <string.h>

//#include <time.h>


#define Help 0
#define Q 1
#define SRCH 2
#define ADD 4
#define DEL 3
#define PRNT 6
#define LOAD 5
#define DMP 7

size_t hashfunc(size_t htsize, const char* key) {
        size_t h = 0x73f8e5a39fe37a1b;
        while(*key) {
                h ^= h << 13 | h >> 51;
                h += (*key)*0xa5a15f11a1f1af31;
                h = h << 49 | h>>15;
                key++;
        }
        return h % htsize;
}

int main() {
        ht *table = ht_create(1000, hashfunc);
        if(_load_db("database.txt", table))
                printf("ERROR while loading database\n");
        //FILE *jr = fopen("journal.txt", "r+"); // пока не сделала
        printf("Hello! Please, enter a command or type 'Help':\n");
        int cmd = 0;
        // команды надо вводить цифрами, пока не разобралась, как сделать по другому
        int flag = 1;
        while(flag){
                scanf("%d", &cmd);
                // не добавлены обработки ошибок
                char key[100];
                switch(cmd){
                        case Help:
                                manual();
                                break;
                        case Q:
                                printf("Bye!\n");
                                flag = 0;
                                break;
                        case SRCH:
                                scanf("%s", key);
                                ht_list* res = malloc(sizeof(ht_list));
                                res = ht_search(table, key);
                                if(res != NULL)
                                        list_print(res);
                                else
                                        printf("Nothing found\n");
                                break;
                        case ADD:
                                int value;
                                scanf("%s %d", key, &value);
                                ht_insert(table, value, key);
                                printf("ADDED successfully\n");
                                break;
			case LOAD:
				char file_name[500];
				scanf("%s", file_name);
				if(_load_db(file_name, table))
					printf("File not found\nPlease, make sure that the file is in the right folder and the name is correct.\n");
				else
					printf("LOADED successfully\n");
				
                        case DEL:
                                scanf("%s", key);
                                ht_delete(table, key);
                                break;
                        case PRNT:
                                ht_print(table);
                                break;
			case DMP:
				if (! _dump_db("database.txt", table))
					printf("DUMPED successfully\n");
				else
					printf("ERROR while dumping");
				break;

                        default:
                                printf("Unknown command\n");
                                break;
                }
        }
        //fclose(jr);
}

