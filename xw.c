/**************************************************************************
 * xw.c                                                                   *
 * 4096R/B7B720D6 "Kyle Isom <coder@kyleisom.net>"                        *
 * 2011-01-13                                                             *
 *                                                                        *
 * xterm wrapper - passes arguments to xterm                              *
 *                                                                        *
 * i am bored right now.                                                  *
 * massive attack is playing, so                                          *
 * i need to write C.                                                     *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "xw.h"

char xterm[MAX_SH_LEN]  = DEFAULT_XTERM;    /* terminal emulator */
char *path              = NULL;             /* path env variable */
/* char *xenv; */
char **env;                                 /* execve env */

int main(int argc, char **argv) {
    char *cmd           = NULL;         /* command buffer */
    char **cli          = NULL;         /* pointer to argv[1] */

    if (1 == argc) {
        /* if no command line is passed, die a horrifically flaming death */
        usage(argv[0]);
    }

    get_xterm();            /* determine which terminal emulator to use */
    load_path();
    cli = build_args(argv);

    cmd = calloc(MAX_SH_LEN + MAX_CMD_LEN, sizeof cmd);
    if (NULL == cmd) {
        fprintf(stderr, "calloc() failed!\n");
        return EXIT_FAILURE;
    }

    strncpy(cmd, xterm, MAX_SH_LEN);

    execve( (const char *) xterm, cli, env );
     
    perror("execve");
    
    return EXIT_SUCCESS;
}

void usage(char *bin) {
    printf("usage: %s <command line program to pass to xterm>\n", bin);
    printf("\tthere are two places %s searches for a suitable "
           "default terminal \n\temulator:\n", bin);
    printf("\t\t* first, the value of the environment variable %s\n",
           ENV_VAR_NAME);
    printf("\t\t* second, in ${HOME}/%s which may have a maximum\n\t\t  length "
           "of %u bytes\n", CONFRC_PATH, (unsigned int) MAX_SH_LEN);

    exit(EXIT_SUCCESS);
}

void get_xterm( ) {
    char *env_xw        = NULL;     /* pointer to results of getenv() */
    char *confrc_path   = NULL;     /* char buffer holding path to confrc */
    FILE *confrc         = NULL;    /* file pointer to CONFRC_PATH */
    size_t readsz       = 0;        /* number of bytes read */

    env_xw = getenv(ENV_VAR_NAME);

    if (NULL == env_xw) {
        /* if $XWRAPPER isn't defined, look in the conf path */
        env_xw = getenv("HOME");
        confrc_path = calloc(MAX_ENV_LEN, sizeof confrc_path);
        snprintf(confrc_path, MAX_ENV_LEN, "%s/%s", env_xw, CONFRC_PATH);

        confrc = fopen(confrc_path, "r"); 
        if (NULL == confrc) {
            /* do any error handling here */
        } else {
            readsz = fread(xterm, sizeof *xterm, MAX_SH_LEN, confrc);
            if ((0 == readsz) && (ferror(confrc))) {
                perror("fread");
            }   /* on success, xterm already has already been loaded */
            if (0 != fclose(confrc)) {
                perror("fclose");
            }      

            confrc = NULL; 
        }
        free(confrc_path);
        confrc_path = NULL;

    } else {
        strncpy(xterm, env_xw, MAX_SH_LEN);
    }

    if (0 != strncmp(xterm, DEFAULT_XTERM, MAX_SH_LEN)) {
        printf("new terminal emulator: %s\n", xterm);
    }

    return;
}

void load_path( ) {
    char *env_xw        = NULL;     /* pointer to result of getenv() */
    env = calloc(2, sizeof env);
    
    path = calloc(MAX_ENV_LEN, sizeof path);
    if (NULL == path) {
        return;
    }
    strncpy(path, "path=", 0x05);   /* why hex? BECAUSE */

    env[0] = path;
    env[1] = NULL;

    env_xw = getenv("PATH");

    if (NULL != env_xw) {
        /* copy path size minus 'path=' to path */
        strncat(path, env_xw, MAX_ENV_LEN - 5);
    }

    return;
}

char *basename(const char *path) {
    char *result    = NULL;
    size_t path_len = strlen(path);
    size_t i        = path_len;
    size_t start    = 0;
    size_t end      = path_len;

    for (i = path_len; i >= 0; --i) {
        /* a space means we're closer to the end of the binary */
        if (0x20 == path[i]) {      
            end = i - 1;
        }
        /* stop at the first slash - this denotes the start of the basename */
        if (0x2F == path[i]) {
            start = i + 1;
            break;
        }
    }

    path_len = (end - start) + 1;
    result  = calloc((end - start), sizeof result);
    for (i = 0; i < path_len; ++i)
        result[i] = path[i + start];


    return result;
}    

char **build_args(char **argv) {
    char **arg      = calloc(MAX_ARGS, sizeof arg);
    size_t argc     = 0;    /* current arg */
    size_t i        = 0;    /* outer loop index */
    size_t j        = 0;    /* inner loop index */
    size_t start    = 0;    /* start of current arg */
    size_t subsz    = 0;    /* size of substring */

    /* argv[0] needs to be the basename of the terminal emulator */
    arg[argc++] = basename(xterm);

    for (i = 0; i < strlen(xterm); ++i) {
        if (0x20 != xterm[i]) { continue; }
        start = ++i;

        do {
            /* skip until the next space */
            if (0x20 != xterm[i] && 0x0 != xterm[i + 1]) { 
                printf("%d: %c\n", i, xterm[i]);
                i++;
                continue; 
            } 

            /* set up arg[argc] as the current substr */
            subsz = i - start;  /* substr is from start to j - 1, add an extra *
                                 * char for the \x00 terminator.               */
            arg[argc] = calloc(subsz, sizeof arg[argc]);
            for (j = start; j <= i; ++j) {
                printf("=>%c\n", xterm[j]);
                arg[argc][j - start] = xterm[j];
                printf("!%c\n", arg[argc][i - j]);
            }
            argc++;
            break;
        } while (i <= strlen(xterm));
    } 
    
                

    return arg;
}

