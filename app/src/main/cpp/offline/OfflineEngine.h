#pragma once
#include <vector>
#include "graph/OutputNode.h"
#include "graph/SineNode.h"
#include "graph/NoiseNode.h"
#include "graph/MonoGainNode.h"
#include "graph/ChannelRouterNode.h"
#include "graph/MixerNode.h"

class OfflineEngine {
public:
    explicit OfflineEngine(int sampleRate = 48000, int channels = 2, int blockSize = 512);

    // === Parámetros ===
    void setSineFreq(float f);
    void setSineGain(float g);
    void setNoiseGain(float g);
    void setSineChannelGain(int ch, float g); // 0=L,1=R
    void setNoiseChannelGain(int ch, float g);

    // Procesa 'numFrames' y devuelve vector interleaved float
    std::vector<float> process(int numFrames);

    int getSampleRate()  const { return m_sampleRate;  }
    int getChannels()    const { return m_channels;    }
    int getBlockSize()   const { return m_blockSize;   }

private:
    void buildGraph();

    int m_sampleRate;
    int m_channels;
    int m_blockSize;

    // nodos
    std::unique_ptr<SineNode>          m_sine;
    std::unique_ptr<NoiseNode>         m_noise;
    std::unique_ptr<MonoGainNode>      m_sineGain;
    std::unique_ptr<MonoGainNode>      m_noiseGain;
    std::unique_ptr<ChannelRouterNode> m_sineRouter;
    std::unique_ptr<ChannelRouterNode> m_noiseRouter;
    std::unique_ptr<MixerNode>         m_mixer;
    std::unique_ptr<OutputNode>        m_output;

    std::vector<float> m_buffer; // internal working buffer
};