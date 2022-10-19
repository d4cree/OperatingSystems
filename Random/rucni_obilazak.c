#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>

#define check_error(cond, userMsg)\
    do{\
        if(!(cond)){\
        perror(userMsg);\
        exit(EXIT_FAILURE);\
        }\
    } while(0)

#define check_pthread(errNum, userMsg)\
    do{\
        int _err = (errNum);\
        if(_err > 0){\
            errno = _err;\
            check_error(0, userMsg);\
        }\
    } while(0)

void sizeOfDir(const char* filePath, int* dirSize){
    struct stat fileInfo;
    check_error(lstat(filePath, &fileInfo) != -1, "uwuci ova muda");
    if(!(S_ISDIR(fileInfo.st_mode))){
        *dirSize += fileInfo.st_size;
        return;
    } else {
        *dirSize += fileInfo.st_size;
    }

    DIR* dir = opendir(filePath);
    check_error(dir != NULL, "Prazno :(");

    check_error(chdir(filePath) != -1, "Poruka :D");
    struct dirent *dirent = NULL;
    errno = 0;
    while((dirent = readdir(dir)) != NULL){
        if((strcmp(dirent->d_name, ".")) == 0 || (strcmp(dirent->d_name, "..")) == 0){
            check_error(stat(dirent->d_name, &fileInfo) != -1, filePath);
            *dirSize += fileInfo.st_size;
            errno = 0;
            continue;
        }
        sizeOfDir(dirent->d_name, dirSize);
        errno = 0;
    }

    check_error(errno != EBADF, "crying");
    check_error(chdir("..") != -1, "sadge");
    check_error(closedir(dir) != -1, "Zatvori");
    return;

}

int main(int argc, char** argv){

    check_error(argc != 1, "uwu");
    struct stat fileInfo;
    check_error(stat(argv[1], &fileInfo) != -1, "OwO");

    if(!(S_ISDIR(fileInfo.st_mode))){
        check_error(-1, "Hello");
    }
    
    unsigned dirSize = 0;
    sizeOfDir(argv[1], &dirSize);
    printf("%u\n", dirSize);

    exit(EXIT_SUCCESS);
}

