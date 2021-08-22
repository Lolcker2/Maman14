#ifndef ASSEMBLER_LABEL_TABLE_H
#define ASSEMBLER_LABEL_TABLE_H

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


int add_symbol_table(char* symbol, ATTRIBUTE attr, long value, table *symbol_table, line_origin error_origin);

