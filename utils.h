#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H


#include "defs.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdarg.h>


int is_empty_char(char c);


char* reverse_str(char* str);


char* int_to_bin(int num, int size);


int move_not_empty_char(char* line, int i);


int fprint_error(line_origin error_origin, char* error,...);


int str_alphanumeric(char* str);


int valid_label_name(char* name);


int look_for_label(char* line, int i, char* label, line_origin error_origin);


instruction look_for_instruction(char* line, int i, struct instruction_item instructions[], line_origin error_origin);


struct operation look_for_operation(char* line, int i, struct operation operations[], line_origin error_origin);


int is_int(char* num);


int process_instruction(char *line, int i, instruction instruct, char *symbol, long dc, table *symbol_table,
                        image data_image, line_origin error_origin);


int add_memory_img(image *memory_img,char* data, long value);


registers is_register(char* reg, struct register_item registers_table[]);


int add_op_input(op_input* table, char input_type, int input_int, char* input_str, registers input_reg, char* error_name);


int process_operation(char* line, int i, struct operation oper, char* symbol, long ic, table *symbol_table,
      struct operation operations[], struct register_item regs[] ,image *memory_img , line_origin error_origin);





