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
#include <sys/types.h>

#include <string.h>

#define DEFAULT_PASSES  3
#define DEV_RANDOM      "/dev/urandom"
#define MAX_CHUNK       4096


int do_wipe(char *, size_t);
int wipe(char *);

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

    while( -1 != (opt = getopt(argc, argv, "hn:v"))) {
        switch( opt ) {
            case 'h':
                usage(argv[0]);
                return retval;
                break;              /* don't technically need it but meh */
            case 'n':
                tmp_passes = atoi(optarg);
                passes = tmp_passes > 0 ? tmp_passes : passes;
                break;
            case 'v':
                verbose = 1;
                break;
            default:
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
        } else {
            wipe_success[wiped] = strdup(*file_ptr);
            wiped++;
            printf(" OK!");
        }
        file_ptr++;
        printf("\n");
    }

    if (1 == verbose) {
        if (0 < wiped) {
            printf("success: ");
            for( i = 0; i < wiped; ++i ) {
                printf("%s ", wipe_success[i]);
            }
            printf("\n");
        }

        if (0 < failed) {
            printf("failures: ");
            for( i = 0; i < failed; ++i ) {
                printf("%s ", wipe_fail[i]);
            }
            printf("\n");
        }
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
    struct stat sb;
    size_t filesize, i;
    int retval;

    retval = EXIT_FAILURE;
    if (-1 == stat(filename, &sb)) {
        fprintf(stderr, "error opening file!");
        return retval;
    }
    filesize = sb.st_size;

    for( i = 0; i < passes; ++i ) {
        if (EXIT_FAILURE == wipe(filename)) {
            printf("!");
            return retval;
        } else if (-1 == stat(filename, &sb)) {
            printf("!");
            return retval;
        } else if (filesize != sb.st_size) {
            printf("!");
            return retval;
        }
        printf(".");
        fflush(stdout);
    }
/*
    if (-1 == unlink(filename)) {
        fprintf(stderr, "unlink error!");
    } else {
        retval = EXIT_SUCCESS;
    }
*/
    return retval;
}

int
wipe(char *filename)
{
    struct stat sb;

    size_t chunk;
    size_t filesize;
    size_t rdsz;
    size_t wiped;
    size_t wrsz;

    FILE *devrandom;
    FILE *target;

    int retval;
    int targetfd;

    char *rdata;

    retval = EXIT_FAILURE;
    wiped  = 0;

    if (-1 == stat(filename, &sb)) {
        return retval;
    }

    filesize = sb.st_size;

    /* 
     * open devrandom first: if this fails, we don't want to touch the target
     * file. 
     */
    devrandom = fopen(DEV_RANDOM, "r");
    if (NULL == devrandom) {
        printf("failed to open PRNG %s!", DEV_RANDOM);
        return retval;
    } else if (-1 == ferror(devrandom)) {
        printf("error opening %s!", DEV_RANDOM);
        return retval;
    }

    target   = fopen(DEV_RANDOM, "w");
    if (NULL == target) {
        fprintf(stderr, "failed to open file");
        fclose(devrandom);
        return retval;
    } else if (-1 == ferror(target)) {
        fprintf(stderr, "error opening file!");
        fclose(devrandom);
        return retval;
    }

    targetfd = fileno(target);

    /* wait to calloc until we really need the data - makes cleaning up less
     * tricky.
     */
    rdata = calloc(MAX_CHUNK, sizeof(char));
    while( wiped < filesize ) {
        chunk = filesize - wiped;
        chunk = chunk > MAX_CHUNK ? MAX_CHUNK : chunk;

        rdsz  =  fread( rdata, sizeof(char), chunk, devrandom );
        wrsz  = fwrite( rdata, sizeof(char), chunk, target );

        if ( -1 == stat(filename, &sb)) {
            fprintf(stderr, " stat() error !");
            break;
        }
        if ( (rdsz != wrsz) || (filesize != sb.st_size) ) {
            fprintf(stderr, "invalid read/write size");
            break;
        }

        wiped += chunk;
    }
    
    
    fclose(devrandom);
    fclose(target);
    free(rdata);
    rdata = NULL;

    return retval;
}

