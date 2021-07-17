#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "defs.c"
#define MAX_LINE_LENGTH 100

// max line should be changed

// checks for non 'white' chars
// returns 1 if white , 0 if not
// params c : the char that's being checked
int is_empty_char(char c)
{
    if(c == ' ' || c == '\t'){return 1;}
    return 0;
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

int fprint_error(char* error, int line)
{
    printf("ERROR in line %d", line);
    printf(" : ");
    printf(" %s", error);

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

// checks whether a name is valid for a label
// param name: the name that's being checked
int valid_label_name(char* name)
{
    int check1 = strlen(name) <= 31? 1 : 0;     // not too long
    int check2 = isalpha(name[0]) == 0? 0 : 1;  // first char is alpha
    int check3 = str_alphanumeric(name);       // other chars are alpha numeric
    int check4;     // not op code nor register

    if(check1 == 1 && check2 == 1 && check3 == 1 && check4 == 1)
    {
        return 1;
    }

    return 0;
}


// checks whether a label exists in that line, and checks for validation
// params line, i, label : the current line, the index of line scan, the place to store the name
int look_for_label(char* line, int i, char* label, int line_number)
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
            fprint_error("Invalid label name",line_number);
            return 0;
        }
    }
    return 1;
}


instruction look_for_instruction(char* line, int i, struct instruction_item instructions[], int line_number)
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
    fprint_error("Invalid instruction", line_number);
    return NONE;
}


struct operation look_for_operation(char* line, int i, struct operation operations[], int line_number)
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

    for (int j = 0; j < 28; ++j) {
        if(strcmp(operations[j].name, name) == 0)
        {
            return operations[j];
        }
    }
    fprint_error("Invalid operation", line_number);
    return operations[27];  // null
}


int is_int(char* num)
{
    int i;
    if(num[0] == '-')
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    for (; i < strlen(num); ++i) {
        if(isdigit(num[i]) == 0)
        {
            return 0;
        }
    }
    return 1;
}


int process_instruction(char *line, int i, instruction instruct, char *symbol, int dc, table *symbol_table, int line_number)
{
    if(instruct == ASCIZ)
    {
        if(symbol[0] != '\0')
        {
            label temp;

            int label_found = find_label(symbol, symbol_table, temp);

            if(valid_label_name(temp) == 0)
            {
                fprint_error("Invalid label",line_number);
                return 0;
            }

            if(label_found == 1)
            {
                fprint_error("Label was declared before",line_number);
                return 0;
            }
            add_symbol_table(symbol, A_DATA, dc, symbol_table);
            dc++;
        }

        move_not_empty_char(line, i);
        if(line[i] != '"')
        {
            fprint_error("Missing opening quotation mark",line_number);
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
            fprint_error("Missing string after quotation mark",line_number);
            return 0;
        }

        if(scanned[j-1] != '"')
        {
            fprint_error("Missing closing quotation mark",line_number);
            return 0;
        }

        param = scanned;

        // add param to the data image
        dc++;
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
            fprint_error("Invalid label after .extern",line_number);
            return 0;
        }
        param = scanned;

        label temp;

        int label_found = find_label(param, symbol_table, temp);

        if(valid_label_name(temp) == 0)
        {
            fprint_error("Invalid label",line_number);
            return 0;
        }

        if(label_found == 1 && temp.attribute != A_EXTERNAL)
        {
            fprint_error("Label was declared before, as external label",line_number);
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
                fprint_error("Invalid label",line_number);
                return 0;
            }

            if(label_found == 1)
            {
                fprint_error("Label was declared before",line_number);
                return 0;
            }

            add_symbol_table(symbol, A_DATA, dc, symbol_table);
            dc++;
        }

        move_not_empty_char(line, i);
        if(line[i] == ',')
        {
            fprint_error("Unexpected comma after .data instruction",line_number);
            return 0;
        }

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
            temp = scanned;
            if(is_int(temp) == 0)
            {
                fprint_error("Invalid number for .data instruction",line_number);
                return 0;
            }
            number = strtol(temp, &trash_data, 10);
            //add number to data image with dc(DH, DB, DW)
            if(instruct == DH)
            {

            }

            else if(instruct == DB)
            {

            }

            else    //instruct is DW
            {

            }

            // incroment dc

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
                fprint_error("Repetitive use of comma",line_number);
                return 0;
            }
            else if(line[i] == '\n' || line[i] == EOF)
            {
                fprint_error("Unexpected comma",line_number);
                return 0;
            }

        }while(line[i] != '\n' && line[i] != EOF);

        return 1;
    }

    if(instruct == ENTRY)
    {
        if(symbol[0] != '\0')
        {
            fprint_error("Unexpected label at .entry instruction", line_number);
            return 0;
        }
    }

    return 0;   // not succeed
}