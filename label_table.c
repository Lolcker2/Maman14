#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <malloc.h>



typedef enum attributes
{
    A_CODE,
    A_DATA,
    A_EXTERNAL,
    A_ENTRY
}ATTRIBUTE;


typedef struct label
{
    char *name;
    int value;
    ATTRIBUTE attribute;
    ATTRIBUTE second_attribute;
} label;

typedef struct label_link* table;

typedef struct label_link
{
    label symbol;
    table prev;
    table next;
} table_item;



int add_symbol_table(char* symbol, ATTRIBUTE attr, long value, table *symbol_table, line_origin error_origin)
{
    label temp = {symbol, value, attr}; /* create a label object from that data */
    void* malloc_ress  = malloc(sizeof(table));     /* get new pointer */
    if(!malloc_ress) {
        fprint_error(error_origin "Couldn't allocate enouth memory");
        return 0;
    }
    table new_item = (table)malloc_ress;    /* cast the void* to a table item  */
    new_item -> symbol = temp;  /* set up it's symbol */
    if(!(*symbol_table))    /* first item */
        {
        *symbol_table = new_item;
        return 1;
        }   /* not first item */
        new_item -> prev = *symbol_table;  /* setting the prev item */
        (*symbol_table) ->next = new_item;  /* setting the next item */
        *symbol_table = new_item;   /* update the head */
        return 1;
}


int find_label(char* symbol, table *symbol_table, label output_label)
{
    if(strcmp((*symbol_table)->symbol.name, symbol) == 0)   /* it's head */
        {
        output_label = (*symbol_table)->symbol;
        return 1;
        }

        table *current_item = symbol_table; /* prevent aliasing */
        while((*current_item)->prev)
        {
            if(strcmp((*current_item)->symbol.name, symbol) == 0)   /* it's the curren one */
                {
                output_label = (*current_item)->symbol; /* ser up the returned symbol */
                return 1;
                }
                *current_item = (*current_item)-> prev; /* go on */
        }
        return 0;   /* not found */
}
