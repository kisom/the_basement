/************************************************************************
 * srm.c                                                                *
 * author: kyle isom <coder@kyleisom.net>                               *
 * license: isc / public domain dual-licensed                           *
 * date: 2011-08-03                                                     *
 *                                                                      *
 * securely remove a file                                               *
 ************************************************************************/
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <limits.h>
#include <unistd.h>

#include <sys/stat.h>

#include <string.h>

#define DEFAULT_PASSES  3
#define DEV_RANDOM      "/dev/urandom"
#define MAX_CHUNK       4096


int do_wipe(char *, size_t);
int wipe(char *);
size_t get_size(char *);

void 
usage(char *argv0)
{
    printf("usage: %s [options] file list\n", basename(argv0));
    printf("\noptions:\n");
    printf("\t-n <number of passes>: specify number of passes\n");
    printf("\t\t(default is %d passes)\n", DEFAULT_PASSES);
    printf("\t-v: verbose mode. display list of failures and wiped files ");
    printf("after wiping\n");

    printf("\n");
    return;
}

int 
main( int argc, char **argv )
{
    size_t passes, tmp_passes, wiped, failed, i;
    char **wipe_success, **wipe_fail;
    char **file_ptr;
    int retval, opt, verbose;

    passes  = DEFAULT_PASSES;
    retval  = EXIT_FAILURE;
    wiped   = 0;
    failed  = 0;
    verbose = 0;

    if (argc == 1) {
        usage(argv[0]);
        return retval;
    }

    while( -1 != (opt = getopt(argc, argv, "n:v"))) {
        switch( opt ) {
            case 'n':
                tmp_passes = atoi(optarg);
                passes = tmp_passes > 0 ? tmp_passes : passes;
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                fprintf(stderr, "invalid option!\n");
                return retval;
        }
    }

    file_ptr = &argv[optind];
    wipe_success = calloc(argc, sizeof(char *));
    wipe_fail    = calloc(argc, sizeof(char *));
    

    while (NULL != *file_ptr) {
        printf("%s: ", *file_ptr);
        fflush(stdout);

        if (EXIT_FAILURE == do_wipe(*file_ptr, passes)) {
            wipe_fail[failed] = strdup(*file_ptr);
            failed++;
            
            fprintf(stderr, "\n\t[!] failed to wipe %s\n", *file_ptr);
        } else {
            wipe_success[wiped] = strdup(*file_ptr);
            wiped++;

            printf("OK!\n");
        }
        file_ptr++;
    }

    /* free allocated memory */
    for( i = 0; i < failed; ++i ) {
        free(wipe_fail[i]);
        wipe_fail[i] = NULL;
    }
    free(wipe_fail);
    wipe_fail = NULL;

    for( i = 0; i < wiped; ++i ) {
        free(wipe_success[i]);
        wipe_success[i] = NULL;
    }
    free(wipe_success);
    wipe_success = NULL;

    return retval;
}

int
do_wipe(char *filename, size_t passes)
{
    size_t filesize, i;
    int retval;

    filesize = get_size(filename);
    retval = EXIT_FAILURE;

    for( i = 0; i < passes; ++i ) {
        printf(".");
        fflush(stdout);
        if (EXIT_FAILURE == wipe(filename)) {
            printf("!");
            return retval;
        } else if (filesize != get_size(filename)) {
            printf("!");
            return retval;
        }
    }

    if (-1 == unlink(filename)) {
        fprintf(stderr, "unlink error!");
    } else {
        retval = EXIT_SUCCESS;
    }

    return retval;
}

size_t
get_size(char *filename)
{
    struct stat sb;

    if ( -1 == stat(filename, &sb)) {
        return 0;
    } else {
        return sb.st_size;
    }
}

int
wipe(char *filename)
{
    return 0;
}

