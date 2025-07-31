#pragma once
#include <atomic>
#include <cmath>
#include "AudioNode.h"

class SineNode : public MonoSourceNode {
public:
    explicit SineNode(float f = 440.f) : freq(f) {}
    void setFrequency(float f) { freq.store(f, std::memory_order_relaxed); }

    void prepare(int sr, int /*blockSize*/) override { sampleRate = sr; }

    void process(MonoBuffer& out) override {
        const float f = freq.load(std::memory_order_relaxed);
        const float inc = twoPi * f / sampleRate;
        for (int i = 0; i < out.numFrames; ++i) {
            phase += inc;
            if (phase >= twoPi) phase -= twoPi;
            out.data[i] = std::sin(phase);
        }
    }
private:
    std::atomic<float> freq{440.f};
    float phase = 0.f;
    int   sampleRate = 48000;
    static constexpr float twoPi = 6.283185307179586f;
};