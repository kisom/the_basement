/****************************************************************************
 * irclogd.h                                                                *
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define     DEFAULT_CONFIG                  .irclogdrc
#define     DEFAULT_PORT                    6667
#define     DEFAULT_SNAME                   example
#define     DEFAULT_MYHOST                  example.net

struct irc_config {
    char *server;
    char **channels;
    unsigned int channum;
    char *nick;
    char *username;
    char *realname;
    char *myhost;
    char *sname;
};

void usage(void);
void init_config(void);
void destroy_config(void);
int verify_config(void);
int load_configfile(char *);
size_t find_colon(char *);


