#include "AudioEngineWin.h"
#include <iostream>

bool AudioEngineWin::open(int sampleRate, int ch, int framesPerCallback) {
    m_sampleRate = sampleRate; 
    m_channels = ch; 
    m_blockSize = framesPerCallback;

    buildGraph();
    
    std::cout << "AudioEngine opened - SR: " << sampleRate 
              << ", Channels: " << ch 
              << ", Block: " << framesPerCallback << std::endl;
    
    return true;
}

void AudioEngineWin::close() {
    std::cout << "AudioEngine closed" << std::endl;
}

void AudioEngineWin::buildGraph() {
    // Fuentes mono
    m_sine   = std::make_unique<SineNode>(440.f);
    m_noise  = std::make_unique<NoiseNode>();

    // Ganancias individuales (mono)
    m_sineGain  = std::make_unique<MonoGainNode>(m_sine.get(), 0.5f);
    m_noiseGain = std::make_unique<MonoGainNode>(m_noise.get(), 0.1f);

    // Routers por canal (2 canales)
    m_sineRouter  = std::make_unique<ChannelRouterNode>(m_sineGain.get(),  std::vector<float>{1.f, 1.f});
    m_noiseRouter = std::make_unique<ChannelRouterNode>(m_noiseGain.get(), std::vector<float>{1.f, 0.f});
    m_sineRouter->setOutputChannels(m_channels);
    m_noiseRouter->setOutputChannels(m_channels);

    // Mixer final (estéreo)
    m_mixer  = std::make_unique<MixerNode>();
    m_mixer->setOutputChannels(m_channels);
    m_mixer->addInput(m_sineRouter.get(), 1.f);
    m_mixer->addInput(m_noiseRouter.get(), 1.f);

    m_output = std::make_unique<OutputNode>(m_mixer.get());
    m_output->prepare(m_sampleRate, m_blockSize);
}

void AudioEngineWin::applyPendingCommands() {
    Command c;
    while (m_cmdQueue.pop(c)) {
        switch (c.type) {
            case Command::SetSineFreq:  m_sine->setFrequency(c.value);          break;
            case Command::SetSineGain:  m_sineGain->setGain(c.value);           break;
            case Command::SetNoiseGain: m_noiseGain->setGain(c.value);          break;
            case Command::SetSineL:     m_sineRouter->setGainForChannel(0, c.value); break;
            case Command::SetSineR:     m_sineRouter->setGainForChannel(1, c.value); break;
            case Command::SetNoiseL:    m_noiseRouter->setGainForChannel(0, c.value); break;
            case Command::SetNoiseR:    m_noiseRouter->setGainForChannel(1, c.value); break;
        }
    }
}

void AudioEngineWin::processAudio(float* audioData, int32_t numFrames, int channels) {
    applyPendingCommands();
    
    AudioBuffer buf{ audioData, numFrames, channels };
    m_output->process(buf);
}