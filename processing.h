#ifndef PROCESSING_H
#define PROCESSING_H

#include <unistd.h>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include "pthrd.h"
#include <iostream>
#include <errno.h>
#include <cstring>
#include <vector>
#include <fstream>
#include "chacha20.h"
#include "defines.h"

#ifndef    QPROD_NAME
#define    QPROD_NAME    "/queue_prod"
#endif

#ifndef    QCONS_NAME
#define    QCONS_NAME    "/queue_cons"
#endif

using namespace std;

struct buff_message_processing
{
    uint8_t bytes[65];
};

class Processing: public Pthrd
{
private:
    mqd_t queue_prod_proc;
    mqd_t queue_cons_proc;
    buff_message_processing msg_pross;
    int msg_proc;
    struct mq_attr attr;
    unsigned int sender;

public:
    Processing();//Constructor
    ~Processing();
    void run(void);// runnable function
};

#endif // PROCESSING_H
