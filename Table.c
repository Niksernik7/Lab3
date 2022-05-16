#include "Table.h"

bool Insert(Table* table, Item* item) {
    size_t hashKey = hashFunc(item->key, table->msize);
    size_t maxRel = 0;
    for (size_t i = 0; i < table->msize; i++) {
        if (table->keySpace[i].key != NULL){
            if (strcmp(table->keySpace[i].key, item->key) == 0) {
                if (table->keySpace[i].release > maxRel)
                    maxRel = table->keySpace[i].release;
            }
        }
    }
    size_t i = hashKey;
    do {
        if (table->csize == table->msize)
            return false;
        if (table->keySpace[i].busy == 0) {
            if ((table->keySpace[i].key = strdup(item->key)) == NULL) //ENOMEM
                return false;
            table->keySpace[i].busy = 1;
            table->keySpace[i].data = item;
            table->keySpace[i].release = maxRel + 1;
            table->csize++;
            return true;
        }
        i = (i + 1) % table->msize;
    } while (i != hashKey);

    return 0;
}
Table* FindByKey(const Table* table, const char* key) {
    KeySpace* res = NULL;
    size_t nres = 0; // количество найденных элементов
    size_t hashKey = hashFunc(key, table->msize);
    size_t i = hashKey;
    do {
        if (table->keySpace[i].key != NULL && table->keySpace[i].busy != 0) {
            if (strcmp(key, table->keySpace[i].key) == 0) {
                KeySpace *tmp = realloc(res, (nres + 1) * sizeof(KeySpace));
                if (tmp == NULL)
                    return NULL;
                res = tmp;
                res[nres].key = strdup(key);
                if (res[nres].key == NULL) {
                    free(res);
                    return NULL;
                }
                res[nres].busy = table->keySpace[i].busy;
                res[nres].release = table->keySpace[i].release;
                res[nres].data = table->keySpace[i].data;
                nres++;
            }
        }

        i = (i + 1) % table->msize;
    } while (i != hashKey);
    if (res == NULL)
        return NULL;
    Table* t = calloc(1, sizeof(Table));
    if (t == NULL) {
        free(res);
        return NULL;
    }
    t->keySpace = res;
    t->msize = nres;
    t->csize = nres;
    return t;

}
KeySpace* FindByReleaseKey(const Table* table, const char* key, size_t release){
    KeySpace* res = NULL;
    size_t hashKey = hashFunc(key, table->msize);
    size_t i = hashKey;
    do {
        if (table->keySpace[i].key != NULL && table->keySpace[i].busy != 0) {
            if (strcmp(table->keySpace[i].key, key) == 0 &&
                (table->keySpace[i].release == release)) {
                res = calloc(1, sizeof(KeySpace));
                res->key = strdup(key);
                if (res->key == NULL) {
                    free(res);
                    return NULL;
                }
                res->busy = table->keySpace[i].busy;
                res->release = table->keySpace[i].release;
                res->data = table->keySpace[i].data;
                return res;
            }
        }
        i = (i + 1) % table->msize;
    } while (i != hashKey);
    return NULL;
}

bool DeleteByKey( Table* table,  const char* key){
    Table* ft = FindByKey(table, key);
    if (ft == NULL)
        return false;  // НЕТ ТАКИХ ЭЛЕМЕНТОВ
    for (size_t i = 0; i < table->msize; i++){
        for (size_t j = 0; j < ft->msize; j++){
            if (table->keySpace[i].key != NULL && table->keySpace[i].busy != 0) {
                if (strcmp(table->keySpace[i].key, ft->keySpace[j].key) == 0) {
                    free(table->keySpace[i].key);
                    table->keySpace[i].key = NULL;
                    if (table->keySpace[i].data != NULL){
                        free(table->keySpace[i].data->key);
                        free(table->keySpace[i].data->data);
                        free(table->keySpace[i].data);
                        table->keySpace[i].data = NULL;
                    }
                    table->keySpace[i].data = NULL;
                    table->keySpace[i].release = 0;
                    table->keySpace[i].busy = 0;
                    table->csize--;
                }
            }
        }
    }
    for (size_t i = 0; i < ft->msize; i++){
        free(ft->keySpace[i].key);
    }
    free(ft->keySpace);
    free(ft);
    return true;
}


