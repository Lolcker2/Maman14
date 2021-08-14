#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>
#include "label_table.c"
#include "utils.c"




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



struct operation operation_table[28] = {
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


typedef enum    // need to check how many registers
{
    $0 = 0,
    $1 = 1,
    $2 = 2,
    $3 = 3,
    $4 = 4,
    $5 = 5,
    $6 = 6,
    $7 = 7,
    $8 = 8,
    $9 = 9,
    reg_none = -1

}registers;

struct register_item
{
    char* name;
    registers reg;
};

typedef struct line
{
    int line_num;   // number of the line
    char* file_name;    // name of the current file
}line_origin;

struct register_item register_table[11] =
        {
                {"$0", $0},
                {"$1", $1},
                {"$2", $2},
                {"$3", $3},
                {"$4", $4},
                {"$5", $5},
                {"$6", $6},
                {"$7", $7},
                {"$8", $8},
                {"$9", $9},
                {NULL, reg_none}
        };

typedef struct operation_input* op_in;

typedef struct operation_input
{
    char input_type; // 'i' for int , 's' for string , 'r' for register
    char *type_str; // for error messages
    int int_input;  // the input of type int
    char *str_input;    // the input of type str (char*)
    registers reg_input;    // the input of type registers
    op_in next; // next element
    op_in prev; // prev element

}op_input;



typedef struct memory_image_item* img;

typedef struct memory_image_item
{
	char *data;		// the actual data
	int value;	// the  value
	img next;   // next linked list structure
	img prev;   // preveius linked list structure
}image;




int add_memory_img(image *memory_img,char* data, int value)
{
    void* malloc_ress  = malloc(sizeof(image));  // get new pointer
    if(!malloc_ress) {
        //printf();
        return 0;
    }

    image *new_item = (image*)malloc_ress;    // cast the void* to a new memory image type
    (*new_item).data = data;    // set up the data
    (*new_item).value = value;  // set up the value
    (*memory_img).prev = new_item;  // set the prev one
    memory_img = (*memory_img).prev;    // update the head

    return 1;
}

registers is_register(char* reg, struct register_item registers_table[])
{
    for (int i = 0; i < 10; ++i) // size of the register table  // size of table?
    {
        if(strcmp(reg, registers_table[i].name) == 0) //current one
        {
            return registers_table[i].reg;
        }
    }
    return registers_table[10].reg;  // null

}

int add_op_input(op_input* table, char input_type, int input_int, char* input_str, registers input_reg, char* error_name)
{
    void* malloc_ress  = malloc(sizeof(op_in));  // get new pointer
    if(!malloc_ress) {
        //printf();
        return 0;
    }
    op_in new_item = (op_in)malloc_ress;    // cast the void* to a op_in  type
    new_item -> input_type = input_type;
    new_item ->type_str = error_name;   // for error messages

    if(input_type == 'r')   // if the input type is register, then save the register
        {
        new_item ->reg_input = input_reg; // updating new_item
        }

    if(input_type == 'i')   // if the input type is int, then save the int
        {
        new_item ->int_input = input_int; // updating new_item
        }

    if(input_type == 's')   // if the input type is string, then save the string
        {
        new_item-> str_input = input_str; // updating new_item
        }

    if(!table)    // first item
        {
            table = new_item;
            return 1;
        }   // not first item

        new_item -> prev = table;  // setting the prev item
        table -> next = new_item;  // setting the next item
        table = new_item;   // update the head
        return 1;
}


int process_operation(char* line, int i, struct operation oper, char* symbol, int ic, table *symbol_table,
        struct operation operations[], struct register_item regs[] , line_origin error_origin)
        {
            if(symbol[0] != '\0')
            {
                label temp;

                int label_found = find_label(symbol, symbol_table, temp);

                if(label_found == 1)
                {
                    fprint_error(error_origin, "Label was declared before");
                    return 0;
                }

                add_symbol_table(symbol, A_CODE, ic, symbol_table, error_origin);
            }

