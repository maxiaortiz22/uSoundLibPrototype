#pragma once
#include <memory>
#include <vector>
#include "graph/SineNode.h"
#include "graph/NoiseNode.h"
#include "graph/MonoGainNode.h"
#include "graph/ChannelRouterNode.h"
#include "graph/MixerNode.h"
#include "graph/OutputNode.h"
#include "CommandQueue.h"

class AudioEngineWin {
public:
    bool open(int sampleRate, int channels, int framesPerCallback);
    void close();
    void enqueue(const Command& cmd) { m_cmdQueue.push(cmd); }
    
    // Para Windows - procesamiento manual
    void processAudio(float* audioData, int32_t numFrames, int channels);

private:
    void buildGraph();
    void applyPendingCommands();

    int m_sampleRate = 48000;
    int m_channels = 2;
    int m_blockSize = 256;

    // Nodos del grafo
    std::unique_ptr<SineNode> m_sine;
    std::unique_ptr<NoiseNode> m_noise;
    std::unique_ptr<MonoGainNode> m_sineGain, m_noiseGain;
    std::unique_ptr<ChannelRouterNode> m_sineRouter, m_noiseRouter;
    std::unique_ptr<MixerNode> m_mixer;
    std::unique_ptr<OutputNode> m_output;

    CommandQueue m_cmdQueue;
};