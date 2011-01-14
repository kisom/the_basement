/**************************************************************************
 * xw.h                                                                   *
 * 4096R/B7B720D6 "Kyle Isom <coder@kyleisom.net>"                        *
 * 2011-01-13                                                             *
 *                                                                        *
 * xterm wrapper - passes arguments to xterm                              *
 *                                                                        *
 * i am bored right now.                                                  *
 * massive attack is playing, so                                          *
 * i need to write C.                                                     *
 *                                                                        *
 * bsd licensed.                                                          *
 **************************************************************************/


 /************************************************************************/
 /* defines follows                                                      */
 /************************************************************************/
 #define    ENV_VAR_NAME        "XWRAPPER"
 #define    CONFRC_PATH         ".config/.xwrc"
 #define    DEFAULT_XTERM       "/usr/bin/xterm -c\x00"
 #define    MAX_SH_LEN          255
 #define    MAX_ENV_LEN         255
 #define    MAX_CMD_LEN         255
 #define    MAX_ARGS            64


 /************************************************************************/
 /* function prototypes follow                                           */
 /************************************************************************/

/* usage: print usage and die 
 *      parameters: argv[0]
 */
void usage( char *bin );

/* get_xterm: searches for XWRAPPER environment variable; if found sets 
 *      xw to use the shell specified.
 */
void get_xterm( void );

/* load_path: loads the path and builds the environment for the execve call
 */
void load_path( void );

char *basename(const char *path);
char **build_args(char **argv);

