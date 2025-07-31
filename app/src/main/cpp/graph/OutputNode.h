#pragma once
#include "AudioNode.h"

class OutputNode : public AudioNode {
public:
    explicit OutputNode(AudioNode* in) : input(in) {}
    void prepare(int sr, int blockSize) override { input->prepare(sr, blockSize); }
    void process(AudioBuffer& out) override { input->process(out); }
private:
    AudioNode* input;
};