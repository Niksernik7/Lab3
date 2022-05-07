#include "Menu.h"
char* enterString()
{
    char buf[81] = { 0 };
    char* res = NULL;
    size_t len = 0;
    int n = 0, rc = 0;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0)
        {
            if (!res)
            {
                return NULL;
            }
        }
        else if (n > 0) {
            size_t chunk_len = strlen(buf);
            size_t str_len = len + chunk_len;
            res = realloc(res, str_len + 2);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else {
            rc = scanf("%*c");
        }
    } while (n > 0);
    if (len > 0)
    {
        res[len] = '\0';
    }
    else {
        res = calloc(1, sizeof(char));
        if (res != NULL)
            *res = '\0';
    }
    return res;
}
void ShowFindMenu() {
    printf("Find\n");
    printf("Choice\n");
    printf("1) Find in table by release and key\n");
    printf("2) Find in table by key\n");
    printf("0) Exit\n");
    printf(" : ");
}

void ShowDeleteMenu() {
    printf("Delete\n");
    printf("Choice\n");
    printf("1) Delete by key\n");
    printf("2) Delete by release and key\n");
    printf("0) Exit\n");
    printf(" : ");
}

void ShowError() {
    printf("Miss choice\n");
}

void ShowMainMenu() {
    printf("Menu\n");
    printf("Choice\n");
    printf("1. Print table\n");
    printf("2. Insert item\n");
    printf("3. Delete item\n");
    printf("4. FindInTable item\n");
    printf("0. Quit\n");
    printf(" : ");
}

void printItem(const Item* i) {
    char* s;
    if (i != NULL) {
        s = get_str(i);
        printf("%s\n", s);
        free(s);
    }
    else {
        printf("Row with this key doesn't exist\n");
    }
}

void printTable(const Table* t) {
    char* str = NULL;
    if (t != NULL) {
        str = TransformTableString(t);
    } else {
        printf("Doesn't exist\n");
        return;
    }
    printf("Output\n");
    if (str != NULL) {
        printf("%s\n", str);
        free(str);
    }
}

// End pritfs

int GetInt() {
    char* str = "Doesn't number!\n : ";
    char* ptr;
    do {
        int a = 1;
        char* input = (char*) enterString();
        int n = (int)strtol(input, &ptr, 10);
        if (input == ptr - strlen(input) && strlen(input) != 0) {
            free(input);
            return n;
        }
        printf("%s", str);
        free(input);
    } while (1);
}

void Menu() {
    Table *table = Create();
    do {
        ShowMainMenu();
        int n = GetInt();
        switch (n) {
            case 0: {
                FreeTable(table);
                free(table);
                return;
            }
            case 1: {
                printTable(table);
                break;
            }
            case 2: {
                InsertInTable(table);
                break;
            }
            case 3: {
                ShowDeleteMenu();
                n = GetInt();
                Delete(table, n);
                break;
            }
            case 4: {
                ShowFindMenu();
                n = GetInt();
                FindInTable(table, n);
                break;
            }
            default: {
                ShowError();
                break;
            }
        }
    } while (1);
}

Table *Create() {
    printf("Enter size of table:\n");
    printf(" : ");
    char* str = "Must be greater then zero!!!\n : ";
    int size;
    do {
        size = GetInt();
	    if (size != 0) 
            break;
        printf("%s", str);
    } while (1);
    Table* table = TableCreate(size);
    return table;
}

void Delete(Table *table, int mod) {
    char* key, * msg = "";
    switch (mod) {
        case 0:
            break;

        case 1: {
            printf("Enter:\n");
            printf(" Key : ");
            do {
                printf("%s", msg);
                key = (char*) enterString();
                msg = "Key cannot be empty\n Key : ";
                if(strlen(key) != 0) break;
                free(key);
            } while (1);
            DeleteByKey(table, key);
            free(key);
            break;
        }
        case 2: {
            printf("Enter:\n");
            printf(" Key : ");
            char *key = (char*) enterString();
            printf(" Release : ");
            int release = GetInt();
            DeleteByReleaseKey(table, key, release);
            free(key);
            break;
        }
        default:
            ShowError();
            break;
    }
}

void InsertInTable(Table* table) {
    char* str = "Data cannot be empty\n Data : ";
    printf(" Enter\n");
    printf(" Data : ");
    char* data;
    do {
        data = (char*) enterString();
        if (strlen(data) != 0) 
            break;
        printf("%s", str);
        free(data);
    } while (1);
    printf(" Key : ");
    char* key;
    do {
        key = (char*) enterString();
        if (strlen(key) != 0) 
            break;
        free(key);
        printf("%s", str);
    } while (1);
    Item* item = InitItem(data, key);
    Insert(table, item);
    free(data);
    free(key);
}

void FindInTable(Table *table, size_t mod) {
    switch (mod) {
        case 0:
            break;
        case 1: {
            printf("Enter:\n");
            printf(" Key : ");
            char* key = (char*) enterString();
            printf(" Release : ");
            size_t release = GetInt();
            KeySpace* ks = FindByReleaseKey(table, key, release);
            if (ks != NULL) {
                free(ks->key);
                FreeItem(ks->data);
                free(ks->data);
                free(ks);
            }
            free(key);
            break;
        }
        case 2: {
            printf("Enter:\n");
            printf(" Key : ");
            char *key = (char*) enterString();
            Item *item = FindByKey(table, key);
            free(key);
            printItem(item);
            break;
        }
        default:
            ShowError();
            break;
    }
}
