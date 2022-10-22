#include <iostream>
#include "include/Sem.h"


Sem::Sem()
{}

Semaphore* Sem::MakeSemaphore(int value)
{
    semaphore = (Semaphore*)malloc(sizeof(Semaphore));
    int n = sem_init(semaphore, 0, value);
    if (n != 0) perror("sem_init failed");
    return semaphore;
}

void Sem::SemaphoreWait(Semaphore *sem)
{
    int n = sem_wait(sem);
    if (n != 0) perror("sem_wait failed");
}

void Sem::SemaphoreSignal(Semaphore *sem)
{
    int n = sem_post(sem);
    if (n != 0) perror("sem_post failed");
}




