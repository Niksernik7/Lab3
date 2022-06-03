#ifndef MENU_H
#define MENU_H
#include "Table.h"
int GetInt();
void FindInTable(Table*, size_t);
void Menu();
Table* MakeTable();
void Delete(Table*, int);
void InsertInTable(Table *table);
#endif
