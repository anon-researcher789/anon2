#pragma once
#include <vector>
#include <cstdint>
#include <cstring>
#include <string>
#include "randomness.h"

class Transcript {
public:
    explicit Transcript(const std::string& domain);

    // Absorb arbitrary bytes
    void absorb_bytes(const uint8_t* data, size_t len);

    // Absorb typed objects
    void absorb_u64(uint64_t x);
    void absorb_i32(int32_t x);

    // Absorb commitment / matrix (raw memory)
    template<typename T>
    void absorb_object(const T& obj) {
        absorb_bytes(reinterpret_cast<const uint8_t*>(&obj), sizeof(T));
    }

    // Produce challenges
    int32_t challenge_i32(const std::string& label);
    Randomness challenge_randomness(const std::string& label);

private:
    std::vector<uint8_t> state;

    void absorb_label(const std::string& label);
};
