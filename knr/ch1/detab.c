/**************************************************************************
 * detab.c                                                                *
 * 4096R/B7B720D6  "Kyle Isom <coder@kyleisom.net>"                       *
 * 2011-01-18                                                             *
 *                                                                        *
 * knr exercise 1-20                                                      *
 *                                                                        *
 * detab replaces tabs in the input with the proper number of spaces.     *
 *                                                                        *
 **************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "detab.h"

int main(int argc, char **argv) {
    string_t *buffer = calloc(1, sizeof *buffer);
    string_init(buffer, INPUT_SIZE);
    size_t cur = 0;
    size_t tab = 0;

    printf("input> ");
    fgets(buffer->data, INPUT_SIZE - 1, stdin);

    while ('\x00' != buffer->data[buffer->pos]) {
        printf("pos: %u char: 0x%x\n", (unsigned int) buffer->pos,
               (unsigned int) buffer->data[buffer->pos]);
        if ('\t' == buffer->data[buffer->pos] ) { 
            cur = buffer->pos;
            tab++; 
            while (EXIT_SUCCESS == lookahead(buffer, '\t', 1)) {
                tab++;
            }
        }

        if (tab) {
            if (! sub_tabs(buffer, cur, tab)) { }
        }
        buffer->pos++;
    }

    printf("%u tabs detected...\n", (unsigned int) tab);

    return EXIT_SUCCESS;
}


/* int lookahead() */
int lookahead(string_t *buf, char target, int update) {
    int result = EXIT_FAILURE;
    if ( buf->pos < (buf->nm - 1) ) { 
        if ( buf->data[buf->pos + 1] == target ) { 
            printf("lookahead: success!\n");
            result = EXIT_SUCCESS;
        } else {
            printf("lookahead: failed!\n");
        }

        if (update) { 
            printf("update fired!\n");
            buf->pos++; 
        }
    }

    return result;
}

void string_init(string_t *buffer, size_t nm) {
    buffer->data = calloc(nm, sizeof *buffer->data);
    buffer->nm   = nm;
    buffer->pos  = 0;
}

int sub_tabs(string_t *buf, size_t index, size_t tab_n) {
    return 0;
}