bool DeleteByReleaseKey(Table* table, const char* key, size_t release){
    for (int i = 0; i < table->msize; i++) {
        if (table->keySpace[i].key != NULL && table->keySpace[i].busy != 0) {
            if (strcmp(table->keySpace[i].key, key) == 0 && (table->keySpace[i].release == release)) {
                free(table->keySpace[i].key);
                table->keySpace[i].key = NULL;
                if (table->keySpace[i].data != NULL){
                    free(table->keySpace[i].data->key);
                    free(table->keySpace[i].data->data);
                    free(table->keySpace[i].data);
                    table->keySpace[i].data = NULL;
                }
                table->keySpace[i].data = NULL;
                table->keySpace[i].release = 0;
                table->keySpace[i].busy = 0;
                table->csize--;
                return true;
            }
        }
    }
    return false;
}

Table* TableCreate(size_t size) {
    Table* table = calloc(1,  sizeof(Table));
    table->msize = size;
    table->csize = 0;
    table->keySpace = calloc(size, sizeof(KeySpace));
    return table;
}

size_t hashFunc(const char* s, size_t n) {
    size_t h = 2353 % n;
    for (size_t i = 0; i < strlen(s); i++) {
        h = (h + (h << 5) + s[i]) % n;
    }
    return h % n;
}

char* get_str(const Item* item) {
    if (item == NULL)
        return NULL;
    size_t key = strlen(item->key);
    size_t data = strlen(item->data);
    char* s = calloc(key + 3 + data + 1, sizeof(char));
    strcpy(s, item->key);
    strcat(s, " | ");
    strcat(s, item->data);
    return s;
}

Item* InitItem(const char* data, const char* key) {
    Item* item = calloc(1, sizeof(Item));
    item->data = calloc(strlen(data) + 1, sizeof(char));
    strcpy(item->data, data);
    item->key = calloc(strlen(key) + 1, sizeof(char));
    strcpy(item->key, key);
    return item;
}

void FreeTable(Table* table) {

    for (int i = 0; i < table->msize; i++) {
        KeySpace* ks = &table->keySpace[i];
        KeySpace* t = ks;
        free(t->key);
        if (t->data != NULL) {
            free(t->data->key);
            free(t->data->data);
            free(t->data);
        }
    }
    free(table->keySpace);
}

char* TransformTableString(const Table* table) {
    char* s = NULL, release[255] = {0}, busy[255] = {0};
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
            len = strlen(i);
            s = (char*)calloc(len + 3 + 4 + 1 , sizeof(char));
            snprintf(busy, 255,  "%d", ks->busy);
            strcpy(s, busy);
            strcat(s, " | ");
            strcat(s, i);
            strcat(s, " | ");
            snprintf(release, 255,  "%d", ks->release);
            s = realloc(s, (len + 3 + strlen(release) + 1 + 1 + 4) * sizeof(char));
            strcat(s, release);
            strcat(s, "\n");
            len = strlen(s);
        }
        else {
            size_t new_chunk = strlen(i);
            s = realloc(s, (len + new_chunk + 1 + 1 + 3 + 4) * sizeof(char));
            snprintf(busy, 255,  "%d", ks->busy);
            strcat(s, busy);
            strcat(s, " | ");
            strcat(s, i);
            strcat(s, " | ");
            snprintf(release, 255,  "%d", ks->release);
            s = realloc(s, (len + new_chunk + strlen(release) + 1 + 1 + 4 + 3) * sizeof(char));
            strcat(s, release);
            strcat(s, "\n");
            len = strlen(s);
        }
        free(i);
        ks++;
    }
    return s;
}