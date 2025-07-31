#pragma once
#include "AudioBuffer.h"

class AudioNode {
public:
    virtual ~AudioNode() = default;
    virtual void prepare(int sampleRate, int blockSize) {}
    virtual void process(AudioBuffer& out) = 0; // multicanal
};

class MonoSourceNode {
public:
    virtual ~MonoSourceNode() = default;
    virtual void prepare(int sampleRate, int blockSize) {}
    virtual void process(MonoBuffer& out) = 0; // siempre mono
};