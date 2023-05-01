#include "HashTable.h"

//============================================================================================================

#define Help 'H'
#define Q    'Q'
#define SRCH 'S'
#define ADD  'A'
#define DEL  'D'
#define PRNT 'P'
#define LOAD 'L'
#define DMP  'M'

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


//============================================================================================================

int main() 
{
      ht* table = ht_create(1000, hashfunc);

      _load_db("database.txt", table);

      FILE *jr = fopen("journal.txt", "r+");

      printf("Hello! Please, enter a command or type 'Help':\n");

      char cmd;
      int flag = 1;

      while(flag)
      {
            char n = scanf("%c", &cmd);
            read_chars(n);
            char key[100];
            switch(cmd)
            {
                  case 'H':
                        manual();
                        break;

                  case 'Q':
                        printf("Bye!\n");
                        flag = 0;
                        break;
                  case 'S':
                        scanf("%s", key);
                        ht_list* res = malloc(sizeof(ht_list));
                        res = ht_search(table, key);
                        if(res == NULL)
                        {
                              printf("Nothing found\n");
                        }
                        else
                              list_print(res);
                        break;

                  case 'A':
                        int value;
                        scanf("%s %d", key, &value);
                        ht_insert(table, value, key);
                        printf("Added successfully\n");
                        break;

                  case 'L':
                        char file_name[500];
                        scanf("%s", file_name);
                        _load_db(file_name, table);
                        break;
                        
                  case 'D':
                        scanf("%s", key);
                        ht_delete(table, key);
                        break;

                  case 'P':
                        ht_print(table);
                        break;
                        
                  case 'M':
                        _dump_db("database.txt", table);
                        break;

                  case 'I':
                        scanf("%s", key);
                        if(incr_el(table, key))
                              printf("Element not found");
                        break;
                  case 'E':
                        scanf("%s", key);
                        if(decr_el(table, key))
                              printf("Element not found");
                        break;


                  default:
                        printf("Unknown command\n");
                        break;
            }
        }
        fclose(jr);
}