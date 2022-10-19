#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <time.h>

#define check_error(cond, userMsg)\
    do{\
        if(!(cond)){\
            perror(userMsg);\
            exit(EXIT_FAILURE);\
        }\
    } while(0)

#define check_pthread (errNum, userMsg)\
    do{\
        int _err = (errNum); \
        if(_err > 0){\
            errno = err;\
            check_error(0, userMsg);\
        }\
    }while(0)

int main(int argc, char** argv){
    check_error(argc != 2, "uwu");
    int sekunde = atoi(argv[1]);
    int mesec = atoi(argv[2]);
    mesec %= 12;

    struct tm* vreme = localtime((time_t*)&sekunde);

    char* meseci[] = {"januar", "februar", "mart", "april", "maj", 
                        "jun", "jul", "avgust", "septembar", "oktobar", 
                        "novembar", "decembar"};

    printf("%s ", meseci[vreme->tm_mon]);
    int nesto = (int)vreme->tm_mon - mesec;
    if(nesto < 0){
        nesto += 12;
    } 
    printf("%s\n", meseci[nesto]);

    exit(EXIT_SUCCESS);
}