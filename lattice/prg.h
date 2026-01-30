#pragma once
#include <array>
#include <cstdint>
#include <random>

class PRG {
    std::mt19937_64 rng;

public:
    explicit PRG(const std::array<uint8_t, 32>& seed) {
        uint64_t s = 0;
        for (auto b : seed) s = (s << 1) ^ b;
        rng.seed(s);
    }

    uint32_t next_u32() {
        return rng();
    }
};
