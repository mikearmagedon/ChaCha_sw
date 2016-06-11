#ifndef CONSUMER_H
#define CONSUMER_H

#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include "pthrd.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <errno.h>
#include "defines.h"

#ifndef    QCONS_NAME
#define    QCONS_NAME    "/queue_cons"
#endif

using namespace std;

class Consumer: public Pthrd
{
private:
    char * filename;
    buff_message msg_cons;
    mqd_t queue_cons;
    int msg_con_int;
    struct mq_attr attr;
    int ID;
    char queue_name[20];
public:
    Consumer();//Constructor
    ~Consumer();
    void set_ID(int);
    void set_filename(char *);
    void run(void);
};

#endif // CONSUMER_H
