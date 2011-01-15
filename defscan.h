/**************************************************************************
 * defscan.h                                                              *
 * 4096R/B7B720D6  "Kyle Isom <coder@kyleisom.net>"                       *
 * 2011-01-14                                                             *
 *                                                                        *
 * scan header files for unused defines                                   *
 *                                                                        *
 * looks through either a single header file or every header file in the  *
 * directory, looking for #define statements and checking to see if those *
 * are used at all in the source. the idea is not to check to see which   *
 * code maybe included, for example, but to identify obsolete #defines.   *
 *                                                                        *
 **************************************************************************/

#include <stdbool.h>

/***********************
 * defines / constants *
 ***********************/
#define         MAX_SOURCES              8
#define         MAX_TARGETS             32
#define         MAX_DEFINES             64
#define         LINEBUF_SZ             128

typedef enum filetype {
    FILE_OTHER  = 0,
    FILE_CHDR,          /* C header file */
    FILE_CSRC,          /* C source file */
} filetype_t;


/**********************
 * structs / typedefs *
 **********************/

typedef struct define_set {
    char *macro;        /* the macro name                           */
    size_t used;        /* number of times the macro was matched    */
} defset, *defset_t;



/***********************
 * function prototypes *
 ***********************/

void dirwalk( const char *, char **, size_t *, bool );
void scanload_macros(const char *header);

