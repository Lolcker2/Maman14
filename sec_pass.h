#ifndef SEC_PASS_H
#define SEC_PASS_H



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "table.h"
#include "utils.h"
#include "defs.h"
#include "label_table.h"



bool process_line_spass(char* line, image *item_img, table *symbol_table, long *ic, operation *operation_table) ;
