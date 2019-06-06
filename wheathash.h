/*
    Wheathash takes (optimally) 32-bit inputs and produces a 64-bit hash as its result.
    It is a slightly-edited version of Waterhash, which is an edited version of wyhash.
    It is meant to use very similar code to Waterhash, which produces a 32-bit hash.
    Original Author: Wang Yi <godspeed_china@yeah.net>
    Wheathash Variant Author: Tommy Ettinger <tommy.ettinger@gmail.com>
*/
#ifndef wheathash_version_2
#define wheathash_version_2
#include <stdint.h>
#include <math.h>
const uint64_t _wheatp0 = 0xa0761d6478bd642full, _wheatp1 = 0xe7037ed1a0b428dbull, _wheatp2 = 0x8ebc6af09c88c6e3ull;
const uint64_t _wheatp3 = 0x589965cc75374cc3ull, _wheatp4 = 0x1d8e4e27c47d124full, _wheatp5 = 0xeb44accab455d165ull;

static inline uint64_t _wheatmum(uint64_t A, uint64_t B) {
    uint64_t r = A * B;
    return r - (r >> 32);
}

static inline uint64_t _wheatr08(const uint8_t *p){ uint8_t  v; memcpy(&v, p, 1); return v; }
static inline uint64_t _wheatr16(const uint8_t *p){ uint16_t v; memcpy(&v, p, 2); return v; }
static inline uint64_t _wheatr32(const uint8_t *p){ uint32_t v; memcpy(&v, p, 4); return v; }
static inline uint64_t wheathash(const void* key, uint32_t len, uint64_t seed){
    const uint8_t *p = (const uint8_t*)key;
    uint32_t i;
    for (i = 0; i + 16 <= len; i += 16, p += 16) {
        seed = _wheatmum(
            _wheatmum(_wheatr32(p) ^ _wheatp1, _wheatr32(p + 4) ^ _wheatp2) + seed,
            _wheatmum(_wheatr32(p + 8) ^ _wheatp3, _wheatr32(p + 12) ^ _wheatp4));
    }
    seed += _wheatp5;
    switch (len & 15) {
    case 1:  seed = _wheatmum(_wheatp2 ^ seed, _wheatr08(p) ^ _wheatp1); break;
    case 2:  seed = _wheatmum(_wheatp3 ^ seed, _wheatr16(p) ^ _wheatp4); break;
    case 3:  seed = _wheatmum(_wheatr16(p) ^ seed, _wheatr08(p + 2) ^ _wheatp2); break;
    case 4:  seed = _wheatmum(_wheatr16(p) ^ seed, _wheatr16(p + 2) ^ _wheatp3); break;
    case 5:  seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr08(p + 4) ^ _wheatp1); break;
    case 6:  seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr16(p + 4) ^ _wheatp1); break;
    case 7:  seed = _wheatmum(_wheatr32(p) ^ seed, (_wheatr16(p + 4) << 8 | _wheatr08(p + 6)) ^ _wheatp1); break;
    case 8:  seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr32(p + 4) ^ _wheatp0); break;
    case 9:  seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr32(p + 4) ^ _wheatp2) ^ _wheatmum(seed ^ _wheatp4, _wheatr08(p + 8) ^ _wheatp3); break;
    case 10: seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr32(p + 4) ^ _wheatp2) ^ _wheatmum(seed, _wheatr16(p + 8) ^ _wheatp3); break;
    case 11: seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr32(p + 4) ^ _wheatp2) ^ _wheatmum(seed, ((_wheatr16(p + 8) << 8) | _wheatr08(p + 10)) ^ _wheatp3); break;
    case 12: seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr32(p + 4) ^ _wheatp2) ^ _wheatmum(seed ^ _wheatr32(p + 8), _wheatp4); break;
    case 13: seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr32(p + 4) ^ _wheatp2) ^ _wheatmum(seed ^ _wheatr32(p + 8), (_wheatr08(p + 12)) ^ _wheatp4); break;
    case 14: seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr32(p + 4) ^ _wheatp2) ^ _wheatmum(seed ^ _wheatr32(p + 8), (_wheatr16(p + 12)) ^ _wheatp4); break;
    case 15: seed = _wheatmum(_wheatr32(p) ^ seed, _wheatr32(p + 4) ^ _wheatp2) ^ _wheatmum(seed ^ _wheatr32(p + 8), (_wheatr16(p + 12) << 8 | _wheatr08(p + 14)) ^ _wheatp4); break;
    }
    seed = (seed ^ seed << 16) * (len ^ _wheatp0);
    return seed - (seed >> 31) + (seed << 33);
}
#endif
