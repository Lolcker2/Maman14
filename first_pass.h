#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H

// process 1 line as the first pass
int first_pass_line(char* line, table *symbol_table, long DC, long IC, image *operation_img,
                    image *data_img, line_origin error_origin);