#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <pthread.h>

#include <math.h>

#define check_error(cond, userMsg) \
    do { \
        if(!(cond)) { \
            perror(userMsg); \
            exit(EXIT_FAILURE); \
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

typedef struct {

    int idx;
    int numVect;
} inputArg_t;

typedef struct{
    int idx;
    double norma;
} outputArg_t;


int m, n, k;
double ** vektori;

void* threadFunc(void* a){

    inputArg_t* arg = (inputArg_t*)a;

    int startIdx = arg->numVect * arg->idx;
    int endIdx = startIdx + arg->numVect;

    double maxNorma = -1;
   
    for(int i = startIdx; i < endIdx; i++){
        double norma = 0;
        for(int j = 0; j < n; j++){
            norma += vektori[i][j]*vektori[i][j];
        }
        norma = sqrt(norma);

        if(norma > maxNorma){
            maxNorma = norma;
        }
    }

    outputArg_t* out = malloc(sizeof(outputArg_t));
    check_error(out != NULL, "...");

    out -> idx = arg -> idx;
    out -> norma = maxNorma;

    return out;
}

int main(int argc, char** argv){

    scanf("%d%d%d", &m, &n, &k);
    vektori = malloc(m*sizeof(double*));
    check_error(vektori != NULL, "...");

    for(int i = 0; i < m; i++){

        vektori[i] = malloc(n*sizeof(double));
        check_error(vektori[i] != NULL, "...");
    }

    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            scanf("%lf", &vektori[i][j]);
        }
    }

    pthread_t* tids = malloc(k*sizeof(pthread_t));
    check_error(tids != NULL, "...");

    inputArg_t* args = malloc(k*sizeof(inputArg_t));
    check_error(args != NULL, "...");

    for(int i = 0; i < k; i++){
        args[i].idx = i;
        args[i].numVect = m/k;
        check_pthread(pthread_create(&tids[i], NULL, threadFunc, &args[i]), "...");
    }

    double maxNorma = -1;
    int maxIdx = -1;
    for(int i = 0; i < k; i++){
        outputArg_t* retVal = NULL;
        check_pthread(pthread_join(tids[i], (void**)&retVal), "...");

        if(retVal -> norma > maxNorma){
            maxNorma = retVal -> norma;
            maxIdx = retVal -> idx;
        }

        free(retVal);
    }

    printf("%d %lf\n", maxIdx, maxNorma);

    free(args);
    free(tids);

    for(int i = 0; i < m; i++){
        free(vektori[i]);
    }
    free(vektori);


    exit(EXIT_SUCCESS);
}