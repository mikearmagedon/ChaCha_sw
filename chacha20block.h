#ifndef CHACHA20BLOCK_H
#define CHACHA20BLOCK_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "defines.h"

class Chacha20Block
{
public:
    Chacha20Block(const uint8_t [32], const uint8_t [12]);
    void set_counter(uint32_t);
    void next(uint32_t [16]);
private:
    uint32_t state[16];
    uint32_t rotl32(uint32_t, int);
    uint32_t pack4(const uint8_t *);
    void unpack4(uint32_t, uint8_t *);
};

#endif // CHACHA20BLOCK_H
