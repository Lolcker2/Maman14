#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>





typedef enum instruction_types
{
    DH,
    DW,
    DB,
    ASCIZ,
    ENTRY,
    EXTERN,
    NONE,
}instruction;

struct instruction_item
{
    char* name;
    instruction instruct;
};

struct instruction_item instruction_table[7] = {
        {"dh", DH},
        {"dw", DW},
        {"db", DB},
        {"asciz", ASCIZ},
        {"entry", ENTRY},
        {"extern", EXTERN},
        {NULL, NONE}
};

struct operation
{
    char* name;
    char op_type;
    int funct;
    int op_code;
};

typedef enum op_code{
    op_add = 0,
    op_sub = 0,
    op_and = 0,
    op_or = 0,
    op_nor = 0,

    op_move = 1,
    op_mvhi = 1,
    op_mvlo = 1,

    op_addi = 10,
    op_subi = 11,
    op_andi = 12,
    op_ori = 13,
    op_nori = 14,
    op_bne = 15,
    op_beq = 16,
    op_blt = 17,
    op_bgt = 18,
    op_lb = 19,
    op_sb = 20,
    op_lw = 21,
    op_sw = 22,
    op_lh = 23,
    op_sh = 24,
    op_jump = 30,
    op_la = 31,
    op_call = 32,
    op_stop = 63,
    op_none = 0
}OP_CODE;

typedef enum funct{
    fn_add = 1,
    fn_sub = 2,
    fn_and = 3,
    fn_or = 4,
    fn_nor = 5,

    fn_move = 1,
    fn_mvhi = 2,
    fn_mvlo = 3,

    fn_addi = 0,
    fn_subi = 0,
    fn_andi = 0,
    fn_ori = 0,
    fn_nori = 0,
    fn_bne = 0,
    fn_beq = 0,
    fn_blt = 0,
    fn_bgt = 0,
    fn_lb = 0,
    fn_sb = 0,
    fn_lw = 0,
    fn_sw = 0,
    fn_lh = 0,
    fn_sh = 0,
    fn_jump = 0,
    fn_la = 0,
    fn_call = 0,
    fn_stop = 0,
    fn_none = 0
}FUNCT;



struct operation operation_table[28] = {        // need to add num of params (operands)
        {"add", 'R', fn_add, op_add},
        {"sub", 'R', fn_sub, op_sub},
        {"and", 'R', fn_and, op_and},
        {"or", 'R', fn_or, op_or},
        {"nor", 'R', fn_nor, op_nor},
        {"move", 'R', fn_move, op_move},
        {"mvhi", 'R', fn_mvhi, op_mvhi},
        {"mvlo", 'R', fn_mvlo, op_mvlo},
        {"addi", 'I', fn_addi, op_addi},
        {"subi", 'I', fn_subi, op_subi},
        {"andi", 'I', fn_andi, op_andi},
        {"ori", 'I', fn_ori, op_ori},
        {"nori", 'I', fn_nori, op_nori},
        {"bne", 'I', fn_bne, op_bne},
        {"beq", 'I', fn_beq, op_beq},
        {"blt", 'I', fn_blt, op_blt},
        {"bgt", 'I', fn_bgt, op_bgt},
        {"lb", 'I', fn_lb, op_lb},
        {"sb", 'I', fn_sb, op_sb},
        {"lw", 'I', fn_lw, op_lw},
        {"sw", 'I', fn_sw, op_sw},
        {"lh", 'I', fn_lh, op_lh},
        {"sh", 'I', fn_sh, op_sh},
        {"jmp", 'J', fn_jump, op_jump},
        {"la", 'J', fn_la, op_la},
        {"call", 'J', fn_call, op_call},
        {"stop", 'J', fn_stop, op_stop},
        {NULL, 'N', fn_none, op_none},
};





int process_operation(char* line, int i, struct operation oper, char* symbol, int ic, table *symbol_table)
        {
            if(symbol[0] != '\0')
            {
                add_symbol_table(symbol, A_CODE, ic, symbol_table);
            }

            // for more need to read...
            return 0;
        }

/*

int main()
{

    int *ptr = (int*)malloc(sizeof(int));
    int ten = 10;
    ptr = &ten;
    printf("hey,  %d \n", *ptr);
    printf("size,  %d \n", (sizeof(operation_table)/ sizeof(struct operation)));

     need to check for '-'.
     if is, save it.
     then check that every thing is int and convert.



    int scanned[MAX_LINE_LENGTH];
    int *params;
    printf("scanned: %d \n", scanned[0]);

    if(*params == NULL)
    {
        printf("params is NUll");
    }
    return 0;
}
*/