                int num_of_operands = 0;
                op_input *input_table;
            do {
                int j = 0;
                char scanned[MAX_LINE_LENGTH];
                char *temp, *trash_data;
                long number;

                for (; i < MAX_LINE_LENGTH && line[i] != EOF && line[i] && line[i] != '\t'
                       && line[i] != ',' && line[i] != ' ' && line[i] != '\n'; ++i, ++j) {
                    scanned[j] = line[i];
                }
                scanned[j] = '\0';
                registers reg;
                if(reg = is_register(scanned, register_table) != reg_none)    // current input is a register
                    {
                    add_op_input(input_table, 'r', 0, "str", reg, "register");   // the 0, "str" are not used
                    }
                else if(is_int(scanned) == 1)   // it's an int
                    {
                    // cast scanned to int
                    add_op_input(input_table, 'i', 0, "str", reg_none, "int") ;  // the "str", reg_none are not used
                    }
                else    // it's a string
                {
                    add_op_input(input_table, 's', 0, scanned, reg_none "char*");   // the 0, Null are not used
                }

                num_of_operands++;

                move_not_empty_char(line, i);
                if(line[i] == ',')
                {
                    i++;
                }

                else if(!line[i] || line[i] == '\n' || line[i] == EOF)
                {
                    break;
                }

                move_not_empty_char(line, i);
                if(line[i] == ',')
                {
                    fprint_error(error_origin, "Repetitive use of comma");
                    return 0;
                }
                else if(line[i] == '\n' || line[i] == EOF)
                {
                    fprint_error(error_origin, "Unexpected comma");
                    return 0;
                }

            }while(line[i] != '\n' && line[i] != EOF);

