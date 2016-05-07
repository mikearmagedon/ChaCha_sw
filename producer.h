#ifndef PRODUCER_H
#define PRODUCER_H

#include <unistd.h>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include "pthrd.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <errno.h>
#include "chacha20.h"
#include "defines.h"

#ifndef    QPROD_NAME
#define    QPROD_NAME    "/queue_prod"
#endif

using namespace std;

struct buff_message
{
    uint8_t bytes[65];
};


//Class of the thread Producer
class Producer: public Pthrd
{
private:
    const char * filename;
    mqd_t queue_prod;
    buff_message msg_prod;
    struct mq_attr attr;
public:
    Producer(const char *);//Constructor
    ~Producer();
    void run(void);// runnable function
    int load_file(const char *path, buff_message &);
};

#endif // PRODUCER_H
