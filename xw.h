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
 #define    DEFAULT_XTERM       "/usr/local/bin/urxvt -e"
 #define    MAX_SH_LEN          255
 #define    MAX_ENV_LEN         255
 #define    MAX_CMD_LEN         255
 #define    MAX_ARGS            64

 #define    FATAL_NO_DISPLAY    1       /* set to 0 if your terminal    
                                         * emulator doesn't require X -
                                         * if 1, and $DISPLAY isn't set,
                                         * the program will die a firey
                                         * death and take your soul with
                                         * it in the deep abyss. you have
                                         * been warned. */


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
void load_env( void );

/* basename: return a suitable argv[0] from the full path to the binary */
char *basename( const char *path );

/* buildargs: pass in the main program's argv, return a suitable argp for
 * passing off the the execve. */
char **build_args( char **argv );

/* stripargs: strip the args off xterm and pass in just the path to the
 * binary */
char *stripargs( void );

