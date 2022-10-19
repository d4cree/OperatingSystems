#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <errno.h>
#include <math.h>

#define check_error(cond, userMsg) \
    do{ \
        if(!(cond)){\
        perror(userMsg);\
        exit(EXIT_FAILURE);\
        }\
    } while (0)

#define check_pthread(errNum, userMsg)\
    do {\
        int _err = (errNum);\
        if(_err > 0){\
            errno = _err;\
            check_error(0, userMsg);\
        }\
    } while (0)


typedef struct {
    int sum;
    pthread_mutex_t lock;
}Suma;

Suma suma;
pthread_t mutex_t;

typedef struct{
    int indeks;
    unsigned broj;
    unsigned povratak;
}BozeMoj;


void *function(void *arg){
    BozeMoj *broj = arg;
    int brojCopy = broj->broj;
    int brojCopy1 = broj->broj;
    int i = 0;
    while(brojCopy1 > 0){
        i++;
        brojCopy1 /= 10;
    }
    int prva_cifra = brojCopy/pow(10, i-1);
    int poslednja_cifra = brojCopy%10;
    int izmedju = brojCopy - prva_cifra*pow(10, i-1) - poslednja_cifra;
    int noviBroj = izmedju + prva_cifra + poslednja_cifra*pow(10, i-1);

    check_pthread(pthread_mutex_lock(&suma.lock), "Greskilic");

    suma.sum += noviBroj;
    broj->povratak = noviBroj; 

    check_pthread(pthread_mutex_unlock(&suma.lock), "Greskica");

    return NULL;

}

int main(int argc, char** argv){
    
    int n;

    check_error(argc != 0, "hehexD");
    
    scanf("%d", &n);
    BozeMoj *brojevi = malloc(n*sizeof(BozeMoj));
    check_error(brojevi != NULL, "ups");
    
    for(int i = 0; i < n; i++){
        scanf("%u", &brojevi[i].broj);
        brojevi[i].indeks = i;
        brojevi[i].povratak = 0;
    }

    int numOfThreads = n;
    pthread_t *tids = malloc(numOfThreads*sizeof(pthread_t));
    check_error(tids != NULL, "OhNe");

    check_pthread(pthread_mutex_init(&suma.lock, NULL), "jaojaojao100xjao");

    for(int i = 0; i < numOfThreads; i++){
        check_pthread(pthread_create(&tids[i], NULL, function, brojevi+i), "uwu"); 
    }

    for(int i = 0; i < numOfThreads; i++){
        check_pthread(pthread_join(tids[i], NULL), "owo");
    }

    check_pthread(pthread_mutex_destroy(&suma.lock), "jaojaojao100xjao");

    for(int i = 0; i < n; i++){
        printf("%u ", brojevi[i].povratak);
    }
    
    printf("\n%u", suma.sum);

    
    free(tids);
    free(brojevi);

    

    exit(EXIT_SUCCESS);
}