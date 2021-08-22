#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdarg.h>
#include "defs.c"
#include "label_table.c"




// max line should be changed
//assuming that the int ot bin function
// checks for non 'white' chars
// returns 1 if white , 0 if not
// params c : the char that's being checked
int is_empty_char(char c)
{
    if(c == ' ' || c == '\t'){return 1;}
    return 0;
}

// reverses the given string and returns it
char* reverse_str(char* str)
{
    char result[strlen(str)];
    int i = strlen(str) -1;
    for(int j = 0; j < strlen(str) && i >= 0; i--, j++) // 2 pointers: i and j
        if(str[i] != '\0') {    // so '\0' wont be first
            result[j] = str[i];
        }
    result[6] = '\0'; // closing the string

    char* returned = (char*)result; // casting to char*
    return  returned;
}


// converts num into a binary number with size digits.
// params: num , size: the number that be converted, number of digits of the binary number
char* int_to_bin(int num, int size)
{
    if(num < 0)		// if num is negative
        {
        num *= -1;
        int power_of_two = 2;
        for(int i =1; i<= size; i++)	// clac 2 to the power of size
            {
            power_of_two *= 2;
            }
        num = power_of_two - num;	// two's comliment
        }


    char result[size+1];    // size + 1 meaning '\0' at last index

    int i = 0;
    while(num > 0 && i < size)  // converting to binary
        {
        if(num % 2 == 1)
        {
            result[i] = '1';
        }else {result[i] = '0';}

        num = num / 2;
        i++;
        }

    for(; i < size; i++)   // adding 0's to fit the size
        {
        result[i] = '0';
        } result[i] = '\0'; // closing the string

        return reverse_str(result); // reversing it
}


// moves to the next, closes non white char
// params line, i: the current line, the index of the line scan
int move_not_empty_char(char* line, int i)
{

    if (is_empty_char(line[i]) == 0) {
        return i;   // if the current char, return
    }

    for (; i < strlen(line) && i <= MAX_LINE_LENGTH;) {   // scan the line from the index
        i++;    // increment the index
        if (is_empty_char(line[i]) == 0) {
            return i;
        }
    }
    return i;
}

int fprint_error(line_origin error_origin, char* error,...)
{
    printf("ERROR in file %s, line %d : ", error_origin.file_name, error_origin.line_num);
    va_list args;
    va_start(args, error);
    vprintf(error, args);
    va_end(args);
    printf("\n");
    return 0;
}

// checks whether a string contains only alpha numeric symbols
// params str: the string that's being checked
int str_alphanumeric(char* str)
{
    for(int i = 0; i < strlen(str); ++i)
    {
        if(isalpha(str[i]) == 0 && isdigit(str[i]) == 0) {return 0;}    // if not alpha nor digit then return
    }return 1;  // else
}

int not_register_or_function(char *name, instruction_item instructs[], operation opers[], struct register_item regs[])
{
    for (int i = 0; i < 6; ++i) // for each item in instruct, size of instructs is 6 (not including none)
    {
        if(strcmp(name, instructs[i].name) == 0)    // is instruction
        {
            return 0;
        }
    }

    for (int i = 0; i < 27; ++i) // for each item in opers, size of opers is 27 (not including null)
        {
        if(strcmp(name, opers[i].name) == 0)    // is operation
        {
            return 0;
        }

            if(is_register(name, regs) == regs[32]) // register null
            {
                return 1;
            }

            return 0;   // it's a register
        }
}

// checks whether a name is valid for a label
// param name: the name that's being checked
int valid_label_name(char* name)
{
    int check1 = strlen(name) <= 31? 1 : 0;     // not too long
    int check2 = isalpha(name[0]) == 0? 0 : 1;  // first char is alpha
    int check3 = str_alphanumeric(name);       // other chars are alpha numeric
    int not_register_or_function;     // not operation nor register nor instruction

    if(check1 == 1 && check2 == 1 && check3 == 1 && check4 == 1)
    {
        return 1;
    }

    return 0;
}


