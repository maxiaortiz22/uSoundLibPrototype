#pragma once
#include <vector>
#include <algorithm>
#include "AudioNode.h"

class ChannelRouterNode : public AudioNode {
public:
    ChannelRouterNode(MonoSourceNode* in, std::vector<float> gainsPerChannel)
        : input(in), gains(std::move(gainsPerChannel)) {}

    void setGainForChannel(int ch, float g) {
        if (ch >= 0 && ch < (int)gains.size()) gains[ch] = g;
    }

    void prepare(int sr, int blockSize) override {
        input->prepare(sr, blockSize);
        scratch.resize(blockSize);
    }

    void setOutputChannels(int n) {
        outChannels = n;
        if ((int)gains.size() != outChannels) gains.assign(outChannels, 0.0f);
    }

    void process(AudioBuffer& out) override {
        MonoBuffer mono{ scratch.data(), out.numFrames };
        input->process(mono);

        std::fill(out.data, out.data + out.numFrames * out.numChannels, 0.0f);
        for (int f = 0; f < out.numFrames; ++f) {
            float v = mono.data[f];
            for (int ch = 0; ch < outChannels; ++ch)
                out.data[f * out.numChannels + ch] += v * gains[ch];
        }
    }
private:
    MonoSourceNode*     input;
    std::vector<float>  gains;
    std::vector<float>  scratch;
    int                 outChannels = 2;
};