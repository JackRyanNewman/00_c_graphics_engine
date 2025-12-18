#ifndef D0EXTRADEBUG_H
#define D0EXTRADEBUG_H
/*=================================================================================================*/
/* Includes
fprintf(stderr, ": %s\n", strerror(errno));       \
*/
#define DEBUG_MODE 1
#define VERB 3
#include <errno.h>
#include <stdio.h>  
#include <stdlib.h>








#define crit_error(...)                           \
fprintf(stderr, __VA_ARGS__);                     \
exit(0);\

#define exit_error(...)                           \
fprintf(stderr, __VA_ARGS__);                     \
exit(0);\

#define val_crit_error_1(cond, ret, ...)           \
if(cond) {                                         \
    fprintf(stderr, __VA_ARGS__);                  \
    exit(0);                                    \
}

#define val_crit_error_2(cond, ret, act, ...)      \
if(cond) {                                         \
    fprintf(stderr, __VA_ARGS__);                  \
    act;                                           \
    exit(0);                                    \
}
                      
#define val_exit_error_1(cond, ...)           \
if(cond) {                                         \
    fprintf(stderr, __VA_ARGS__);                  \
    exit(0);                                   \
}

#define val_exit_error_2(cond, act, ...)      \
if(cond) {                                         \
    fprintf(stderr, __VA_ARGS__);                  \
    act;                                           \
    exit(0);                                    \
}

#endif