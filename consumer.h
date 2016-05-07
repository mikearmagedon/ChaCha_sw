#ifndef CONSUMER_H
#define CONSUMER_H

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

struct buff_message_cons
{
    uint8_t bytes[65];
};

class Consumer: public Pthrd
{
private:
    const char * filename;
    buff_message_cons msg_cons;
    mqd_t queue_cons;
    int msg_con_int;
    struct mq_attr attr;
    unsigned int sender;
public:
    Consumer(const char *);//Constructor
    ~Consumer();
    void run(void);
    void save_file(const char *, const buff_message_cons &);
};

#endif // CONSUMER_H
