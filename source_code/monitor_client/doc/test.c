#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int play = 1;

void *MyThread(void *android)
{
    for (;;)
    { /* Playback loop */
        pthread_mutex_lock(&lock);
        while (!play)
        {                                    /* We're paused */
            pthread_cond_wait(&cond, &lock); /* Wait for play signal */
        }
        pthread_mutex_unlock(&lock);
        /* Continue playback */
        printf("Run...\n");
        sleep(1);
    }
}

void Pause()
{
    pthread_mutex_lock(&lock);
    play = 0;
    pthread_mutex_unlock(&lock);
}

void Continue()
{
    pthread_mutex_lock(&lock);
    play = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);
}

void *MyController(void *par)
{
    pthread_t myThread= *((pthread_t*)par);
    while (1)
    {
        int i;
        scanf("%d", &i);
        if(i==0){
            Pause();
        }else if(i==1){
            Continue();
        }else{
            pthread_cancel(myThread);
            pthread_exit(NULL);
        }
    }
}

int main(int argc, char **argv)
{
    pthread_t myThread, myController;
    pthread_create(&myThread, NULL, &MyThread, NULL);
    pthread_create(&myController, NULL, &MyController, (void *)&myThread);
    pthread_join(myThread, NULL);
    pthread_join(myController, NULL);

    return 0;
}