#include <stdio.h>
#include <string.h>
#include "interpreter.h"

char *classify(char *given_cmdname)
{
        static struct cmdmapping bn_cmdmap[] =
        {
                {"cd", NULL},
                {"exit", NULL},
                {"fg", NULL},
                {"jobs", NULL},
                {NULL, NULL}
        };

        /*
         *TODO: check actual requirement of the string, I am lazy
         */
        struct cmdmapping *map_entry = bn_cmdmap;
        while (map_entry->cmdname){
                if (!strcmp(map_entry->cmdname, given_cmdname))
                        return "builtin";
                map_entry++;
        } 

        return "external";
}

void interpreter(struct parsetree cmd_info)
{
        char **token = cmd_info.cmd;
        
        if (token[0] != NULL){
                char *cmd_type = classify(token[0]);
                printf("Token %d: %s (%s)\n", 1, token[0], cmd_type);
        }
        for (int i = 1; token[i] != NULL; i++)
                printf("Token %d: %s\n", i + 1, token[i]);
}
