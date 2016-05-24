#ifndef PROCESSING_H
#define PROCESSING_H

#include <signal.h>
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

static volatile int work = 0;
static volatile int quit = 0;

class Processing: public Pthrd
{
private:
    static char src_queue_name[20];
    static char dst_queue_name[20];
    mqd_t queue_prod_proc;
    mqd_t queue_cons_proc;
    buff_message msg_pross;
    struct mq_attr attr;
    sigset_t mask;
    siginfo_t info;
    ChaCha20 chacha;

public:
    Processing(const uint8_t [32], const uint8_t [12]);//Constructor
    void run(void);// runnable function
};

#endif // PROCESSING_H