// checks whether a label exists in that line, and checks for validation
// params line, i, label : the current line, the index of line scan, the place to store the name
int look_for_label(char* line, int i, char* label, line_origin error_origin)
{
    int line_len = (int)(strlen(line));    // max length
    char scanned[line_len];        // saves the name of the label
    int j = 0;
    i = move_not_empty_char(line, i);
    for(; i < line_len && line[i] != ':' && line[i] && line[i] != EOF; ++j)
    {
        scanned[j] = line[i];       // update the name of the label
    }
    label = scanned;    // updates label
    label[j] = '\0';    // end of string

    if(line[i] == ':')      // meaning a label has been found
    {
        if(valid_label_name(line) == 0) // not valid, error and return
        {
            fprint_error(error_origin, "Invalid label name");
            return 0;
        }
    }
    return 1;
}


instruction look_for_instruction(char* line, int i, struct instruction_item instructions[], line_origin error_origin)
{
    move_not_empty_char(line, i);
    if(line[i] != '.')
    {
        return NONE;
    }
    int j = 0;
    char scanned[MAX_LINE_LENGTH];
    char *name;
    for(; j < 6 && line[i] != ' ' && line[i] != '\n' && line[i] && line[i] != EOF && line[i] != '\t'; j++ , ++i)
    {
        scanned[j] = line[i];
    }
    scanned[j] = '\0';
    name = scanned;

    for (int j = 0; j < 7; ++j) {
        if(strcmp(instructions[j].name, name) == 0)
        {
            return instructions[j].instruct;
        }
    }
    fprint_error(error_origin, "Invalid instruction");
    return NONE;
}


struct operation look_for_operation(char* line, int i, struct operation operations[], line_origin error_origin)
{
    move_not_empty_char(line, i);
    char scanned[MAX_LINE_LENGTH];
    int j = 0;
    char *name;
    for(; j < 6 && line[i] != ' ' && line[i] != '\n' && line[i] && line[i] != EOF && line[i] != '\t'; j++ , ++i)
    {
        scanned[j] = line[i];
    }
    scanned[j] = '\0';
    name = scanned;

    for (int 2
    j = 0; j < 28; ++j) {
        if(strcmp(operations[j].name, name) == 0)
        {
            return operations[j];
        }
    }
    fprint_error(error_origin, "Invalid operation");
    return operations[27];  // null
}


int is_int(char* num)
{
    int i;
    if(num[0] == '-')
    {
        i = 1;  // avoid the '-' char
    }
    else
    {
        i = 0;
    }
    for (; i < strlen(num); ++i) {
        if(isdigit(num[i]) == 0)
        {
            return 0;   // not an int
        }
    }
    return 1;   // it's an int
}


