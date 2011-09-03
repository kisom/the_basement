/************************************************************************
 * srm.c                                                                *
 * author: kyle isom <coder@kyleisom.net>                               *
 * license: isc / public domain dual-licensed                           *
 * date: 2011-08-03                                                     *
 *                                                                      *
 * securely remove a file                                               *
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

#include <sys/stat.h>

#include <string.h>

#define DEV_RANDOM      "/dev/urandom"

size_t get_filesize(char *);
size_t wipe(char *, size_t);

int
main( int argc, char **argv)
{
    char *argp;
    char *filename;             /* the file target to be wiped */
    size_t filesize;            /* the size of the file to be wiped */
    size_t i;                   /* loop counter */
    int passes;                 /* number of passes for wipe */
    
    passes = 1;                 /* default is one pass for a wipe */

    /* if an argument is passed, assume it is the number of passes to
     * wipe the file. test to make sure a valid number was passed in.
     */
    if (3 == argc) {
        int tmp_pass;           /* temporary pass variable */
        tmp_pass = atoi(argv[1]);

        if (0 != tmp_pass) {
            passes = tmp_pass;
        }
    }

    filename = calloc(PATH_MAX + 1, sizeof(char));
    if (2 == argc) {
        argp = argv[1];
    } else if (3 == argc) {
        argp = argv[2];
    }

    if (0 == strlcpy(filename, argp, PATH_MAX)) {
        printf("failed to copy target filename into memory!\n");
        free(filename);
        return EXIT_FAILURE;
    }
    
    filesize = get_filesize(filename);
    printf("wiping %s\n", filename);

    for (i = 0; i < passes; i++) {
        printf("\tpass: %d\t", (int) i + 1);
        if (EXIT_FAILURE == wipe(filename, filesize)) {
            printf("FAILED\n");
            return EXIT_FAILURE;
        } else {
            printf("OK!\n");
        }
    }

    free(filename);
    filename = NULL;
    return EXIT_SUCCESS;
}
        
size_t
get_filesize(char *filep)
{
    struct stat statbuf;

    if (-1 == stat(filep, &statbuf)) {
        perror("stat");
        return EXIT_FAILURE;
    }

    return statbuf.st_size;
}

int
verify_filewrite(size_t expected_filesize, char *filep)
{
    size_t filesize = get_filesize(filep);

    return expected_filesize == filesize ? EXIT_SUCCESS : EXIT_FAILURE;
}

size_t
wipe(char *filep, size_t write_size)
{
    size_t rdsz;                /* number of bytes read from DEV_RANDOM */
    size_t wrsz;                /* number of bytes written to target */
    char *random_data;          /* random data to write to the file */
    FILE *target;               /* file to be wiped */
    FILE *devrandom;            /* PRNG file */

    devrandom = fopen(DEV_RANDOM, "r");
    if (NULL == devrandom) {
        fprintf(stderr, "%s is an invalid PRNG\n", DEV_RANDOM);
        return EXIT_FAILURE;
    } else if (0 != ferror(devrandom)) {
        fprintf(stderr, "error opening %s!\n", DEV_RANDOM);
        return EXIT_FAILURE;
    }

    /* read random data from DEV_RANDOM */
    random_data = calloc(write_size, sizeof(char));
    rdsz = fread(random_data, sizeof(char), write_size, devrandom);
    fclose(devrandom);

    if (rdsz != write_size) {
        fprintf(stderr, "did not read the expected number of bytes from %s!\n",
                DEV_RANDOM);
        return EXIT_FAILURE;
    }

    /* open target for write and check for errors */
    target = fopen(filep, "w");
    if (NULL == target) {
        fprintf(stderr, "%s does not exist!\n", filep);
        fclose(devrandom);
        return EXIT_FAILURE;
    } else if (0 != ferror(target)) {
        fprintf(stderr, "error opening %s\n", filep);
        fclose(devrandom);
        return EXIT_FAILURE;
    }

    wrsz = fwrite(random_data, sizeof(char), write_size, target);
    fclose(target);
    free(random_data);
    random_data = NULL;

    if (wrsz != write_size) {
        fprintf(stderr, "failed writing the expected number of bytes to %s!\n",
                filep);
        return EXIT_FAILURE;
    }
     

    return EXIT_SUCCESS;
}

