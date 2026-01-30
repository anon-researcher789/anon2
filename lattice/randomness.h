#pragma once
#include <array>
#include <cstdint>

constexpr size_t FS_SEED_BYTES = 32;

struct Randomness {
    std::array<uint8_t, FS_SEED_BYTES> seed;

    // Default: zero
    Randomness() {
        seed.fill(0);
    }

    explicit Randomness(const std::array<uint8_t, FS_SEED_BYTES>& s)
        : seed(s) {}

    // Allow indexing
    uint8_t& operator[](size_t i) {
        return seed[i];
    }

    const uint8_t& operator[](size_t i) const {
        return seed[i];
    }

    // Explicit conversion for PRG / PCS
    const std::array<uint8_t, FS_SEED_BYTES>& bytes() const {
        return seed;
    }
};
