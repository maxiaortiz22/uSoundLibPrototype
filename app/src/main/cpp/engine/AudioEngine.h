#pragma once
#include <memory>
#include <oboe/Oboe.h>
#include "CommandQueue.h"
#include "graph/OutputNode.h"
#include "graph/SineNode.h"
#include "graph/NoiseNode.h"
#include "graph/MonoGainNode.h"
#include "graph/ChannelRouterNode.h"
#include "graph/MixerNode.h"

struct Command {
    enum Type {
        SetSineFreq,
        SetSineGain,
        SetNoiseGain,
        SetSineL, SetSineR,
        SetNoiseL, SetNoiseR
    } type;
    float value;
};

class AudioEngine : public oboe::AudioStreamCallback {
public:
    AudioEngine() = default;
    ~AudioEngine() { close(); }

    bool open(int sampleRate = 48000, int ch = 2, int framesPerCallback = 192);
    void close();

    void enqueue(const Command& c) { m_cmdQueue.push(c); }

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream* stream,
                                          void* audioData,
                                          int32_t numFrames) override;
private:
    void applyPendingCommands();
    void buildGraph();

    // Nodos
    std::unique_ptr<SineNode>          m_sine;
    std::unique_ptr<NoiseNode>         m_noise;
    std::unique_ptr<MonoGainNode>      m_sineGain;
    std::unique_ptr<MonoGainNode>      m_noiseGain;
    std::unique_ptr<ChannelRouterNode> m_sineRouter;
    std::unique_ptr<ChannelRouterNode> m_noiseRouter;
    std::unique_ptr<MixerNode>         m_mixer;
    std::unique_ptr<OutputNode>        m_output;

    std::shared_ptr<oboe::AudioStream> m_stream;
    int m_sampleRate = 48000;
    int m_channels   = 2;
    int m_blockSize  = 192;

    SPSCQueue<Command, 128> m_cmdQueue;
};