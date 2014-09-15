#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "parser.h"
#include "interpreter.h"

#define CMD_BUF_SIZE (MAX_CMD_LEN * sizeof(char))

void getcmdline(char *buffer, size_t size)
{
        int read_size = read(0, buffer, size-1);
        if (read_size == -1)
                perror("getcmdline");
        buffer[read_size] = '\0';
}

void initialize()
{
        /*
         * TODO:
         * initialize env PATH
         */
}

void mainloop()
{
         static char cmdline[CMD_BUF_SIZE] = "ab cd eee";  // this store command line
         static struct parsetree cmd_info;

         while (true){
                 getcmdline(cmdline, CMD_BUF_SIZE);
                 parser(cmdline, &cmd_info);
                 interpreter(cmd_info);
         }
}

void main(void)
{
        initialize();
        mainloop();
}
