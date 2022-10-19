#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <pthread.h>

#define check_error(cond, userMsg)\
    do { \
        if(!(cond)){ \
            perror(userMsg); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

#define check_pthread(errNum, userMsg)\
    do {\
        int _err = (errNum);\
        if(_err > 0){ \
            errno = _err; \
            check_error(0, userMsg); \
        }\
    } while(0)

int main(int argc, char** argv){


    exit(EXIT_SUCCESS);
}