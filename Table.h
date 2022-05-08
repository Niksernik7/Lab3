#ifndef TABLE_H
#define TABLE_H
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"

typedef struct Item_ Item;
typedef struct KeySpace {
    char* key; 
    size_t release;
    Item* data;
    int busy;
} KeySpace;
struct Item_ {
    char* data;
    char* key;
};
typedef struct Table{
    KeySpace* keySpace;
    size_t msize;
    size_t csize;
} Table;
bool Insert(Table*, Item*);
Table* FindByKey(const Table*, const char*);
bool DeleteByKey(Table*, const char*);
KeySpace* FindByReleaseKey(const Table*, const char*, size_t);
bool DeleteByReleaseKey(Table*, const char*, size_t);



 Item* InitItem(const char*, const char*);
 size_t hashFunc(const char*, size_t);
 char* get_str(const Item *item);
 char* TransformTableString(const Table*);
 void FreeTable(Table*);
 Table* TableCreate(size_t);
#endif
