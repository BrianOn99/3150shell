#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

/*
 * Here are functions making up the interprter, which actually run commands
 * the functions starting with "bn_" represent builtin commands
 */

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
        /* TODO: really do a cd */

        return 0;
}

int bn_exit(char* cmdargv[])
{
        exit(0);
}

/* 
 * classify the command: builtin or external program
 * A function pointer will be stored in backeval, which implement the action
 */
char *classify(char *given_cmdname, cmd_evaluater *backeval)
{
        static struct cmdmapping bn_cmdmap[] =
        {
                /* TODO: store the function instead of NULL */
                {"cd", bn_cd},
                {"exit", bn_exit},
                {"fg", NULL},
                {"jobs", NULL},
                {NULL, NULL}
        };

        /*
         *TODO: check actual requirement of the string, I am lazy
         */
        struct cmdmapping *map_entry = bn_cmdmap;
        while (map_entry->cmdname){
                if (!strcmp(map_entry->cmdname, given_cmdname)){
                        *backeval = map_entry->cmdfunction;
                        return "Built-in Command";
                }
                map_entry++;
        } 

        return "Command Name";
}


void print_and_run(char **cmd, int *argpos)
{
	cmd_evaluater eval = NULL;
	if (cmd[0] != NULL){
		char *cmd_type = classify(cmd[0], &eval);
		printf("Token %d: %s (%s)\n", (*argpos)++, cmd[0], cmd_type);
	}

	for (int i = 1; cmd[i] != NULL; i++)
		printf("Token %d: %s (%s)\n", (*argpos)++, cmd[i], "Argument");

	if (eval != NULL)
		eval(cmd);
}

/* master function of this library */
void interpreter(struct parsetree *cmd_info)
{
	char ***list = cmd_info->list;
	int argpos = 1;

	print_and_run(list[0], &argpos);
	for (int i=1; i < cmd_info->count; i++){
		printf("Token %d: pipe\n", argpos++);
                print_and_run(list[i], &argpos);
	}
}
