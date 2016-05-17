#ifndef DEFINES
#define DEFINES

#define DEBUG 1

#define RTSIGNAL_P (SIGRTMIN + 1)

struct buff_message
{
    uint8_t bytes[65];
    int numb_bytes_read;
    int no_of_blocks;
};

#endif // DEFINES

