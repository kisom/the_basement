/************************************************************************
 * srm.c                                                                *
 * author: kyle isom <coder@kyleisom.net>                               *
 * license: isc / public domain dual-licensed                           *
 * date: 2011-08-03                                                     *
 *                                                                      *
 * securely remove a file                                               *
 ************************************************************************/
#define _BSD_SOURCE
#undef _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <limits.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/stat.h>

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
            fprintf(stderr, "\n");    
        } else {
            wipe_success[wiped] = strdup(*file_ptr);
            wiped++;

            printf("OK!\n");
        }
        file_ptr++;
    }

    /* print list of files wiped / failed to wipe if verbose */
    if (1 == verbose) {
        if (0 < wiped) {
            printf("successfully wiped: ");
            for( i = 0; i < wiped; ++i ) {
                printf("%s ", wipe_success[i]);
            }
            printf("\n");
        }

        if (0 < failed) {
            printf("failed to wipe: ");
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
        fprintf(stderr, "failed to open file");
        return retval;
    } else {
        filesize = sb.st_size;
    }

    if (0 == filesize) { }

    for( i = 0; i < passes; ++i ) {
        if (EXIT_FAILURE == wipe(filename)) {
            printf("!");
            return retval;
        } else if ((-1 == stat(filename, &sb)) && (filesize != sb.st_size)) {
            printf("!");
            return retval;
        }
        printf(".");
        fflush(stdout);
    }
    printf(" %0.0fk ", (double) filesize / 1024.0);

    if (-1 == unlink(filename)) {
        fprintf(stderr, "unlink error!");
    } else {
        retval = EXIT_SUCCESS;
    }

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
    char *random_data;
    int retval;
    int targetfd;

    retval = EXIT_FAILURE;
    filesize = stat(filename, &sb);

    if (-1 == stat(filename, &sb)) {
        fprintf(stderr, "failed to open file");
        return retval;
    } else {
        filesize = sb.st_size;
    }

    wiped = 0;

    devrandom = fopen(DEV_RANDOM, "r");
    if (NULL == devrandom) {
        fprintf(stderr, "could not open %s!", DEV_RANDOM);
        return retval;
    } else if (0 != ferror(devrandom)) {
        fprintf(stderr, "error opening %s!", DEV_RANDOM);
        perror("fopen");
        return retval;
    }

    /* open target second that way file is not touched if we can't open
     * the DEV_RANDOM file */
    target = fopen(filename, "w");
    if (NULL == target) {
        printf("%s -> %d\n", filename, fileno(target));
        fprintf(stderr, "file does not exist!");
        fclose(devrandom);
        return retval;
    } else if (0 != ferror(target)) {
        fprintf(stderr, "error opening file!");
        fclose(devrandom);
        return retval;
    } else {
        /* need to exclusively lock the target file to make sure no one
         * else is touching it. otherwise a secure wipe can't be 
         * guaranteed! (ex. file is open in a text editor: user just saves
         * and all the work spent wiping the file is for naught...)
         */
        targetfd = fileno(target);
        if ( -1 == flock(targetfd, LOCK_EX) ) {
            fprintf(stderr, "failed to obtain a lock!");
            fclose(target);
            fclose(devrandom);
            close(targetfd);
            return retval;
        }
    }


    /* avoid allocating until we absolutely have to */
    /* makes returning from an error less tricky */
    random_data = calloc( MAX_CHUNK, sizeof(char) );
    while( wiped < filesize ) {
        chunk = filesize - wiped;
        chunk = chunk > MAX_CHUNK ? MAX_CHUNK : chunk;
        
        rdsz = fread(random_data, sizeof(char), chunk, devrandom);
        wrsz = fwrite(random_data,sizeof(char), chunk, target);

        if ( (rdsz != wrsz) || (wrsz != chunk) ) {
            fprintf(stderr, "failed to write expected number of bytes!");
            printf("chunk: %u\trdsz: %u\twrsz: %u\n",
                    (unsigned int) chunk, (unsigned int) rdsz,
                    (unsigned int) wrsz);
            break;  /* break now for cleanup */
        }

        wiped += chunk;

    }

    flock(targetfd, LOCK_UN);
    fclose(devrandom);
    fclose(target);
    close(targetfd);
    if (wiped == filesize) {
        retval = EXIT_SUCCESS;
    }

    free(random_data);
    random_data = NULL;

    return retval;
}

