/****************************************************************************
 * irclogd.c                                                                *
 * 4096R/B7B720D6 "Kyle Isom" <coder@kyleisom.net>                          *
 * license: ISC / public domain dual-license (see end of header comments)   *
 * start: 2011-08-05                                                        *
 * last modified: 2011-08-05                                                *
 *                                                                          *
 * simple irc logger - log into several irc channels on the same server     *
 * and write everything in the channel to disk.                             *
 *                                                                          *
 * you're all i need,                                                       *
 * you're all i need,                                                       *
 * i'm in the middle of your picture,                                       *
 * lying in the reeds...                                                    *
 *                                                                          *
 ****************************************************************************
 * the ISC license:                                                         *
 * Copyright (c) 2011 Kyle Isom <coder@kyleisom.net>                        *
 *                                                                          *
 * Permission to use, copy, modify, and distribute this software for any    *
 * purpose with or without fee is hereby granted, provided that the above   *
 * copyright notice and this permission notice appear in all copies.        *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES *
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF         *
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR  *
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES   *
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN    *
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF  *
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.           *
 ****************************************************************************
 * you may choose to follow this license or public domain. my intent with   *
 * dual-licensing this code is to afford you, the end user, maximum freedom *
 * with the software. if public domain affords you more freedom, use it.    *
 ****************************************************************************/

#include <daemon.h>         /* libdaemon: http://github.com/kisom/libdaemon */
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "irclogd.h"

static struct irc_config cfg;

int
main(int argc, char **argv)
{
    int opt, retval;

    retval = EXIT_FAILURE;

    while (-1 != (opt = getopt(argc, argv, "hf:"))) {
        switch(opt) {
        case 'h':
            usage();
            break;
        default:
            usage();
            /* NOTREACHED */
        }

    }
    argc -= optind;
    argv += optind;

    if (EXIT_FAILURE == init_config())
        return retval;

    destroy_config();
    return retval;
}

void
usage( )
{
    extern char *__progname;
    fprintf(stderr, "usage: %s [-f configfile]\n", __progname);
    exit(EXIT_FAILURE);
}

/*
 * initialise irc_config structure to all zeroes
 */
int
init_config()
{
    int retval;

    retval = EXIT_FAILURE;

    if (EXIT_FAILURE == verify_config())
        ;   /* nothing */
    else {
        cfg.server      = NULL;
        cfg.channels    = NULL;
        cfg.channum     = 0;
        cfg.nick        = NULL;
        cfg.username    = NULL;
        cfg.realname    = NULL;
        cfg.myhost      = NULL;
        cfg.sname       = NULL;
        retval          = EXIT_SUCCESS;
    }

    return retval;
}

void
destroy_config()
{
    size_t i;

    if (NULL != cfg.channels)
        for (i = 0; i < cfg.channum; ++i) {
            free(cfg.channels[i]);
            cfg.channels[i] = NULL;
        }

    cfg.channels = NULL;

    free(cfg.server);
    free(cfg.nick);
    free(cfg.username);
    free(cfg.realname);
    free(cfg.myhost);
    free(cfg.sname);
    cfg.server      = NULL;
    cfg.nick        = NULL;
    cfg.username    = NULL;
    cfg.realname    = NULL;
    cfg.myhost      = NULL;
    cfg.sname       = NULL;

    return;
}

/*
 * verify config struct is valid. if any member is NULL or if the number of 
 * channels does not match the reported number or if the list of channels is
 * not NULL-terminated, the config struct is invalid.
 */
int
verify_config()
{
    size_t i;
    int invalid_chans, retval;

    retval          = EXIT_FAILURE;
    invalid_chans   = -1;

    if (NULL != cfg.channels)
        for (i = 0; i < cfg.channum + 1; ++i)
            if (NULL == cfg.channels[i])
                break;

    if (i == cfg.channum)
        invalid_chans = 0;

    if ((NULL == cfg.server)        ||
        (NULL == cfg.channels)      ||
        (0 == cfg.channum)          ||
        (NULL == cfg.nick)          ||
        (NULL == cfg.username)      ||
        (NULL == cfg.realname)      ||
        (NULL == cfg.myhost)        ||
        (NULL == cfg.sname))
        ;   /* nothing */
    else
        retval = EXIT_SUCCESS;


    return invalid_chans == 0 ? retval : EXIT_FAILURE;
}

int
load_configfile(char *configfile)
{
    int retval;

    retval = EXIT_FAILURE;

    return retval;
}

size_t
find_colon(char *line)
{
    size_t colon, i, strsz;

    strsz = strlen(line);
    colon = 0;

    for (i = 0; i < strsz; ++i)
        if (':' == line[i]) {
            colon = i;
            break;
        }

    return colon;
}
