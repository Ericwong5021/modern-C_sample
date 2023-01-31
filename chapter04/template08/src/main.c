#include <stdio.h>
#include "int_sorter.h"

int main()
{
    // FILE *test = fopen("test","r");
    char* fname = "D:\\test.txt";
    IntSorterError errorcode = int_sorter(fname);
    printf("file create = %d \n", errorcode);
    return errorcode;
}
