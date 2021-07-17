#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <malloc.h>



typedef enum attributes
{
    A_CODE,
    A_DATA,
    A_EXTERNAL

}ATTRIBUTE;


typedef struct label
{
    char *name;
    int value;
    ATTRIBUTE attribute;
} label;

typedef struct label_link* table;

typedef struct label_link
{
    label symbol;
    table prev;
} table_item;



int add_symbol_table(char* symbol, ATTRIBUTE attr, int value, table *symbol_table, int line_number)
{
    label temp = {symbol, value, attr};
    void* malloc_ress  = malloc(sizeof(table));
    if(!malloc_ress) {
        //printf();
        return 0;
    }
    table new_item = (table)malloc_ress;
    new_item -> symbol = temp;
    if(!(*symbol_table))
    {
        *symbol_table = new_item;
        return 1;
    }
    new_item -> prev = *symbol_table;
    *symbol_table = new_item;
    return 1;
}


int find_label(char* symbol, table *symbol_table, label output_label)
{
    if(strcmp((*symbol_table)->symbol.name, symbol) == 0)
    {
        output_label = (*symbol_table)->symbol;
        return 1;
    }

    table *current_item = symbol_table;
    while((*current_item)->prev)
    {
        if(strcmp((*current_item)->symbol.name, symbol) == 0)
        {
            output_label = (*current_item)->symbol;
            return 1;
        }
        *current_item = (*current_item)-> prev;
    }
    return 0;
}
