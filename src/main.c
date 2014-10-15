#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include "setsig.h"
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
                if (ferror(stdin)){
                        fprintf(stderr, "error reading input\n");
                } else {
                        /* ctrl-d pressed */
                        /* TODO: exit the shell */
                        buffer[0] = '\0';
			bn_exit((char *[]){"exit", NULL});
                }
        } else{
                buffer[strlen(buffer) - 1] ='\0';
        }
}

void pr_prompt()
{
        /* TODO: print out the prompt (See Specification P.4) */
	char *dirname = get_current_dir_name();
	printf("[3150 shell:%s]$ ", dirname);
	free(dirname);
}

void wait_foreground()
{
        int my_pgid = getpgrp();
        while (tcgetpgrp (0) != my_pgid)
                kill (- my_pgid, SIGTTIN);
}

void set_ownpgrp()
{
        int my_pgid = getpgrp();
        if (setpgid(0, 0) == -1) {
                perror("setpgid");
                exit(1);
        }

        tcsetpgrp(0, my_pgid);
}

void initialize()
{
        wait_foreground();
        setsig();
        set_ownpgrp();
        interpreter_init();
        /*
         * TODO:
         * initialize env PATH (See specification P.9)
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
