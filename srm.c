/************************************************************************
 * srm.c                                                                *
 * author: kyle isom <coder@kyleisom.net>                               *
 * license: isc / public domain dual-licensed                           *
 * date: 2011-08-03                                                     *
 *                                                                      *
 * securely remove a file                                               *
 * powered by radiohead - ok computer                                   *
 *                                                                      *
 * i was bored and needed a secure file removal tool                    *
 * ergo, i rolled my own                                                *
 *                                                                      *
 * TODO: add recursive removal for securely removing a directory        *
 ************************************************************************/
/*
 * Copyright (c) 2011 KYLE ISOM <coder@kyleisom.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define _BSD_SOURCE

#include <sys/cdefs.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#define DEFAULT_PASSES  3
#define DEV_RANDOM      "/dev/urandom"
#define MAX_CHUNK       4096

/* prototypes */
static int         do_wipe(char *, size_t);
static int         wipe(char *);
#ifdef OPENBSD
__dead void usage(void);
#else
static void        usage(void);
#endif

/*
 * secure rm: overwrite a file with one or more passes of random data,
 * then unlink it.
 */
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

    if (argc == 1)
        usage();

    while (-1 != (opt = getopt(argc, argv, "hn:v"))) {
        switch( opt ) {
        case 'h':
            usage();
            break;              /* don't technically need it but meh */
        case 'n':
            tmp_passes = atoi(optarg);
            passes = tmp_passes > 0 ? tmp_passes : passes;
            break;
        case 'v':
            verbose = 1;
            break;
        default:
            usage();
            /* NOTREACHED */
        }
    }

    argc -= optind;
    argv += optind;

    file_ptr = argv;
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
    for (i = 0; i < failed; ++i) {
        free(wipe_fail[i]);
        wipe_fail[i] = NULL;
    }
    free(wipe_fail);
    wipe_fail = NULL;

    for (i = 0; i < wiped; ++i) {
        free(wipe_success[i]);
        wipe_success[i] = NULL;
    }
    free(wipe_success);
    wipe_success = NULL;

    return retval;
}

/*
 * int do_wipe(char *, size_t)
 *  takes a filename and the number of passes to wipe the file
 *  returns EXIT_SUCCESS || EXIT_FAILURE
 */
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

    for (i = 0; i < passes; ++i) {
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

    if (-1 == unlink(filename)) {
        fprintf(stderr, "unlink error!");
    } else {
        retval = EXIT_SUCCESS;
    }

    return retval;
}

/*
 * int wipe(char *)
 *  takes a filename and attempts to overwrite it with random data
 *  returns EXIT_SUCCESS || EXIT_FAILURE
 */
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

    if (-1 == stat(filename, &sb))
        return retval;

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

    target   = fopen(filename, "r+");
    if (NULL == target) {
        fprintf(stderr, "failed to open file");
        fclose(devrandom);
        return retval;
    } else if (-1 == ferror(target)) {
        fprintf(stderr, "error opening file!");
        fclose(devrandom);
        return retval;
    }
    rewind(target);
    targetfd = fileno(target);
    if (-1 == flock(targetfd, LOCK_EX)) {
        fprintf(stderr, "error locking file");
        fclose(devrandom);
        fclose(target);
    }

    /* wait to calloc until we really need the data - makes cleaning up less
     * tricky.
     */
    rdata = calloc(MAX_CHUNK, sizeof(char));
    if (NULL == rdata) {
        fprintf(stderr, "could not allocate random data memory");
        return retval;
    }

    while (wiped < filesize) {
        chunk = filesize - wiped;
        chunk = chunk > MAX_CHUNK ? MAX_CHUNK : chunk;

        rdsz  =  fread( rdata, sizeof(char), chunk, devrandom );
        wrsz  = fwrite( rdata, sizeof(char), chunk, target );

        if (-1 == stat(filename, &sb)) {
            fprintf(stderr, " stat() error !");
            break;
        }
        if ((rdsz != wrsz) || (filesize != (unsigned int)sb.st_size)) {
            fprintf(stderr, "invalid read/write size");
            break;
        }

        wiped += chunk;
    }
    
    /* release lock */
    if (-1 == flock(targetfd, LOCK_UN))
        fprintf(stderr, "error releasing lock");

    /* for cleanup, report close errors and free mem */
    if ((0 != fclose(devrandom)) || (0 != fclose(target)))
        fprintf(stderr, "error closing file");
    else
        retval = EXIT_SUCCESS;

    free(rdata);
    rdata = NULL;

    return retval;
}

/*
 * usage: print a quick usage message 
 */
void 
usage(void)
{
    extern char *__progname;

    printf("usage: %s [-v] [-n number] file list\n", __progname);
    printf("\noptions:\n");
    printf("\t-v: verbose mode. display list of failures and wiped files ");
    printf("after wiping\n");
    printf("\t-n <number of passes>: specify number of passes\n");
    printf("\t\t(default is %d passes)\n", DEFAULT_PASSES);

    printf("\n");
    exit(EXIT_FAILURE);
}


