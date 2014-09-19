#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "parser.h"
#include "interpreter.h"

#define CMD_BUF_SIZE (MAX_CMD_LEN * sizeof(char))

/*
 * read a line fron stdin and store it in buffer, without terminating
 * newline character
 */
void getcmdline(char *buffer, size_t size)
{

        char *ret = fgets(buffer, size, stdin);
        if (!ret){
                if (ferror(stdin))
                        fprintf(stderr, "error reading input\n");
                else
                        /* ctrl-d pressed */
                        /* TODO: exit the shell */
                        buffer[0] = '\0';
			bn_exit();
        } else{
                buffer[strlen(buffer) - 1] ='\0';
        }


        /* this is old version using bare system call, but error prone */
        /*
        int read_size = read(0, buffer, size-1);
        if (read_size == -1)
                perror("getcmdline");
        else if (read_size == 0)
                buffer[0] = '\0';
        else
                buffer[read_size - 1] = '\0';
        */
}

void pr_prompt()
{
        /* TODO: print out the prompt (See Specification P.4) */
}
void initialize()
{
        /*
         * TODO: (some in phase 2)
         * initialize env PATH (See specification P.9)
         * ignore some signals (sigint, etc)
         * change behavior of sigchld
         */
}

void mainloop()
{
         static char cmdline[CMD_BUF_SIZE];  // this store command line
         static struct parsetree cmd_info;

         while (true){
                 pr_prompt();
                 getcmdline(cmdline, CMD_BUF_SIZE);
                 if (parser(cmdline, &cmd_info) != -1)
			 interpreter(&cmd_info);
         }
}

void main(void)
{
        initialize();
        mainloop();
}
