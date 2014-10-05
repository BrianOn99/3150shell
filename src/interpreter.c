#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "interpreter.h"

/*
 * Here are functions making up the interprter, which actually run commands
 * the functions starting with "bn_" represent builtin commands
*/

enum cmd_type { builtin, external };

struct iofd {
        int in;
        int out;
};

int cmdlen(char* cmdargv[])
{
	char **arg_ptr = cmdargv;
	while (*(++arg_ptr));
	return arg_ptr - cmdargv;
}

int bn_cd(char* cmdargv[])
{
        int cmdargc = cmdlen(cmdargv);
        if (cmdargc != 2){
                /*
                 * Error SHOULD be printed to stderr, but P.21 require stdout
                 * I am Very reluctant to do so, it is ugly
                 */
                fprintf(stdout, "cd: wrong number of arguments\n");
                return 1;
        }
        chdir(cmdargv[1]);

        return 0;
}

int bn_fg(char* cmdargv[])
{
        return 0;
}

int bn_jobs(char* cmdargv[])
{
        return 0;
}
 
int bn_exit(char* cmdargv[])
{
        int cmdargc = cmdlen(cmdargv);
        if (cmdargc != 1){
                fprintf(stdout, "exit: wrong number of arguments\n");
                return 1;
        }

        printf("[ Shell Terminated ]\n");
        exit(0);
}

int run_builtin(char **cmd)
{
        cmd_evaluater eval = NULL;
        classify(cmd[0], &eval);
        return eval(cmd);
}

int run_external(char* cmdargv[], struct iofd inoutfd)
{
        int pid = fork();
        if (pid == -1) {
                perror("fork");
                return -1;
        } else if (pid == 0) {
                execvp(cmdargv[0], cmdargv);
                perror("exec");
                exit(-1);
        } else {
                int status;
                if (waitpid(pid, &status, 0) == -1)
                        return -1;
                else if (WIFEXITED(status))
                        return WEXITSTATUS(status);
        }
        return -1;
}

static struct cmdmapping bn_cmdmap[] =
{
        /* TODO: store the function instead of NULL */
        {"cd", bn_cd},
        {"exit", bn_exit},
        {"fg", bn_fg},
        {"jobs", bn_jobs},
        {NULL, NULL}
};

/* 
 * classify the command: builtin or external program
 * A function pointer will be stored in backeval, which implement the action
 */
enum cmd_type classify(char *given_cmdname, cmd_evaluater *backeval)
{

        struct cmdmapping *map_entry = bn_cmdmap;
        while (map_entry->cmdname){
                if (!strcmp(map_entry->cmdname, given_cmdname)){
                        if (backeval)
                                *backeval = map_entry->cmdfunction;
                        return builtin;
                }
                map_entry++;
        } 

        return external;
}


void print_and_run(char **cmd, int *argpos)
{
        static char *typestr[] = {"Built-in Command", "Command Name"};
	cmd_evaluater eval = NULL;

        char *type = typestr[classify(cmd[0], &eval)];

	if (cmd[0] != NULL)
		printf("Token %d: \"%s\" (%s)\n", (*argpos)++, cmd[0], type);

	for (int i = 1; cmd[i] != NULL; i++)
		printf("Token %d: \"%s\" (%s)\n", (*argpos)++, cmd[i], "Argument");

	if (eval != NULL){
		int ret = eval(cmd);
                if (ret)
                        return;
        }
}

/* master function of this library */
int interpreter(struct parsetree *cmd_info)
{
	char ***list = cmd_info->list;
        struct iofd *inoutfds = malloc(sizeof(struct iofd) * cmd_info->count);

        inoutfds[0].in = 0;
        inoutfds[cmd_info->count-1].out = 1;

        for (int i=0; i+1 < cmd_info->count; i++){
                int fildes[2];

                if (pipe(fildes) == -1){
                        perror("Creating pipe");
                        return -1;
                }

                inoutfds[i].out = fildes[1];
                inoutfds[i+1].in = fildes[0];
        }

        if (cmd_info->count > 1){
                for (int i=0; i < cmd_info->count; i++){
                        if (classify(list[i][0], NULL) == builtin){
                                printf("Error: invalid input command line\n");
                                return -1;
                        }
                }

                for (int i=0; i < cmd_info->count; i++)
                        run_external(list[i], inoutfds[i]);
                
        } else {
                cmd_evaluater eval = NULL;
                if (classify(list[0][0], &eval) == builtin)
                        return eval(list[0]);
                else
                        run_external(list[0], inoutfds[0]);
        }

        /* TODO: close fd */
}
