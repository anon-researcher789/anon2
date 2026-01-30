#include "transcript.h"
#include <numeric>

static uint64_t cheap_hash(const std::vector<uint8_t>& v) {
    uint64_t h = 0x9e3779b97f4a7c15ULL;
    for (uint8_t b : v) {
        h ^= b + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
    }
    return h;
}

Transcript::Transcript(const std::string& domain) {
    absorb_label(domain);
}

void Transcript::absorb_label(const std::string& label) {
    absorb_bytes(reinterpret_cast<const uint8_t*>(label.data()), label.size());
}

void Transcript::absorb_bytes(const uint8_t* data, size_t len) {
    state.insert(state.end(), data, data + len);
}

void Transcript::absorb_u64(uint64_t x) {
    absorb_bytes(reinterpret_cast<uint8_t*>(&x), sizeof(x));
}

void Transcript::absorb_i32(int32_t x) {
    absorb_bytes(reinterpret_cast<uint8_t*>(&x), sizeof(x));
}

int32_t Transcript::challenge_i32(const std::string& label) {
    absorb_label(label);
    uint64_t h = cheap_hash(state);
    return static_cast<int32_t>(h & 0x7fffffff);
}

Randomness Transcript::challenge_randomness(const std::string& label) {
    absorb_label(label);
    Randomness rho;
    uint64_t h = cheap_hash(state);

    for (size_t i = 0; i < FS_SEED_BYTES; i++) {
        rho[i] = static_cast<uint8_t>((h >> (8 * (i % 8))) & 0xff);
    }
    return rho;
}
