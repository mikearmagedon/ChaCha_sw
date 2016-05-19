#ifndef CHACHA20_H
#define CHACHA20_H

#include "chacha20block.h"

class ChaCha20
{
public:
    Chacha20Block block;
    uint32_t keystream32[16];
    size_t position;
    ChaCha20(const uint8_t [32], const uint8_t [12], uint32_t = 0);
    void crypt(uint8_t *, size_t);
};

#endif // CHACHA20_H
