#pragma once
#include <cstdint>
#include "AudioNode.h"

class NoiseNode : public MonoSourceNode {
public:
    void prepare(int /*sr*/, int /*blockSize*/) override {
        if (state == 0) state = 0x12345678u; // seed
    }

    void process(MonoBuffer& out) override {
        for (int i = 0; i < out.numFrames; ++i) {
            out.data[i] = nextSample();
        }
    }
private:
    uint32_t state = 0;
    inline float nextSample() {
        uint32_t x = state;
        x ^= x << 13; x ^= x >> 17; x ^= x << 5;
        state = x;
        return (static_cast<int32_t>(x) / 2147483647.0f); // [-1,1]
    }
};