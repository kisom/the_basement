/**************************************************************************
 * defscan.c                                                              *
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defscan.h"

int main(int argc, char **argv) {

    return EXIT_SUCCESS;
}

