#include "HashTable.h"
#include "Addresses.h"

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

int db_interpret(char* buf, char** resp_buf) 
{
      flag = 0;

      ht* table = ht_create(1000, hashfunc);
      
      char* resp = malloc(RESPONSE_SIZE);

      _load_db("database.txt", table, resp);

      //FILE *jr = fopen("journal.txt", "r+");

      //printf("Hello! Please, enter a command or type 'H' for help:\n");

      //read_chars(n);
      char cmd = buf[0];
      buf += 1;
      switch(cmd)
      {
            case 'H':
                  manual(resp);
                  break;

            case 'Q':
                  strcpy(resp, "Bye!\n");
                  break;
            case 'S':
                  char* key = malloc(sizeof(buf));
                  strcpy(key, buf);
                  ht_list* res = malloc(sizeof(ht_list));
                  res = ht_search(table, key);
                  if(res == NULL)
                  {
                        strcpy(resp, "Nothing found\n");
                  }
                  else
                        list_print(res, resp); 
                  break;

            case 'A':
                  int value;
                  char* key = malloc(sizeof(buf));
                  int i = 0;
                  while(&buf != ' ' ){
                        key[i] = &buf;
                        buf++;
                        i++;
                  }
                  buf++;
                  value = atoi(buf);
                  ht_insert(table, value, key);
                  strcpy(resp, "Added successfully\n");
                  break;
            /*
            case 'L':
                  char file_name[500];
                  scanf("%s", file_name);
                  _load_db(file_name, table);
                  break;
              */    
            case 'D':
                  char* key = malloc(sizeof(buf));
                  strcpy(key, buf);
                  ht_delete(table, key);
                  break;

            case 'P':
                  ht_print(table, resp);
                  break;
                  
            case 'M':
                  _dump_db("database.txt", table);
                  break;

            case 'I':
                  char* key = malloc(sizeof(buf));
                  strcpy(key, buf);
                  if(incr_el(table, key))
                        strcpy(resp, "Element not found");
                  break;
            case 'E':
                  char* key = malloc(sizeof(buf));
                  strcpy(key, buf);
                  if(decr_el(table, key))
                        strcpy(resp, "Element not found");
                  break;
            case ' ': case '\n': case '\0':
                  break;

            default:
                  strcpy( resp, "Unknown command\n");
                  break;
      }
      resp_buf = *resp;
        //fclose(jr);
      return 0;
}