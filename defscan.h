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

/***********************
 * defines / constants *
 ***********************/

typedef struct define_set {
    char *macro;        /* the macro name                           */
    size_t used;        /* number of times the macro was matched    */
} defset, *defset_t;
