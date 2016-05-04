#include "pthrd.h"

Pthrd::Pthrd()
{

}



void Pthrd::CheckFail(int status)
{
    if(status)
    {
        if(status==EPERM)
            std::cerr << "pthread_create() got EPERM\n";
        else if(status==EINVAL)
            std::cerr << "pthread_create() got EINVAL\n";
        else
            std::cerr << "pthread_create() got neither EPERM not EINVAL\n";

        std::cerr << "pthread_create() got error " << status << std::endl;
        errno=status;
        std::cerr << "pthread_create()" << std::endl;
        exit(1);
    }
}


int Pthrd::Start(int prio)
{
    pthread_attr_init(&thread_attr);
    pthread_attr_getschedpolicy(&thread_attr, &thread_policy);
    pthread_attr_getschedparam(&thread_attr, &thread_param);

    this->SetupThread(prio, &thread_attr, &thread_param);

    int rtn = pthread_create(&pthread,&thread_attr,Pthrd_Func,static_cast<void*>(this));

    std::cout << "\nPriority: " << thread_param.__sched_priority << std::endl;
    CheckFail(rtn);
    //pthread_detach(pthread);
    return rtn;
}


void Pthrd::SetupThread(int prio, pthread_attr_t *pthread_attr, struct sched_param * pthread_param)
{

//    int rr_min_priority, rr_max_priority;

//    //Step 2: Retirve max and min priority
//    rr_min_priority = sched_get_priority_min(SCHED_RR);
//    rr_max_priority = sched_get_priority_max(SCHED_RR);

//    std::cout << "MAX: " << rr_max_priority << " MIN: " << rr_min_priority << std::endl;

    // Step 3: Calculate and/or assign priority value to sched_param structure
    pthread_param-> sched_priority = prio;

    //Step 4: set scheduling policy
    pthread_attr_setschedpolicy (pthread_attr,SCHED_RR);

    //Step 5: set scheduling param
    pthread_attr_setschedparam(pthread_attr,pthread_param);

    //Step 6: set scheduling attributes to be determined by attributes object
    pthread_attr_setinheritsched(pthread_attr, PTHREAD_EXPLICIT_SCHED);


}

void* Pthrd::Pthrd_Func(void *p)
{

    Pthrd *pthrd = static_cast<Pthrd*>(p);
    pthrd->run();
    return 0;
}

