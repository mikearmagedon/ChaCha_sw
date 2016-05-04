#include "chacha20.h"

ChaCha20::ChaCha20(const uint8_t key[32], const uint8_t nonce[8], uint64_t counter)
: block(key, nonce), position(64)
{
    block.set_counter(counter);
}

void ChaCha20::crypt(uint8_t *bytes, size_t n_bytes)
{
    uint8_t *keystream8 = (uint8_t*)keystream32;
    for (size_t i = 0; i < n_bytes; i++){
        if (position >= 64){
            block.next(keystream32);
            position = 0;
        }
        bytes[i] ^= keystream8[position];
        position++;
    }
}
