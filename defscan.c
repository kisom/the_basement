/**************************************************************************
 * defscan.c                                                              *
 * 4096R/B7B720D6  "Kyle Isom <coder@kyleisom.net>"                       *
 * 2011-01-14                                                             *
 *                                                                        *
 * scan header files for unused defines                                   *
 *                                                                        *
 * looks through either a single header file or every header file in the  *
 * directory, looking for #define statements and checking to see if those *
 * are used at all in the source. the idea is not to check to see which   *
 * code maybe included, for example, but to identify obsolete #defines.   *
 *                                                                        *
 **************************************************************************/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defscan.h"


static char **sources = NULL;           /* NULL-terminated list of header files
                                         * containing the defines to check   */
static char **targets = NULL;           /* NULL-terminated list of source files
                                         * to scan for occurences of defines */
static defset_t *dset = NULL;           /* NULL-terminated list of defsets,
                                         * which contain macros and their use
                                         * counts.                            */

int main(int argc, char **argv) {
    sources = calloc(MAX_SOURCES, sizeof sources);
    targets = calloc(MAX_TARGETS, sizeof targets);
    targets = calloc(MAX_DEFINES, sizeof *dset);

    /* need to build the sources list                                       */

    /* scan sources for macros                                              */

    /* build target list                                                    */

    /* scan targets for occurrences of macros                               */
    /* note that sources may also be targets, but will need to skip         *
     * #defines!                                                            */

    free(sources);
    free(targets);
    free(dset);
    return EXIT_SUCCESS;
}

