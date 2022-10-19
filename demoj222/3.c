#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <errno.h>
#include <wait.h>

#define MAX_KARAKTERA (255)
#define RD_END (0)
#define WR_END (1)

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

int main(int argc, char** argv){

    int pipe[2];
    check_error((pipe(pipe)) != -1, "Greska");
    check_error(argc != 1, "Greskica");
    pid_t childeId = fork();
    check_error(childeId != -1, "Greskilic");
    
    if(childeId > 0){
        close(pipe[WR_END]);

    } else {
        close(pipe[RD_END]);
        dup2(open("/dev/null", O_WRONLY), STDOUT_FILENO);
        dup2(open("/dev/null", O_WRONLY), STDOUT_FILENO);
        execvp(argv[1], &argv[1]);
        exit(EXIT_FAILURE);
    }

    int status;
    wait(&status);
    if(WIFEXITED(status)){
        if(WEXITSTATUS(status) == EXIT_SUCCESS){
        } else {
            check_error(0, "Greska1");
        }

    } else {
        check_error(0, "Greska2");
    }


    exit(EXIT_SUCCESS);
}