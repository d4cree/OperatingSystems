#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>

#define check_error(cond, userMsg)\
    do {\
        if(!(cond)) {\
            perror(userMsg);\
            exit(EXIT_FAILURE);\
            }\
        } while(0)

#define check_pthead(errNUm, userMsg) \
    do {\
        int _err = (errNum);\
        if(_err > 0) {\
            errno = _err; \
            check_error(0, userMsg);\
        }\
    } while(0)

#define RD_END (0)
#define WR_END (0)

int main (int argc, char** argv){

    check_error(argc == 2, "...");

    int pipes[2];
    check_error(pipe(pipes) != -1, "...");

    pid_t childPid = fork();
    check_error(childPid != -1, "...");

    if(childPid == 0){

        close(pipes[RD_END]);
         
        check_error(dup2(pipes[WR_END], STDOUT_FILENO) != 1, "...");

        check_error(execlp("stat", "stat", "--format=%s", argv[1], NULL) != -1, "...");
    }

    close(pipes[WR_END]);

    char buffer[100];
    int bytesRead = read(pipes[RD_END], buffer, 99);
    check_error(bytesRead != -1, "...");

    buffer[bytesRead] = 0;

    int status = 0;
    check_error(wait(&status) != -1, "...");

    if(WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)) {
        printf("%s\n", buffer);
    } else {
        printf("Neuspeh\n");
    }

    exit(EXIT_SUCCESS);
}