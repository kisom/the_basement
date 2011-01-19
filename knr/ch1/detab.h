/**************************************************************************
 * detab.h                                                                *
 * 4096R/B7B720D6  "Kyle Isom <coder@kyleisom.net>"                       *
 * 2011-01-18                                                             *
 *                                                                        *
 * knr exercise 1-20                                                      *
 *                                                                        *
 * detab replaces tabs in the input with the proper number of spaces.     *
 *                                                                        *
 **************************************************************************/

#include <stdlib.h>     /* for size_t */

/***********/
/* defines */
/***********/

#define     TABSTOP         4


/***********/
/* structs */
/***********/

typedef struct string {
    char *data;
    size_t nm;
} string_t, *pstring_t;


