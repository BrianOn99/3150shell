#include <stdio.h>
#include <string.h>
#include "interpreter.h"

/*
 * Here are functions making up the interprter, which actually run commands
 * the functions starting with "bn_" represent builtin commands
 */

int bn_cd(int cmdargc, char* cmdargv[])
{
        if (cmdargc != 1){
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

int bn_exit(int cmdargc, char* cmdargv[])
{
        /* TODO: exit (P.20) */
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

/* master function of this library */
void interpreter(struct parsetree cmd_info)
{
        char **token = cmd_info.cmd;
        
        cmd_evaluater eval = NULL;
        if (token[0] != NULL){
                char *cmd_type = classify(token[0], &eval);
                printf("Token %d: %s (%s)\n", 1, token[0], cmd_type);
        }

        for (int i = 1; token[i] != NULL; i++)
                printf("Token %d: %s (%s)\n", i + 1, token[i], "Argument");

        if (eval != NULL)
                eval(cmd_info.cmd_len, cmd_info.cmd);
}
