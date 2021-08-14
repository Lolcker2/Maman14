//#include "first_pass.c"
#include <stdio.h>
#include "defs.c"

int check(int params,...)
{
    int i =0;
    printf("%d \n", params);
    return 0;
}


int main()
{
    // ----------------------- bin ----------------------------
    check(5,6);
    return 0;
}
