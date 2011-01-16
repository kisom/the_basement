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
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <pcre.h>

#include "defscan.h"


static char **sources = NULL;           /* NULL-terminated list of header files
                                         * containing the defines to check    */
static size_t sources_sz = 0;           /* size_t containing number of header 
                                         * files in source list               */
static char **targets = NULL;           /* NULL-terminated list of source files
                                         * to scan for occurences of defines  */
/*static size_t targets_sz = 0;*/
static defset_t *dset = NULL;           /* NULL-terminated list of defsets,
                                         * which contain macros and their use
                                         * counts.                            */

int main(int argc, char **argv) {
    sources = calloc(MAX_SOURCES, sizeof sources);
    targets = calloc(MAX_TARGETS, sizeof targets);
    dset    = calloc(MAX_DEFINES, sizeof *dset);

    size_t *sz_p = NULL;
    size_t i     = 0;                                         /* loop index */


    /* need to build the sources list                                       */
    printf("building source list...\n");
    sz_p = &sources_sz;
    dirwalk("./", sources, sz_p, FILE_CHDR);
    printf("sources: \n");
    while (i < sources_sz) {
        printf("[+] header: %s\n", sources[i++]);
    }

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

filetype_t header_p(char *pathname) {
    filetype_t ftype = FILE_OTHER;
    size_t psz  = strlen(pathname);             /* path size */
    psz = strlen(pathname);

    /* super effective extension-based file scanning! */
    if (0x2e == pathname[psz - 2]) {
        if (0x68 == pathname[psz - 1]) {
            ftype = FILE_CHDR;
        } else if (0x63 == pathname[psz - 1]) {
            ftype = FILE_CSRC;
        }
    }

    return ftype;
}


void dirwalk(const char *dirpath, char **list, size_t *sz, filetype_t loadt) {
    DIR *dirp               = NULL;         /* pointer to dir stream handle */
    struct dirent *direntp  = NULL;         /* pointer to next dir entry    */
    filetype_t ft           = FILE_OTHER;   /* file type of current file    */

    dirp = opendir(dirpath);
    if (NULL == dirp) {
        return;
    }

    direntp = readdir(dirp);                /* attempt to read next dirent  */

    while (NULL != direntp) {
        /* if the file is a header file add it *
         * the list of source files. We always *
         * load header files...                */
        ft = header_p(direntp->d_name);
        if (((FILE_CHDR == ft) && (loadt & FILE_CHDR)) ||
            ((FILE_CSRC == ft) && (loadt & FILE_CSRC))) {
            /* if it's a header, copy it in to the list */
            list[*sz] = calloc(strlen(direntp->d_name), sizeof *list);
            strncpy(list[*sz], direntp->d_name, strlen(direntp->d_name));
            sources_sz++;
        }

        direntp = readdir(dirp); 
    }

    if (0 == errno) {
        if (0 != closedir(dirp)) {
            fprintf(stderr, "error closing %s!\n", dirpath);
        } /* not much else we can do at this point */

        dirp    = NULL;
        direntp = NULL;

        return;
    } else {
        /* fuck it */
        dirp    = NULL;
        direntp = NULL;
        return;
    }

    return;
}

int scanload_macros(const char *header) {
    int result      = EXIT_SUCCESS; 
    FILE *headerp   = NULL;                             /* file stream ptr   */
    char line_buf[LINEBUF_SZ] = "";                     /* file read buffer  */

    /* set up prce - it's ugly and we have to do a lot of work */
    pcre *regex     = NULL;                             /* compiled regex    */
    char macro_re[] = "^[#]define\\s+(\\w+)\\s+.+$";    /* regex to detect   *
                                                         * macros            */
    const char **re_error  = NULL;                      /* pointer to regex  *
                                                         * error message     */
    int err_off     = 0;                                /* error offset      */
    int capvec[CAPTUREVECTORSIZE];                      /* pcre capture vec  */ 
    
    /* compile pcre with no options and default locale */
    regex = pcre_compile(macro_re, 0, re_error, &err_off, NULL);
    if (NULL == regex) {
        result = EXIT_FAILURE;
    } else {

        /* now we can actually begin to code! */

        headerp = fopen(header, "r");
        if (NULL == headerp) {
            result = EXIT_FAILURE;
        } else {
            while (NULL != fgets(line_buf, LINEBUF_SZ - 1, headerp)) {
            }
        }
    }

    return result;

}

