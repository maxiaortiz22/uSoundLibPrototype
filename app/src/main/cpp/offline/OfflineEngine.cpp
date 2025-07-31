#include "offline/OfflineEngine.h"

OfflineEngine::OfflineEngine(int sr, int ch, int bs)
    : m_sampleRate(sr), m_channels(ch), m_blockSize(bs) {
    buildGraph();
}

void OfflineEngine::buildGraph() {
    m_sine   = std::make_unique<SineNode>(440.f);
    m_noise  = std::make_unique<NoiseNode>();

    m_sineGain  = std::make_unique<MonoGainNode>(m_sine.get(), 0.5f);
    m_noiseGain = std::make_unique<MonoGainNode>(m_noise.get(), 0.1f);

    m_sineRouter  = std::make_unique<ChannelRouterNode>(m_sineGain.get(),  std::vector<float>{1.f, 1.f});
    m_noiseRouter = std::make_unique<ChannelRouterNode>(m_noiseGain.get(), std::vector<float>{1.f, 0.f});
    m_sineRouter->setOutputChannels(m_channels);
    m_noiseRouter->setOutputChannels(m_channels);

    m_mixer  = std::make_unique<MixerNode>();
    m_mixer->setOutputChannels(m_channels);
    m_mixer->addInput(m_sineRouter.get(), 1.f);
    m_mixer->addInput(m_noiseRouter.get(), 1.f);

    m_output = std::make_unique<OutputNode>(m_mixer.get());
    m_output->prepare(m_sampleRate, m_blockSize);

    m_buffer.resize(m_blockSize * m_channels);
}

// ===== setters =====
void OfflineEngine::setSineFreq(float f)  { m_sine->setFrequency(f); }
void OfflineEngine::setSineGain(float g)  { m_sineGain->setGain(g);  }
void OfflineEngine::setNoiseGain(float g) { m_noiseGain->setGain(g); }
void OfflineEngine::setSineChannelGain(int ch,float g){ m_sineRouter->setGainForChannel(ch,g);}  
void OfflineEngine::setNoiseChannelGain(int ch,float g){ m_noiseRouter->setGainForChannel(ch,g);}  

// ===== process =====
std::vector<float> OfflineEngine::process(int numFrames) {
    std::vector<float> out(numFrames * m_channels);
    int remaining = numFrames;
    int offset = 0;
    while (remaining > 0) {
        int frames = std::min(remaining, m_blockSize);
        AudioBuffer buf{ m_buffer.data(), frames, m_channels };
        m_output->process(buf);
        std::copy(buf.data, buf.data + frames * m_channels, out.data() + offset);
        remaining -= frames;
        offset    += frames * m_channels;
    }
    return out;
}