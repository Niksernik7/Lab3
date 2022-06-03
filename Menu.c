#include "Menu.h"
char* ScanfString()
{
    const int bufsize = 1000;
    char* buf = malloc(bufsize);
    buf = fgets(buf, bufsize, stdin);
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
        buf[len - 1] = '\0';
    return buf;
}


void PrintItem(const Item* i) {
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

void PrintTable(const Table* t) {
    char* str = NULL;
    if (t != NULL) {
        str = TableIntoString(t);
    } else {
        printf("Doesn't exist\n");
        return;
    }
    printf("Output\n");
    if (str != NULL) {
        printf("%s\n", str);
        free(str);
    } else printf("Table is empty\n");
}

int FromStrToInt() {
    char* str = "Doesn't number!\n : ";
    char* ptr;
    do {
        char* input = (char*) ScanfString();
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
    Table *table = MakeTable();
    do {
        printf("Menu\n");
        printf("Choice\n");
        printf("1. Print table\n");
        printf("2. Insert item\n");
        printf("3. Delete item\n");
        printf("4. FindInTable item\n");
        printf("0. Quit\n");
        printf(" Input: ");
        int n = GetInt();
        switch (n) {
            case 0: {
                FreeTable(table);
                free(table);
                return;
            }
            case 1: {
                PrintTable(table);
                break;
            }
            case 2: {
                char* str = "Data cannot be empty\n Data : ";
                printf(" Enter\n");
                printf(" Data : ");
                char* data;
                do {
                    data = ScanfString();
                    if (strlen(data) != 0)
                        break;
                    printf("%s", str);
                    free(data);
                } while (1);
                printf(" Key : ");
                char* key;
                do {
                    key = ScanfString();
                    if (strlen(key) != 0)
                        break;
                    free(key);
                    printf("%s", str);
                } while (1);
                Item* item = InitItem(data, key);
                if (!Insert(table, item)) {
                    printf("Check count of elements in table, because it is full.\n ");
                    printf("If you are sure that it isn't full, then you dont have enough memory.\n");
                }
                free(data);
                free(key);
            }
                break;
            case 3: {
                printf("Delete\n");
                printf("Choice\n");
                printf("1.Delete by key\n");
                printf("2.Delete by release and key\n");
                printf("0.Exit\n");
                printf(" Input:");
                size_t mod = GetInt();
                char* key, * msg = "";
                switch (mod) {
                    case 0:
                        break;

                    case 1: {
                        printf("Enter:\n");
                        printf(" Key : ");
                        do {
                            printf("%s", msg);
                            key = ScanfString();
                            msg = "Key cannot be empty\n Key : ";
                            if(strlen(key) != 0) break;
                            free(key);
                        } while (1);
                        if(!DeleteByKey(table, key))
                            printf("Doesn't exist\n");
                        free(key);
                        break;
                    }
                    case 2: {
                        printf("Enter:\n");
                        printf(" Key : ");
                        key = ScanfString();
                        printf(" Release : ");
                        int release = GetInt();
                        if(!DeleteByReleaseKey(table, key, release))
                            printf("Doesn't exist\n");
                        free(key);
                        break;
                    }
                    default:
                        printf("Miss choice\n");
                        break;
                }
                break;
            }
            case 4: {
                printf("Find\n");
                printf("Choice\n");
                printf("1) Find in table by release and key\n");
                printf("2) Find in table by key\n");
                printf("0) Exit\n");
                printf(" Input: ");
                size_t mod = GetInt();
                switch (mod) {
                    case 0:
                        break;
                    case 1: {
                        printf("Enter:\n");
                        printf(" Key : ");
                        char* key = ScanfString();
                        printf(" Release : ");
                        size_t release = GetInt();
                        KeySpace* ks = FindByReleaseKey(table, key, release);
                        if (ks != NULL) {
                            PrintItem(ks->data);
                            free(ks->key);
                            free(ks);
                        } else printf("Doesn't exist\n");
                        free(key);
                        break;
                    }
                    case 2: {
                        printf("Enter:\n");
                        printf(" Key : ");
                        char *key = ScanfString();
                        Table *item = FindByKey(table, key);
                        free(key);
                        PrintTable(item);
                        for (size_t i = 0; i < item->msize; i++){
                            free(item->keySpace[i].key);
                        }
                        free(item->keySpace);
                        free(item);
                        break;
                    }
                    default:
                        printf("Miss choice\n");
                        break;
                }
                break;
            }
            default: {
                printf("Miss choice\n");
                break;
            }
        }
    } while (1);
}

Table *MakeTable() {
    printf("Enter size of table:\n");
    printf(" : ");
    char* str = "Must be greater then zero!!!\n : ";
    int size;
    do {
        size = FromStrToInt();
	    if (size != 0) 
            break;
        printf("%s", str);
    } while (1);
    Table* table = TableCreate(size);
    return table;
}

