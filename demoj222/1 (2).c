#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>

#include <ftw.h>

#define check_error(cond, userMsg) \
    do { \
        if(!(cond)){\
            perror(userMsg);\
            exit(EXIT_FAILURE);\
            }\
    } while(0)

#define check_pthread(errNum, userMsg) \
    do {\
        int _err = (errNum); \
        if(_err > 0){\
            errno = _err;\
            check_error(0, userMsg);\
        } \
    } while(0)

int filter(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
    if(typeflag == FTW_D){
        if(!(sb->st_mode & S_IWGRP) && !(sb->st_mode & S_IWOTH)){
            char* ime = "test_\0";
            int i = 0;

            while(fpath[i] != '\0'){
                i++;
            }

            while(fpath[i] != '/' && i > 0){
                i--;
            }

            i++;

            for(int j = 0; j < 5; j++){
                if(fpath[i+j] != ime[j]){
                    return 0;
                }
            }

            while(fpath[i] != '\0'){
                printf("%c", fpath[i]);
                i++;
            }
            printf("\n");
        }
    }

    return 0;
}

int main(int argc, char** argv){
    
    check_error(argc != 1, "...");
    struct stat fileinfo;
    check_error(stat(argv[1], &fileinfo) != -1, "...");
    if(!S_ISDIR(fileinfo.st_mode)){
        check_error(-1, "Nije direktorijum");
    }
    check_error(nftw(argv[1], filter, 50, FTW_CHDIR | FTW_PHYS) != -1, "uwoahwoahwaoh");



    exit(EXIT_SUCCESS);
}