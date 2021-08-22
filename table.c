#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "defs.h"
#include "label_table.h"

/*function that add to the attrebute of the symble another attrebute*/
void add_attribute_to_item(table item, char* attribute){
	
	ATTRIBUTE *temp ;
	temp = (ATTRIBUTE *) realloc(temp, sizeof(ATTRIBUTE));
	temp = attribute;
	item->symbol.second_attribute = temp;
	return;


}

/* the function search for the symbol and return the pointer to the symbol in symbol table */

table get_label(char* symbol, table *symbol_table)
{
    if(strcmp((*symbol_table)->symbol.name, symbol) == 0)   /* it's head */
    {
        
        return *symbol_table;
    }

    table *current_item = symbol_table; /* prevent aliasing */
    while((*current_item)->prev)
    {
        if(strcmp((*current_item)->symbol.name, symbol) == 0)   /* it's the curren one */
        {
            
            return *current_item;
        }
        *current_item = (*current_item)-> prev; /* go on */
    }
    return NULL;   /* not found return NULL */
}


int complete_img(image item){
	char *arr = item->data;
	int length = sizeof(arr)/sizeof(arr[0]);
	for (int i=0; i < length; i++){
		if(arr[i] == "?"){
			return 1;	
		}	
	}
	return 0;
}

int move_to_empty_char(char* line, int i)
{

    if (is_empty_char(line[i]) == 1) {
        return i;   // if the current char, return
    }

    for (; i < strlen(line) && i <= MAX_LINE_LENGTH;) {   // scan the line from the index
        i++;    // increment the index
        if (is_empty_char(line[i]) == 1) {
            return i;
        }
    }
    return i;
}

/*by given token the function serch in opertion table and return:
1 - name of operation (string)
2 - the type of operation (R,I,J)
3 - funct operation
4 - op operation*/

char* find_op_in_table(char *token,operation *operation_table, int j){
	int i;
	for (i = 0 ; i<28; i++){
		if (strcmp (operation_table[i][1] , token)==0){
			return operation_table[i][j];
		}	
	}
return 0;

}


bool fix_code_image(int curr_address, char* line, int i, image *item_img, table *symbol_table,operation *operation_table){
	
	char *token;
	char *symbol;
	table *curr_symbol;
	int val;
	char *bin;
	token = strtok(line + i, " \n\t");
	switch (find_op_in_table(token, operation_table, 2)){

		case 'R':
			fprint_error("Invalid line, operation type R should be complete.", line);
				return FALSE;
		case 'I':
			i = move_to_empty_char(line, i);
			i = move_not_empty_char(line, i);
			symbol = strtok(line + i, ",");
			symbol = strtok(NULL, ",");
			symbol = strtok(NULL, ",");
			curr_symbol = get_label(symbol, symbol_table);
			if(curr_symbol == NULL || (curr_symbol->symbol->attribute == A_EXTERNAL)){
				fprint_error("Invalid line, symbol not exist in symbol table or external.", line);
				return FALSE;
			}
			val = curr_symbol->symbol.data;
			val = val - curr_address;
			bin = int_to_bin(val,15);
			for (i=0;i<15;i++){
				item_img->data[i]=bin[i];
			}

		case 'J':
			if (token == "stop"){return FALSE;}
			i = move_to_empty_char(line, i);
			i = move_not_empty_char(line, i);
			symbol = strtok(line + i, " \n\t"); 
			curr_symbol = get_label(symbol, symbol_table);
			if(curr_symbol == NULL){
				fprint_error("Invalid line, symbol not exist in symbol table.", line);
				return FALSE;		
			}
			if(curr_symbol->symbol->attribute == A_EXTERNAL){ // need to check how external appeare
				add_address_to_ex_list(symbol, curr_symbol->symbol.data); //
			}
			val = curr_symbol->symbol->data;
			bin = int_to_bin(val,25);
			for (i=0;i<25;i++){
				item_img->data[i]=bin[i];
			}

	}
return TRUE;


}