            if(oper.op_type == 'J')
            {
                if(oper.op_code != op_stop) // not stop operation
                    {
                    if(num_of_operands != 1)
                    {
                        fprint_error(error_origin, "Expected 1 operand, got %d", num_of_operands);  // might not need
                        return 0;
                    }

                    char *curr_op_code = int_to_bin(oper.op_code, );
                    char *curr_address = "";    // 00000...     // the address of the label
                    char *curr_register_address = "";    // 00000...     // the address of the register

                    if((*input_table).input_type == 'i')    // input is an int, not allowed
                        {
                        fprint_error(error_origin, "Expected register or label, got int");
                        return 0;
                        }

                    if((*input_table).input_type == 's')    // input is a string (label in this case)
                    {
                        label temp;
                        if(find_label((*input_table).str_input, symbol_table, temp) == 0)
                        {
                            fprint_error(error_origin, "Incorrect label naming");
                            return 0;
                        }
                        if(temp.attribute == A_EXTERNAL)     // external label
                            {

                            }
                        else
                        {       // non external label
                            curr_address = int_to_bin(temp.value, );  // setting the address of the label
                        }
                    }

                    else    // input is a register
                    {
                        registers temp;
                        temp = is_register((*input_table).reg_input , regs);
                                if(temp)
                                {
                                    curr_register_address = int_to_bin((int)temp, ) // setting up the address of the register
                                }
                    }
                    // ----------------------- bin ----------------------------
               }
                else    // stop operation
                {
                    if(num_of_operands != 0)
                    {
                        fprint_error(error_origin, "Expected 0 operands, got %d", num_of_operands);
                        return 0;
                    }

                    char *curr_op_code = int_to_bin(oper.op_code, );
                    // ----------------------- bin ----------------------------
                }
                return 1;
            }
            if(oper.op_type == 'I')
            {

			   if(num_of_operands != 3)
			   {
			       fprint_error(error_origin, "Expected 3 operands, got %d", num_of_operands);
			       return 0;
			   }

			   if(oper.op_code == op_addi || oper.op_code == op_ori || oper.op_code == op_andi ||oper.op_code == op_subi
			   || oper.op_code == op_nori)  // addi, subi, ori, andi, nori
			   {
			       registers rs;
			       registers rt;
			       int immediate;

			       if((*input_table).input_type != 'r')    // third input must be a register
			           {
			           fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
			           return 0;
			           }
			       rt = (*input_table).reg_input;

			       input_table = (*input_table).prev;   // number of inputs is 3

			       if((*input_table).input_type != 'i') // second input must be an int
			           {
			           fprint_error(error_origin, "Expected int type, got %s type", (*input_table).type_str);
			           return 0;
			           }

			       immediate = (*input_table).int_input;  // setting the immediate value

			       input_table = (*input_table).prev;   // number of inputs is 3

			       if((*input_table).input_type != 'r') // first input must be a register
			           {
			           fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
			           return 0;
			           }

			       rs = (*input_table).reg_input;

			       // ----------------------- bin ----------------------------

			       char *bin_rt = int_to_bin(rt, );
			       char *bin_rs = int_to_bin(rs, );
			       char *bin_immediate = int_to_bin(immediate, );

			       return 1;
			       }

			   // other ops
			   else if(oper.op_code == op_beq || oper.op_code == op_bne || oper.op_code == op_blt ||
			   oper.op_code == op_bgt)     // beq, bne, blt, bgt
			   {
			       char *curr_op_code = int_to_bin(oper.op_code, );
			       char *curr_address = "";    // 00000...     // the address of the label
			       char *label_address = "";    // 00000...     // the address of the register
			       registers rt;
			       registers rs;

			       if((*input_table).input_type == 's')    // third input must be a label (type string)
			           {
			            //fprint();
			            return 0;
			           }

			           label temp;
			           if(find_label((*input_table).str_input, symbol_table, temp) == 0)
			           {
			               fprint_error(error_origin, "Incorrect label naming");
			               return 0;
			           }
			           if(temp.attribute == A_EXTERNAL)     // input mustn't be an external label
			               {
                                //fprint();
                                return 0;
			               }
			           else
			           {       // non external label
			               label_address = int_to_bin(temp.value, );  // setting the address of the label
			           }

			           input_table = (*input_table).prev;   // number of inputs is 3

			           if((*input_table).input_type != 'r') // second input must be a register
			               {
			               fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
			               return 0;
			               }

			           rs = (*input_table).reg_input;

			           input_table = (*input_table).prev;   // number of inputs is 3

			           if((*input_table).input_type != 'r') // first input must be a register
			               {
			               fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
			               return 0;
			               }

			           rt = (*input_table).reg_input;

			           char* rt_bin = int_to_bin(rt, );
			           char* rs_bin = int_to_bin(rs, );

			           // --------------------- bin
			        return 1;
			   }
			   else //  lb, sb, lw, sw, lh, sh
			   {
			       registers rs;
			       registers rt;
			       int immediate;

			       if((*input_table).input_type != 'r')    // third input must be a register
			           {
			           fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
			           return 0;
			           }
			       rt = (*input_table).reg_input;

			       input_table = (*input_table).prev;   // number of inputs is 3

			       if((*input_table).input_type != 'i') // second input must be an int
			           {
			           fprint_error(error_origin, "Expected int type, got %s type", (*input_table).type_str);
			           return 0;
			           }

			       immediate = (*input_table).int_input;  // setting the immediate value

			       input_table = (*input_table).prev;   // number of inputs is 3

			       if((*input_table).input_type != 'r') // first input must be a register
			           {
			           fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
			           return 0;
			           }

			       rs = (*input_table).reg_input;

			       // ----------------------- bin ----------------------------

			       char *bin_rt = int_to_bin(rt, );
			       char *bin_rs = int_to_bin(rs, );
			       char *bin_immediate = int_to_bin(immediate, );

			       return 1;
			   }

            }
            if(oper.op_type == 'R')
            {
                if(oper.op_code == op_add) // add, sub, and ,or ,nor. same op_code
                {
                    if(num_of_operands != 3)
                    {
                        fprint_error(error_origin, "Expected 3 operands, got %d", num_of_operands);
                        return 0;
                    }

                    int i = 0;
                    registers rs;
                    registers rt;
                    registers rd;

                    if((*input_table).input_type != 'r')    // third input must be a register
                        {
                        fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
                        return 0;
                        }
                    rd = (*input_table).reg_input;

                    input_table = (*input_table).prev;   // number of inputs is 3

                    if((*input_table).input_type != 'r') // second input must be a register
                        {
                        fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
                        return 0;
                        }

                    rt = (*input_table).int_input;

                    input_table = (*input_table).prev;   // number of inputs is 3

                    if((*input_table).input_type != 'r') // first input must be a register
                        {
                        fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
                        return 0;
                        }

                    rs = (*input_table).reg_input;

                    // ----------------------- bin ----------------------------

                    char *bin_rd = int_to_bin(rd, );
                    char *bin_rt = int_to_bin(rt, );
                    char *bin_rs = int_to_bin(rs, );


                    return 1;
                }
                else	// move, mvhi, mvlo
                {
                    // bin ------------------------------------
                    if(num_of_operands != 2)
                    {
                        fprint_error(error_origin, "Expected 2 operands, got %d", num_of_operands);
                        return 0;
                    }
                    // bin ------------------------------------

                    return 1;
                }
            }
            return 0;   // at any other case (non valid error in 'look_for_operation') return
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