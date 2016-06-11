#ifndef DEFINES
#define DEFINES

#define DEBUG 0
#define DEBUG_C 0
#define DEBUG_P 0
#define DEBUG_PRO 0

//Annotations
#define NO_OF_THREADS 2
#define ROUNDS 20
#define QUEUE_SIZE 50
#define SCHEDULER SCHED_RR
#define INHERITSCHED PTHREAD_EXPLICIT_SCHED
#define FILENAME file
#define PRIORITY 1
#define KEY 1, 2, 3, 4, 5, 6
#define NONCE 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12

//Annotations
//#define NO_OF_THREADS @no_of_threads@
//#define ROUNDS @rounds@
//#define QUEUE_SIZE @queue_size@
//#define SCHEDULER @scheduler@
//#define INHERITSCHED @inheritsched@
//#define FILENAME @filename@
//#define PRIORITY @priority@

#define RTSIGNAL_P (SIGRTMIN + 1)

struct buff_message
{
    uint8_t bytes[65];
    int numb_bytes_read;
    int no_of_blocks;
};

#endif // DEFINES

