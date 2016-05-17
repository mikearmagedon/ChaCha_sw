#ifndef PRODUCER_H
#define PRODUCER_H

#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include "pthrd.h"
#include <iostream>
#include <cstring>
#include <math.h>
#include <vector>
#include <fstream>
#include <errno.h>
#include "chacha20.h"
#include "defines.h"

#ifndef    QPROD_NAME
#define    QPROD_NAME    "/queue_prod"
#endif

using namespace std;


//Class of the thread Producer
class Producer: public Pthrd
{
private:
    int ID;
    const char * filename;
    mqd_t queue_prod;
    buff_message msg_prod;
    struct mq_attr attr;
    char queue_name[20];
public:
    Producer();//Constructor
    ~Producer();
    void set_ID(int);
    void set_filename(const char *);
    char get_filename();
    void run(void);// runnable function
};

#endif // PRODUCER_H
