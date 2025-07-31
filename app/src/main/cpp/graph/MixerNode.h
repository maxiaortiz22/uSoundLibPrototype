#pragma once
#include <vector>
#include <memory>
#include "AudioNode.h"

class MixerNode : public AudioNode {
public:
    struct Input { AudioNode* node; float gain; };

    void addInput(AudioNode* n, float g = 1.f) { inputs.push_back({n, g}); }

    void prepare(int sr, int blockSize) override {
        for (auto& in : inputs) in.node->prepare(sr, blockSize);
        scratch.resize(blockSize * outChannels);
    }

    void setOutputChannels(int n){ outChannels = n; }

    void process(AudioBuffer& out) override {
        std::fill(out.data, out.data + out.numFrames * out.numChannels, 0.0f);
        AudioBuffer temp{ scratch.data(), out.numFrames, out.numChannels };
        for (auto& in : inputs) {
            in.node->process(temp);
            for (int i = 0; i < out.numFrames * out.numChannels; ++i)
                out.data[i] += temp.data[i] * in.gain;
        }
    }
private:
    std::vector<Input> inputs;
    std::vector<float> scratch;
    int outChannels = 2;
};