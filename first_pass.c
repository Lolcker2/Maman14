#include <stdio.h>
#include "utils.h"


int first_pass_line(char* line, table *symbol_table, long DC, long IC, image *operation_img,
                    image *data_img, line_origin error_origin) {

    int line_index = 0;
    char *label;
    struct operation curr_operation;
    instruction instruct;
    int error_flag = 1;     // 1: no error, 0: error has been found

    move_not_empty_char(line, line_index);
    if (!line[line_index] || line[line_index] == EOF || line[line_index] == '\n' || line[line_index] == ';') {
        return 1;
    }

    if (look_for_label(line, line_index, label, error_origin) == 0) {
        return 0;
    }

    move_not_empty_char(line, line_index);
    if (line[line_index] == '\n') {
        return 1;
    }

    instruct = look_for_instruction(line, line_index, instruction_table, error_origin);
    if(instruct != NONE)
    {
        if(process_instruction(line, line_index, instruct, label, DC, symbol_table, data_img, error_origin)  == 0)
        {
            error_flag = 0;
        }
        return error_flag;
    }

    curr_operation = look_for_operation(line, line_index, operation_table, error_origin);
    if(curr_operation.op_type == operation_table[27].op_type)
    {
        fprint_error(error_origin, "Invalid line, no instruction nor operation were found");
        return 0;
    }

    if(process_operation(line, line_index, curr_operation, label, IC, symbol_table, operation_table,
                         register_table, operation_img, error_origin) == 0)
    {
        error_flag = 0;
    }

    return error_flag;
}