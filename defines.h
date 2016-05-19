#ifndef DEFINES
#define DEFINES

#define DEBUG 0
#define DEBUG_C 0
#define DEBUG_P 0
#define DEBUG_PRO 0

#define RTSIGNAL_P (SIGRTMIN + 1)

struct buff_message
{
    uint8_t bytes[65];
    int numb_bytes_read;
    int no_of_blocks;
};

#endif // DEFINES

