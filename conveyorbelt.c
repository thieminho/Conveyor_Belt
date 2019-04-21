#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 10
#define MAXTHREADS 10
int poczatek ,koniec, ilosccegielnatasmie = 0;
int tasma[N];
pthread_mutex_t mutex;
pthread_cond_t pusta, pelna;

void* producent( void* arg ) {
    int num = 0;
    num = (int) arg;
    printf("Pracownik wkladajacy: %d\n",num);
    while(1) {
        pthread_mutex_lock( &mutex );
        int cegla;
        srand(time(0));
        cegla = 1 + rand() % 2;
        int suma = 0;
        for(int i=0;i<N;i++)
            suma = suma+tasma[i];
        while(ilosccegielnatasmie >= N || suma + cegla >10)
            pthread_cond_wait(&pusta,&mutex);
        tasma[koniec] = cegla;
        koniec = (koniec+1) %N;
        ilosccegielnatasmie++;
        printf("Pracownik wkladajacy %d cegla wlozona: %d suma przed: %d ilosc cegiel: %d\n",num, cegla, suma, ilosccegielnatasmie );
        pthread_cond_signal(&pelna);
        pthread_mutex_unlock( &mutex );
        sleep(1);
        }
}
void* konsument( void* arg ) {
     int num, x = 0;
     num = (int) arg;
     printf("Start konsument: %d\n",num);
     while(1) {
         pthread_mutex_lock( &mutex );
         while(ilosccegielnatasmie ==0)
            pthread_cond_wait(&pelna,&mutex);
         x = tasma[poczatek];
         tasma[poczatek] = 0;
         poczatek = (poczatek+1)%N;
         ilosccegielnatasmie--;
         printf("Pracownik odbierajacy %d  cegla wzieta: %d ilosc cegiel: %d\n ",num, x,ilosccegielnatasmie);
         pthread_cond_signal(&pusta);
         pthread_mutex_unlock( &mutex );
         sleep(1);
        }
}

int main() {
    pthread_t k[MAXTHREADS], p[MAXTHREADS];
    int  i, w;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&pusta,NULL);
    pthread_cond_init(&pelna,NULL);
    for (i=0; i< MAXTHREADS; i++)
        w = pthread_create( &p[i], NULL, producent, (void *)i);
    for (i=0; i< MAXTHREADS-5; i++)
        w = pthread_create( &k[i], NULL, konsument, (void *)i);
    for (i=0; i< MAXTHREADS; i++)
        pthread_join ( p[i],   NULL);
    for (i=0; i< MAXTHREADS-5; i++)
        pthread_join ( k[i],   NULL);
    return 0;
}
