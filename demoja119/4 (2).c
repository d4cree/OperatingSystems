#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>

#define check_error(cond, userMsg)\
    do { \
        if(!(cond)) { \
            perror(userMsg);\
            exit(EXIT_FAILURE);\
        } \
    } while(0)

#define check_pthread(errNum, userMsg) \
    do {\
        int _err = (errNum); \
        if(_err > 0) { \
            errno = _err; \
            check_error(0, userMsg); \
        }\
    }while(0)


#define MAX_SIZE (255)
#define RD_END (0)
#define WR_END (1)


int main(int argc, char** argv){

    check_error(argc != 2, "...");

    FILE*f = fopen(argv[1], "r");
    check_error(f != NULL, "...");

    char komanda[MAX_SIZE];
    char argument[MAX_SIZE];
    
    int maxBytes = -1;
    char maxCommand[2*MAX_SIZE];

    while(fscanf(f, "%s%s", komanda, argument) == 2){
        int pipes[2];
        check_error(pipe(pipes) != -1, "...");

        pid_t childPid = fork();
        check_error(childPid != -1, "...");

        if(childPid > 0){

            close(pipes[WR_END]);

            int localBytes = 0;
            int readBytes = 0;
            char buffer[MAX_SIZE];
            while((readBytes = read(pipes[RD_END], buffer, MAX_SIZE)) > 0){
                localBytes += readBytes;
            }

            check_error(readBytes != -1, "...");

            if(localBytes > maxBytes){
                maxBytes = localBytes;
                sprintf(maxCommand, "%s %s", komanda, argument);
            }

            close(pipes[RD_END]);

        } else {
            close(pipes[RD_END]);
            
            check_error(dup2(pipes[WR_END], STDOUT_FILENO) != -1, "...");

            check_error(execlp(komanda, komanda, argument, NULL) != -1, "...");
        }

        check_error(wait(NULL) != -1, "...");
    }

    check_error(feof(f), "...");

    printf("%s\n", maxCommand);

    fclose(f);

    exit(EXIT_SUCCESS);
}