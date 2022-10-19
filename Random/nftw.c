#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <ftw.h>
#include <limits.h>
#include <time.h>

#define check_error(cond, userMsg)\
    do{\
        if(!(cond)){\
            perror(userMsg);\
            exit(EXIT_FAILURE);\
        }\
    }while(0)

#define check_pthread(errNum, userMsg)\
    do{\
        int _err = (errNum);\
        if(_err > 0){\
            errno = _err;\
            check_error(0, userMsg);\
        }\
    }while(0)

int function(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
    if(typeflag == FTW_F){
        char* putanja = realpath(fpath, NULL);
        printf("%s ", putanja);

        struct stat fileInfo;
        check_error((stat(fpath, &fileInfo)) != -1, "Radimo malo trolovanja");
        struct tm* vreme = localtime(&fileInfo.st_mtime);
        check_error(vreme != NULL, "Greska u vremenu");
        int dan = vreme->tm_wday;
        char* dani[] = {"nedelja", "ponedeljak", "utorak", 
                    "sreda", "cetvrtak", "petak", "subota"};
    
        printf("%s\n", dani[dan]);
    }
    

    return 0;
}

int main(int argc, char** argv){

    check_error(argc != 1, "Nedovoljan broj fajlova");
    struct stat fileInfo;
    check_error(stat(argv[1], &fileInfo) != -1, "fileInfo greska");
    if(!(S_ISDIR(fileInfo.st_mode))){
        check_error(0, "Nije dir");
    }
    check_error((nftw(argv[1], function, 50, FTW_CHDIR | FTW_PHYS)) != -1, "NFTW greska");

    exit(EXIT_SUCCESS);
}