int process_instruction(char *line, int i, instruction instruct, char *symbol, long dc, table *symbol_table, image data_image, line_origin error_origin)
{
    if(instruct == ASCIZ)
    {
        if(symbol[0] != '\0')
        {
            label temp;

            int label_found = find_label(symbol, symbol_table, temp);

            if(valid_label_name(temp) == 0)
            {
                fprint_error(error_origin, "Invalid label");
                return 0;
            }

            if(label_found == 1)
            {
                fprint_error(error_origin, "Label was declared before");
                return 0;
            }
            add_symbol_table(symbol, A_DATA, dc, symbol_table);
            dc++;
        }

        move_not_empty_char(line, i);
        if(line[i] != '"')
        {
            fprint_error(error_origin, "Missing opening quotation mark");
            return 0;
        } i++;

        int j = 0;
        char scanned[MAX_LINE_LENGTH];
        char *param;
        for (; i < MAX_LINE_LENGTH && line[i] && line[i] != '\n' && line[i] !='\t' && line[i] != EOF && line[i] != ' ' && line[i] != '"'; ++i, ++j) {
            scanned[j] = line[i];
        }
        scanned[j] = '\0';
        if(scanned[0] == '\0')
        {
            fprint_error(error_origin, "Missing string after quotation mark");		// eed to check example 2
            return 0;		
        }

        if(scanned[j-1] != '"')		// need to change that
        {
            fprint_error(error_origin, "Missing closing quotation mark");
            return 0;
        }

        param = scanned;    // param is the string itself

        for(int i =0; i < strlen(param); ++i)   // for every character
        {
            char character = int_to_bin((int)(param[i]), ); // cast it into binary
            add_memory_img(data_image, character, dc);  // add to the data image
            dc++;   // increment dc
        }
    }

    if(instruct == EXTERN)
    {
        int j = 0;
        char scanned[MAX_LINE_LENGTH];
        char *param;

        move_not_empty_char(line, i);
        for (; i < MAX_LINE_LENGTH && line[i] && line[i] != '\n' && line[i] !='\t' && line[i] != EOF && line[i] != ' '; ++i, ++j) {
            scanned[j] = line[i];
        }
        scanned[j] = '\0';

        if(scanned[0] == '\0')
        {
            fprint_error(error_origin, "Invalid label after .extern");
            return 0;
        }
        param = scanned;

        label temp;

        int label_found = find_label(param, symbol_table, temp);

        if(valid_label_name(temp) == 0)
        {
            fprint_error(error_origin, "Invalid label");
            return 0;
        }

        if(label_found == 1 && temp.attribute != A_EXTERNAL)
        {
            fprint_error(error_origin, "Label was declared before, as external label");
            return 0;
        }

        add_symbol_table(param, A_EXTERNAL, 0, symbol_table);
    }

    if(instruct == DH || instruct == DB || instruct == DW)  // DATA INSTRUCTION
    {

        if(symbol[0] != '\0')
        {
            label temp;

            int label_found = find_label(symbol, symbol_table, temp);

            if(valid_label_name(temp) == 0)
            {
                fprint_error(error_origin, "Invalid label");
                return 0;
            }

            if(label_found == 1)
            {
                fprint_error(error_origin, "Label was declared before");
                return 0;
            }

            add_symbol_table(symbol, A_DATA, dc, symbol_table);
            dc++;
        }

        move_not_empty_char(line, i);
        if(line[i] == ',')
        {
            fprint_error(error_origin, "Unexpected comma after .data instruction");
            return 0;
        }

        do {
            int j = 0;
            char scanned[MAX_LINE_LENGTH];
            char *temp, *trash_data;
            int number;
			int size;
			char* value;

            for (; i < MAX_LINE_LENGTH && line[i] != EOF && line[i] && line[i] != '\t'
                   && line[i] != ',' && line[i] != ' ' && line[i] != '\n'; ++i, ++j) {
                scanned[j] = line[i];
            }
            scanned[j] = '\0';
            temp = scanned;
            if(is_int(temp) == 0)
            {
                fprint_error(error_origin, "Invalid number for .data instruction");
                return 0;
            }
            number = strtol(temp, &trash_data, 10); // making the number an int
            if(instruct == DH)
            {
				size = 16;
				value = int_to_bin(number, size);
				add_memory_img(data_image, value, dc);
				dc++;
			
            }
            else if(instruct == DB)
            {
				size = 8;
				value = int_to_bin(number, size);
				add_memory_img(data_image, value, dc);
				dc++;
            }

            else    //instruct is DW
            {
				size =  32;
				value = int_to_bin(number, size);
				add_memory_img(data_image, value, dc);
				dc++;
            }


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

        return 1;
    }

    if(instruct == ENTRY)
    {
        if(symbol[0] != '\0')
        {
            fprint_error(error_origin, "Unexpected label at .entry instruction");
            return 0;
        }
    }

    return 0;   // not succeed
}


int add_memory_img(image *memory_img,char* data, long value)
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
    return registers_table[32].reg;  // null

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


        int process_operation(char* line, int i, struct operation oper, char* symbol, long ic, table *symbol_table,
                struct operation operations[], struct register_item regs[] ,image *memory_img , line_origin error_origin)
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
        if(oper.op_code == op_jump) // jump opeation
            {
            if(num_of_operands != 1)
            {
                fprint_error(error_origin, "Expected 1 operand, got %d", num_of_operands);  // might not need
                return 0;
            }

            char *curr_op_code = int_to_bin(oper.op_code, 6);
            char input;     // 1 \ 0 the register field in the bin convertion
            char *curr_address = "";    // 00000...     // the address of the label

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
                    curr_address = "0000000000000000000000000"; // 25 bit of 0
                    }
                else
                {       // non external label
                    curr_address = int_to_bin(temp.value, 25);  // setting the address of the label

                }
                input = '0';
                }

            else    // input is a register
            {
                registers temp;
                temp = is_register((*input_table).reg_input , regs);
                if(!temp)
                {
                    //printf();
                    return 0;
                }
                curr_address = int_to_bin((int)temp, 25); // setting up the address of the register
                input = '1';
            }
            // ----------------------- bin ----------------------------
            char data[OPERATION_LENGTH];
            int i = 0;
            for (; i < 25; i++)
            {
                data[i] = curr_address[i];
            }
            i = 25;
            data[i] = input;
            for(; i < 32; i++)
            {
                data[i] = curr_op_code[i];
            }

            add_memory_img(data, ic);

            }
        else if(oper.op_code ==  op_stop)   // stop operation
            {
            if(num_of_operands != 0)
            {
                fprint_error(error_origin, "Expected 0 operands, got %d", num_of_operands);
                return 0;
            }

            char *curr_op_code = int_to_bin(oper.op_code, 6);
            char *curr_address "0000000000000000000000000"; // 25 bit of 0
            char input = '0';

            char data[OPERATION_LENGTH];
            int i = 0;
            for (; i < 25; i++)
            {
                data[i] = curr_address[i];
            }
            i = 25;
            data[i] = input;
            for(; i < 32; i++)
            {
                data[i] = curr_op_code[i];
            }

            add_memory_img(data, ic);
            ic+=4;

            }
        else    // la, call
        {
            char *curr_address;

            if((*input_table).input_type == 'i')    // can't be an int
                {
                fprint_error(error_origin, "Expected label type ,got int type");
                return 0;
                }
            if((*input_table).input_type == 'r')    // can't be a register
                {
                fprint_error(error_origin, "Expected label type ,got register type");
                return 0;
                }

            label temp;
            if(find_label((*input_table).str_input, symbol_table, temp) == 0)
            {
                fprint_error(error_origin, "Incorrect label naming");
                return 0;
            }
            if(temp.attribute == A_EXTERNAL)     // data completion at second pass
                {
                    char* curr_op_code = int_to_bin(oper.op_code, 6);
                    char data[OPERATION_LENGTH];
                    char input = '0';
                    int i = 0;
                    for (; i < 25; i++)
                    {
                        data[i] = '?';
                    }
                    i = 25;
                    data[i] = input;
                    for(; i < 32; i++)
                    {
                        data[i] = curr_op_code[i];
                    }

                    add_memory_img(memory_img, data, ic);
                    ic+=4;
                    return 1;
                }
            else
            {       // non external label
                curr_address = int_to_bin(temp.value, 25);  // setting the address of the label
            }

            char input = '0';
            char* curr_op_code = int_to_bin(oper.op_code, 6);
            char data[OPERATION_LENGTH];
            int i = 0;
            for (; i < 25; i++)
            {
                data[i] = curr_address[i];
            }
            i = 25;
            data[i] = input;
            for(; i < 32; i++)
            {
                data[i] = curr_op_code[i];
            }

            add_memory_img(memory_img, data, ic);
            ic+=4;
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

            char *bin_immediate = int_to_bin(immediate, 16);
            char *bin_rt = int_to_bin(rt, 5);
            char *bin_rs = int_to_bin(rs, 5 );
            char *curr_op_code = int_to_bin(oper.op_code, 6);
            char* data;
            int i = 0;
            for(;i < 16; i++)
            {
                data[i] = bin_immediate[i];
            }
            i = 16;
            for(;i < 21; i++)
            {
                data[i] = bin_rt[i];
            }
            i = 21;
            for(;i < 26; i++)
            {
                data[i] = bin_rs[i];
            }
            i = 26;
            for(;i < 32; i++)
            {
                data[i] = curr_op_code[i];
            }
            add_memory_img(memory_img, data, ic);
            ic += 4;

            return 1;
            }

        else if(oper.op_code == op_beq || oper.op_code == op_bne || oper.op_code == op_blt ||
        oper.op_code == op_bgt)     // beq, bne, blt, bgt
            {
            registers rt;
            registers rs;
            label temp;

            if((*input_table).input_type != 's')    // third input must be a label (type string)
                {
                    fprint_error(error_origin, "Expected label, got", (*input_table).type_str);
                return 0;
                }

            if(find_label((*input_table).str_input, symbol_table, temp) == 0)
            {
                fprint_error(error_origin, "Incorrect label naming");
                return 0;
            }
            if(temp.attribute == A_EXTERNAL)     // data completion at second pass
                {
                char* curr_op_code = int_to_bin(oper.op_code, 6);
                char data[OPERATION_LENGTH];
                char input = '0';
                int i = 0;
                for (; i < 25; i++)
                {
                    data[i] = '?';
                }
                i = 25;
                data[i] = input;
                for(; i < 32; i++)
                {
                    data[i] = curr_op_code[i];
                }

                add_memory_img(memory_img, data, ic);
                ic+=4;
                return 1;
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

            int curr_address_dist = temp.value - ic; // label address - current address
            char *bin_immediate = int_to_bin(curr_address_dist, 16);
            char *bin_rt = int_to_bin(rt, 5);
            char *bin_rs = int_to_bin(rs, 5 );
            char *curr_op_code = int_to_bin(oper.op_code, 6);
            char* data;
            int i = 0;
            for(;i < 16; i++)
            {
                data[i] = bin_immediate[i];
            }
            i = 16;
            for(;i < 21; i++)
            {
                data[i] = bin_rt[i];
            }
            i = 21;
            for(;i < 26; i++)
            {
                data[i] = bin_rs[i];
            }
            i = 26;
            for(;i < 32; i++)
            {
                data[i] = curr_op_code[i];
            }
            add_memory_img(memory_img, data, ic);
            ic += 4;
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



            char *bin_immediate = int_to_bin(immediate, 16);
            char *bin_rt = int_to_bin(rt, 5);
            char *bin_rs = int_to_bin(rs, 5 );
            char *curr_op_code = int_to_bin(oper.op_code, 6);
            char* data;
            int i = 0;
            for(;i < 16; i++)
            {
                data[i] = bin_immediate[i];
            }
            i = 16;
            for(;i < 21; i++)
            {
                data[i] = bin_rt[i];
            }
            i = 21;
            for(;i < 26; i++)
            {
                data[i] = bin_rs[i];
            }
            i = 26;
            for(;i < 32; i++)
            {
                data[i] = curr_op_code[i];
            }
            add_memory_img(memory_img, data, ic);
            ic += 4;

            return 1;
        }

    }   // there
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


            char *bin_rd = int_to_bin(rd, 5);
            char *bin_rt = int_to_bin(rt, 5);
            char *bin_rs = int_to_bin(rs, 5 );
            char *curr_op_code = "000000";  // 6 bit of 0
            char *curr_funct = int_to_bin(oper.funct, 5);
            char *not_in_use = "00000"; // 5 bit of 0

            char* data;
            int i = 0;
            for(;i < 6; i++)
            {
                data[i] = not_in_use[i];
            }
            i = 6;
            for(;i < 11; i++)
            {
                data[i] = curr_funct[i];
            }
            i = 11;
            for(;i < 16; i++)
            {
                data[i] = bin_rd[i];
            }
            i = 16;
            for(;i < 21; i++)
            {
                data[i] = bin_rt[i];
            }
            i = 21;
            for(;i < 26; i++)
            {
                data[i] = bin_rs[i];
            }
            i = 26;
            for(;i < 31; i++)
            {
                data[i] = curr_op_code[i];
            }

            add_memory_img(memory_img, data, ic);
            ic += 4;

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
            if((*input_table).input_type != 'r') // second input must be a register
                {
                fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
                return 0;
                }

            input_table = (*input_table).prev;   // number of inputs is 2

            if((*input_table).input_type != 'r') // first input must be a register
                {
                fprint_error(error_origin, "Expected register type, got %s type", (*input_table).type_str);
                return 0;
                }

            //bin ----------------------------------------
            char *bin_rd = int_to_bin(rd, 5);
            char *bin_rt = "00000"; // 5 bit of 0
            char *bin_rs = int_to_bin(rs, 5 );
            char *curr_op_code = "000000";  // 6 bit of 0
            char *curr_funct = int_to_bin(oper.funct, 5);
            char *not_in_use = "00000"; // 5 bit of 0

            char* data;
            int i = 0;
            for(;i < 6; i++)
            {
                data[i] = not_in_use[i];
            }
            i = 6;
            for(;i < 11; i++)
            {
                data[i] = curr_funct[i];
            }
            i = 11;
            for(;i < 16; i++)
            {
                data[i] = bin_rd[i];
            }
            i = 16;
            for(;i < 21; i++)
            {
                data[i] = bin_rt[i];
            }
            i = 21;
            for(;i < 26; i++)
            {
                data[i] = bin_rs[i];
            }
            i = 26;
            for(;i < 31; i++)
            {
                data[i] = curr_op_code[i];
            }

            add_memory_img(memory_img, data, ic);
            ic += 4;
            return 1;
        }
    }
    return 0;   // at any other case (non valid error in 'look_for_operation') return
                        }




