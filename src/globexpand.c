#include <glob.h>
#include <stdlib.h>
#include <stdio.h>
#include "globexpand.h"

/* 
 * expand {"你都". "戇*"} to {"你都", "戇鳩", "戇撚鳩", "戇撚柒鳩鳩"}
 * the result result is stored in pglob_res->gl_pathv
 */

int glob_expand(char * const *words, glob_t *pglob_res)
{
        char *word;
        int flags = GLOB_MARK | GLOB_NOMAGIC | GLOB_NOCHECK;

        while (word = *words++) {
                int ret = glob(word, flags, NULL, pglob_res);

                if (ret == GLOB_NOSPACE) {
                        fprintf(stderr, "No memory for globbing\n");
                        return -1;
                }

                /* want this flag starting from 2nd time, in order to append the expanded
                 * tokens to the structure glob_t
                 */
                flags |= GLOB_APPEND;  
        }

        return 0;
}
