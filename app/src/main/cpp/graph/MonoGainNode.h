#pragma once
#include <atomic>
#include "AudioNode.h"

class MonoGainNode : public MonoSourceNode {
public:
    explicit MonoGainNode(MonoSourceNode* in, float g = 1.f) : input(in), gain(g) {}
    void setGain(float g) { gain.store(g, std::memory_order_relaxed); }
    void prepare(int sr, int bs) override { input->prepare(sr, bs); }

    void process(MonoBuffer& out) override {
        input->process(out);
        const float g = gain.load(std::memory_order_relaxed);
        for (int i = 0; i < out.numFrames; ++i) out.data[i] *= g;
    }
private:
    MonoSourceNode*    input;
    std::atomic<float> gain{1.0f};
};