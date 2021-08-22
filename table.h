/* Implements a dynamically-allocated symbol table */
#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>

/*function that add to the attrebute of the symble another attrebute*/
void add_attribute_to_item(table item, char* attribute);
/* the function search for the symbol and return the pointer to the symbol in symbol table */

table get_label(char* symbol, table *symbol_table);

/*function that chack if there is a miss code in the img*/
int complete_img(image item);

/*function that move the pointer to the next empty char*/

int move_to_empty_char(char* line, int i);

/*by given token the function serch in opertion table and return:
1 - name of operation (string)
2 - the type of operation (R,I,J)
3 - funct operation
4 - op operation*/

char* find_op_in_table(char *token,operation *operation_table, int j);

/*function that add img code*/
bool fix_code_image(int curr_address, char* line, int i, image *item_img, table *symbol_table,operation *operation_table);


