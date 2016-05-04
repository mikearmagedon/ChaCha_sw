#ifndef PTHRD_H
#define PTHRD_H
#include <mqueue.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <errno.h>

class Pthrd
{
protected:
    int thread_policy;
    pthread_attr_t thread_attr; // Attribute of Pthread
    struct sched_param thread_param;

private:
    virtual void run() = 0;// run virtual function
    static void* Pthrd_Func(void* );//Pthread function to call run fucntion

protected:
    //Setting up the attributes, the type of scheduler and the priority of pthread
    void SetupThread(int prio, pthread_attr_t *pthread_attr, struct sched_param * pthread_param);
    void CheckFail(int status);
public:
    pthread_t pthread;// object of type Pthread

    int Start(int prio, bool detach = false);//Function to start the thread process
    Pthrd();// Constructor
};

#endif // PTHRD_H
