#include "chacha20block.h"

Chacha20Block::Chacha20Block(const uint8_t key[32], const uint8_t nonce[8]){
    const uint8_t *magic_constant = (uint8_t*)"expand 32-byte k";
    state[ 0] = pack4(magic_constant + 0*4);
    state[ 1] = pack4(magic_constant + 1*4);
    state[ 2] = pack4(magic_constant + 2*4);
    state[ 3] = pack4(magic_constant + 3*4);
    state[ 4] = pack4(key + 0*4);
    state[ 5] = pack4(key + 1*4);
    state[ 6] = pack4(key + 2*4);
    state[ 7] = pack4(key + 3*4);
    state[ 8] = pack4(key + 4*4);
    state[ 9] = pack4(key + 5*4);
    state[10] = pack4(key + 6*4);
    state[11] = pack4(key + 7*4);
    // 64 bit counter initialized to zero by default.
    state[12] = 0;
    state[13] = 0;
    state[14] = pack4(nonce + 0*4);
    state[15] = pack4(nonce + 1*4);
}

uint32_t Chacha20Block::rotl32(uint32_t x, int n){
    return (x << n) | (x >> (32 - n));
}

uint32_t Chacha20Block::pack4(const uint8_t *a){
    return
        uint32_t(a[0] << 0*8) |
        uint32_t(a[1] << 1*8) |
        uint32_t(a[2] << 2*8) |
        uint32_t(a[3] << 3*8);
}

void Chacha20Block::unpack4(uint32_t src, uint8_t *dst){
    dst[0] = (src >> 0*8) & 0xff;
    dst[1] = (src >> 1*8) & 0xff;
    dst[2] = (src >> 2*8) & 0xff;
    dst[3] = (src >> 3*8) & 0xff;
}

void Chacha20Block::set_counter(uint64_t counter){
    // Want to process many blocks in parallel?
    // No problem! Just set the counter to the block you want to process.
    state[12] = uint32_t(counter);
    state[13] = counter >> 32;
}

void Chacha20Block::next(uint32_t result[16]){
    // This is where the crazy voodoo magic happens.
    // Mix the bytes a lot and hope that nobody finds out how to undo it.
    for (int i = 0; i < 16; i++) result[i] = state[i];

#define CHACHA20_QUARTERROUND(x, a, b, c, d) \
x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a], 16); \
x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c], 12); \
x[a] += x[b]; x[d] = rotl32(x[d] ^ x[a], 8); \
x[c] += x[d]; x[b] = rotl32(x[b] ^ x[c], 7);

    for (int i = 0; i < 10; i++){
        CHACHA20_QUARTERROUND(result, 0, 4, 8, 12)
        CHACHA20_QUARTERROUND(result, 1, 5, 9, 13)
        CHACHA20_QUARTERROUND(result, 2, 6, 10, 14)
        CHACHA20_QUARTERROUND(result, 3, 7, 11, 15)
        CHACHA20_QUARTERROUND(result, 0, 5, 10, 15)
        CHACHA20_QUARTERROUND(result, 1, 6, 11, 12)
        CHACHA20_QUARTERROUND(result, 2, 7, 8, 13)
        CHACHA20_QUARTERROUND(result, 3, 4, 9, 14)
    }

    for (int i = 0; i < 16; i++) result[i] += state[i];

    uint32_t *counter = state + 12;
    // increment counter
    counter[0]++;
    if (0 == counter[0]){
        // wrap around occured, increment higher 32 bits of counter
        counter[1]++;
        // Limited to 2^64 blocks of 64 bytes each.
        // If you want to process more than 1180591620717411303424 bytes
        // you have other problems.
        // We could keep counting with counter[2] and counter[3] (nonce),
        // but then we risk reusing the nonce which is very bad.
        assert(0 != counter[1]);
    }
}
