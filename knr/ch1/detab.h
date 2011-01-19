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
#define     INPUT_SIZE      110     /* number of chars for input */


/***********/
/* structs */
/***********/

struct string {
    char *data;     /*  character buffer  */
    size_t nm;      /* number of members  */
    size_t pos;     /* position in buffer */
};

typedef struct string string_t; 


/***********************/
/* function prototypes */
/***********************/

/* lookahead: check the next character in the string to look for a specific
 * character.
 *      arguments:  string_t containing the string to search
 *                  char containing the target character to find
 *                  int set to nonzero if the position should be updated
 *      returns: EXIT_SUCCESS if the character was found
 *               EXIT_FAILURE if the character was not found
 */
int lookahead(string_t *buf, char target, int update);

void string_init(string_t *buffer, size_t nm);
int sub_tabs(string_t *buf, size_t index, size_t tab_n) ;

