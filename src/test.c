#include <stdio.h>
#include "parser.h"

void main(void)
{
        char string[] = "ab cd eee";
        struct parsetree cmd_info;
        parser(string, cmd_info);
}
