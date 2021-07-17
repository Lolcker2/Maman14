#include <stdio.h>
#include "utils.c"
#include "label_table.c"
#include "defs.c"


int first_pass_line(char* line, table *symbol_table, int DC, int IC, int line_number) {
    int line_index = 0;
    char *label;
    struct operation curr_operation;
    instruction instruct;

    move_not_empty_char(line, line_index);
    if (!line[line_index] || line[line_index] == EOF || line[line_index] == '\n' || line[line_index] == ';') {
        return 1;
    }

    if (look_for_label(line, line_index, label, line_number) == 0) {
        return 0;
    }

    move_not_empty_char(line, line_index);
    if (line[line_index] == '\n') {
        return 1;
    }

    instruct = look_for_instruction(line, line_index, instruction_table, line_number);
    if(instruct != NONE)
    {
        process_instruction(line, line_index, instruction_table, label, DC, symbol_table, line_number);
        return 1;
    }

    curr_operation = look_for_operation(line, line_index, operation_table, line_number);
    if(curr_operation == operation_table[27])
    {
        fprint_error("Invalid line, no instruction nor operation were found", line_number);
        return 0;
    }

    process_operation(line, line_index, curr_operation, label, IC, symbol_table);

    return 1;
}
