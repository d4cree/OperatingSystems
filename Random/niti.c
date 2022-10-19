#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>

#define MAX_SIZE (250)

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
    }while(0)

typedef struct {
    int najveci;
    pthread_mutex_t lock;
} Najveci;

Najveci naj;
pthread_t mutex_t;


void *function(void *arg){
    char *niz = arg;

    int i = 0;
    char niz1[MAX_SIZE];

    while(niz[i] != " "){
        strcat(niz1, niz[i]);
        i++;
    }
    int duzina = atoi(niz1);
    int *brojevi = malloc(sizeof(int) * duzina);
    
    for(int j = 0; j < duzina; j++){
        i++;
        while(niz[i] != " "){
            strcpy(niz1, niz[i]);
            i++;
        }
        brojevi[j] = atoi(niz1);
    }

    int max = brojevi[0];
    for(int j = 0; j < duzina; j++){
        if(max < brojevi[j]){
            max = brojevi[j];
        }
    }

    return NULL;
}

int main(int argc, char** argv){

    check_error(argc != 1, "Nije tacno prosledjeno");

    FILE* fd = fopen(argv[1], "r");
    check_error(fd != NULL, "Nije dobro ocitan fajl");


    char* linija = NULL;
    size_t duzina_linije = 0;
    check_error(getline(&linija, &duzina_linije, fd) != -1, "Lose ocitana linija");
    
    int numOfThreads = atoi(linija);
    pthread_t *tids = malloc(numOfThreads*sizeof(pthread_t));
    check_error(tids != NULL, "Losi id-evi");

    check_pthread(pthread_mutex_init(&naj.lock, NULL), "Muteks greska");


    for(int i = 0; i < numOfThreads; i++){

        check_error(getline(&linija, &duzina_linije, fd) != -1, "Lose u petlji");
        check_pthread(pthread_create(&tids[i], NULL, function, linija), "Greska u niti");
    }

    for(int i = 0; i < numOfThreads; i++){
        check_pthread(pthread_join(tids[i], NULL), "Greska u niti");
    }

    check_pthread(pthread_mutex_destroy(&naj.lock), "Lose unistenje");
    

    
    free(linija);
    free(tids);
    fclose(fd);
    exit(EXIT_SUCCESS);
}