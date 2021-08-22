#ifndef WRITEFILES_H
#define WRITEFILES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "table.h"
#include "utils.h"
#include "defs.h"
#include "label_table.h"

/*return the next binary num (4 cells in the array)*/
int * get_next_num(int * val, int * img_value, int i);


/*convert array of binary digits to hex*/
long int convert_bin_to_hex(int *binaryval);



/*write object file*/
bool write_ob(image *item_img, image *data_img, long icf, long dcf, char *filename) ;
/*write entries file*/
bool write_entries(table *symbol_table, char *filename);
/*write external file*/
bool write_external(table *symbol_table, char *filename);
/*check if entries file is needed*/
bool check_for_entries(table *symbol_table);
/*check if external file is needed*/
bool check_for_external(table *symbol_table);

