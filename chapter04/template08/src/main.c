#include <stdio.h>
#include "int_sorter.h"

int main()
{
    FILE *test = fopen("test","w+");
    IntSorterError errorcode = int_sorter(test->_tmpfname);
    printf("file create = %d \n", errorcode);
    return errorcode;
}
