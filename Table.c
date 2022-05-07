#include "Table.h"




void* Insert(Table* table, Item* item) {
    size_t hashKey = hashFunc(item->key, table->msize);
    size_t maxRel = 0;
    for (size_t i = 0; i < table->msize; i++) {
        if (table->keySpace->key != NULL){
            if (strcmp(table->keySpace[i].key, item->key) == 0) {
                if (table->keySpace[i].release > maxRel)
                    maxRel = table->keySpace[i].release;
             }
        }
    }
    size_t i = hashKey;
    do {
            if (table->csize == table->msize)
                return NULL;
            if (table->keySpace[i].busy == 0) {
                if ((table->keySpace[i].key = strdup(item->key)) == NULL) //ENOMEM
                    return 0;
                table->keySpace[i].busy = 1;
                table->keySpace[i].data = item;
                table->keySpace[i].release = maxRel + 1;
                table->csize++;
                return 0;
            }
            i = (i + 1) % table->msize;
    } while (i != hashKey);

    errno = ENOBUFS;
    return 0;
}

Table* FindByKey(const Table* table, const char* key) {
    KeySpace* res = NULL;
    size_t nres = 0; // количество найденных элементов
    for (size_t i = 0; i < table->msize; i++){
        if (table->keySpace->key != NULL) {
            if (strcmp(key, table->keySpace[i].key) == 0) {
                KeySpace *tmp = realloc(res, (nres + 1) * sizeof(KeySpace));
                if (tmp == NULL)
                    return NULL;
                res = tmp;
                res[nres].key = strdup(key);
                if (res[nres].key == NULL)
                    goto err;
                res[nres].busy = table->keySpace[i].busy;
                res[nres].release = table->keySpace[i].release;
                res[nres].data = table->keySpace->data;
                nres++;
            }
        }
    }
    if (res == NULL)
        return NULL;
    Table* t = calloc(1, sizeof(Table));
    if (t == NULL) {
        goto err;
    }
    t->keySpace = res;
    t->msize = nres;
    return t;

err:
    free(res);
    return NULL;
}

// NEED CHECK
bool DeleteByKey(const Table* table,  const char* key){
    Table* ft = FindByKey(table, key);
    if (ft == NULL)
        return NULL;  // НЕТ ТАКИХ ЭЛЕМЕНТОВ
    for (size_t i = 0; i < table->msize; i++){
        for (size_t j = 0; j < ft->msize; i++){
            if (strcmp(table->keySpace[i].key, ft->keySpace[j].key) == 0 ||
            (table->keySpace[i].release == ft->keySpace[j].release))
                table->keySpace[i].busy = 0;
            return true;
        }
    }
    return false;
}

//NEED CHECK
KeySpace* FindByReleaseKey(const Table* table, const char* key, size_t release){
    KeySpace* res = NULL;
    for(size_t i = 0; i < table->msize; i++){
        if (strcmp(table->keySpace[i].key, key) == 0 &&
            (table->keySpace[i].release == release)) {
            res = calloc(1, sizeof(KeySpace));
            res->key = strdup(key);
            if ( res->key == NULL) {
                free(res);
                return NULL;
            }
            res->busy = table->keySpace[i].busy;
            res->release = table->keySpace[i].release;
            res->data = table->keySpace[i].data;
            return res;
        }
    }
    return NULL;
}
//NEED CHECK
bool DeleteByReleaseKey(const Table* table, const char* key, size_t release){
    if (table->csize == table->msize)
        return NULL;
    for (int i = 0; i < table->msize; i++) {
        if (strcmp(table->keySpace[i].key, key) == 0 && (table->keySpace[i].release == release)) {
            table->keySpace[i].busy = 0;
            return true ;
        }
    }
    return false;
}

 Table* TableCreate(size_t size) {
    Table* table = calloc(1,  sizeof(Table));
    table->msize = size;
    table->csize = 0;
    table->keySpace = calloc(table->msize, sizeof(KeySpace));
    return table;
}


 size_t hashFunc(const char* s, size_t n) {
    size_t h = 2353 % n;
    for (size_t i = 0; i < strlen(s); i++) {
        h = (h + (h << 5) + s[i]) % n;
    }
    return h % n;
}




 void CopyItem(Item* dist, const Item* src) {
    dist->data = (char*)calloc(strlen(src->data) + 1, sizeof(char));
    strcpy(dist->data, src->data);
    dist->key = (char*)calloc(strlen(src->key) + 1, sizeof(char));
    strcpy(dist->key, src->key);
}

 char* get_str(const Item* item) {
    if (item == NULL)
        return NULL;
    size_t key = strlen(item->key), data = strlen(item->data);
    char* s = calloc(key + 3 + data + 1, sizeof(char));
    strcpy(s, item->key);
    strcat(s, " | ");
    strcat(s, item->data);
    return s;
}

 Item* InitItem(const char* data, const char* key) {
    Item* item = malloc(sizeof(Item));
    item->data = calloc(strlen(data) + 1, sizeof(char));
    strcpy(item->data, data);
    item->key = calloc(strlen(key) + 1, sizeof(char));
    strcpy(item->key, key);
    return item;
}

 void FreeItem(Item* item) {
    free(item->data);
    free(item->key);
}



 void FreeTable(Table* table) {

    for (int i = 0; i < table->msize; i++) {
        KeySpace* ks = table->keySpace + i;
        KeySpace* t = ks;
        free(t->key);
    }
    free(table->keySpace);
}

 char* TransformTableString(const Table* table) {
    char* s = NULL, release[255] = {0};
    size_t len = 0;
    KeySpace* ks = table->keySpace;
    if (ks == NULL)
        return NULL;
    for (size_t j = 0; j < table->msize; j++) {
        Item* item = ks->data;
        char* i = get_str(item);
        if (i == NULL) {
            ks++;
            continue;
        }
        if (s == NULL) {
            len = strlen(i) + 1;
            s = (char*)calloc(len + 1, sizeof(char));
            strcpy(s, i);
            strcat(s, " | ");
            itoa(ks->release, release, 10);
            strcat(s, release);
            strcat(s, "\n");
        }
        else {
            size_t new_chunk = strlen(i) + 1;
            s = (char*)realloc(s, (len + new_chunk + 1) * sizeof(char));
            strcat(s, i);
            strcat(s, "\n");
            len += new_chunk + 1;
        }
        free(i);
        ks++;
    }
    return s;
}


