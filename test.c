#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdarg.h>
//#include "defs.c"


// malloc_with_check
// use opcodes and functs
// use typedefs for dicts
// diff instructions, functions

// reverses the given string and returns it
char* reverse_str(char* str)
{
    char result[strlen(str)];
    unsigned int i = strlen(str) -1;
    int j = 0;
    for(; j < strlen(str) && i >= 0; i--, j++) // 2 pointers: i and j
        if(str[i] != '\0') {    // so '\0' wont be first
            result[j] = str[i];
            //printf("nnn; %d", result[j]);
        }
    result[j] = '\0'; // closing the string
   // printf("ress; %s",result);
    char* returned = (char*)result; // casting to char*
    //printf("ret; %s", returned);
    return returned;
}


// converts num into a binary number with size digits.
// params: num , size: the number that be converted, number of digits of the binary number
char* int_to_bin(int num, int size)
{
    if(num < 0)		// if num is negative
        {
        num *= -1;
        int power_of_two = 2;
        for(int i =1; i< size; i++)	// clac 2 to the power of size
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
            result[i] = (char)49;   // '1'
        }else {result[i] = (char)48;}   // '0'

        num = num / 2;
        i++;
        }

    for(; i < size; i++)   // adding 0's to fit the size
        {
        result[i] = (char)48;
        } result[i] = '\0'; // closing the string

        printf("res: %s \n", result);
        char* returned = reverse_str(result);
        printf("hhh %s\n", returned);
    return returned; // reversing it
}

char* int_to_small_bin(int num, int max)
{

    if(num < 0)		// if num is negative
        {
        num *= -1;
        char* bin = int_to_small_bin(num, max);
        int len = strlen(bin) + 1;
        num *= -1;
        return int_to_bin(num, len);
        }
    char result[max+1];    // size + 1 meaning '\0' at last index

    int i = 0;
    while(num > 0 && i < max)  // converting to binary
        {
        if(num % 2 == 1)
        {
            result[i] = (char)49;   // '1'
        }else {result[i] = (char)48;}   // '0'

        num = num / 2;
        i++;
        }
        result[i] = '\0'; // closing the string

        char* returned = reverse_str(result);
        return returned;
}


int main()
{
    //printf("aaa %s \n", int_to_bin(-7,16));
  printf("bbb %s \n", int_to_small_bin(-7, 16));
   // printf("num: 4, result: %s \n", int_to_bin(-4, 16));
    //printf("num: 5, result: %s \n", int_to_bin(-5, 4));
    //printf("num: 7, result: %s \n", int_to_bin(-7, 4));
    return 0;
}
