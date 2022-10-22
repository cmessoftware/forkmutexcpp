#include<semaphore.h>

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef sem_t Semaphore;
   
class Sem
{
private:
    Semaphore *semaphore;
public:
    Sem();
    ~Sem();
    Semaphore* MakeSemaphore(int value);
    void SemaphoreWait(Semaphore *sem);
    void SemaphoreSignal(Semaphore *sem);

};

#endif






