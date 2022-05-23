#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

sem_t EmptyBuffer,FullBuffer,LockBuffer,LockCounter;
int Counter, Buffer ;


int randomgen()
{
    int lower = 1, upper = 20;
    int num = (rand()% ( upper - lower + 1) + lower );
    printf("%d",num);
    return num ;
}


void *counterThread(void * args)
{

    while(1)
    {
        printf("Counter %d : Message Recived \n ", 4  );
        printf("Counter %d : Waiting to Write Message \n ", 4  );

        sem_wait(&LockCounter);
        Counter++;
        printf("Counter %d : Message Added to Counter , Current Count : %d \n ", 4 , Counter );
        sem_post(&LockCounter);

        // rand = randomgen();
        sleep(rand()%20);
    }

}

void *collector(void * args)
{

    while(1)
    {

        int check, Temp ;
        sem_getvalue(&EmptyBuffer,check);
        if(check==0)
        {
            printf("Collector Thread : BUFFER EMPTY \n");
        }
        else
        {

            sem_wait(&EmptyBuffer);
            sem_wait(&LockBuffer);
            printf("Monitor Thread : READING from BUFFER ");
            Buffer = 0;
            sem_post(&LockBuffer);
            sem_post(&EmptyBuffer);
        }









        //rand = randomgen();
        sleep(rand()%20);
    }

}

void *monitor(void * args)
{

    while(1)
    {

        int check, Temp ;
        sem_getvalue(&EmptyBuffer,check);
        if(check==0)
        {
            printf("Monitor Thread : BUFFER FULL \n");
        }
        else
        {
            printf("Monitor Thread : Waiting to read Counter\n");

            semWait(&LockCounter);
            printf("Monitor Thread : READING Counter Value , Current Count : %d \n ", Counter );
            Temp = Counter ;
            Counter = 0 ;
            semSignal(&LockCounter);

        }



        sem_wait(&EmptyBuffer);
        sem_wait(&LockBuffer);

        printf("Monitor Thread : WRITING into buffer ");
        Buffer = Buffer + Counter ;

        sem_post(&LockBuffer);
        sem_post(&EmptyBuffer);

        //rand = randomgen();
        sleep(rand()%20);
    }

}


void main()
{

    int n ;
    scanf("%d",&n);

    pthread_t collecterthread;
    pthread_create(&collecterthread,NULL,collector,NULL);

    pthread_t monitorthread;
    pthread_create(&monitorthread,NULL,monitor,NULL);

    pthread_t counterthread[n];
    for ( int i = 0 ; i < n ; i++)
    {
        pthread_create(&counterthread[i],NULL,counterThread,NULL);
    }


    sem_init(&LockCounter,0,1);
    sem_init(&LockBuffer,0,1);

    sem_init(&EmptyBuffer,0,10);
    sem_init(&FullBuffer,0,0);


    pthread_join(&collecterthread,NULL);
    pthread_join(&monitorthread,NULL);
    for (  int  i = 0 ; i < n ; i++)
    {
        pthread_join(&counterthread[i],NULL);
    }


}
