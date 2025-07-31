#include "AudioEngine.h"
#include <android/log.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  "AudioEngine", __VA_ARGS__)

bool AudioEngine::open(int sampleRate, int ch, int framesPerCallback) {
    m_sampleRate = sampleRate; m_channels = ch; m_blockSize = framesPerCallback;

    buildGraph();

    oboe::AudioStreamBuilder builder;
    builder.setDirection(oboe::Direction::Output)
           .setPerformanceMode(oboe::PerformanceMode::LowLatency)
           .setSharingMode(oboe::SharingMode::Exclusive)
           .setChannelCount(ch)
           .setSampleRate(sampleRate)
           .setFormat(oboe::AudioFormat::Float)
           .setFramesPerCallback(framesPerCallback)
           .setCallback(this);

    oboe::Result result = builder.openStream(m_stream);
    if (result != oboe::Result::OK) {
        LOGE("Failed to open stream: %s", oboe::convertToText(result));
        return false;
    }
    m_stream->requestStart();
    return true;
}

void AudioEngine::close() {
    if (m_stream) {
        m_stream->stop();
        m_stream->close();
        m_stream.reset();
    }
}

void AudioEngine::buildGraph() {
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

void AudioEngine::applyPendingCommands() {
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

oboe::DataCallbackResult AudioEngine::onAudioReady(
        oboe::AudioStream* stream, void* audioData, int32_t numFrames) {

    applyPendingCommands();

    AudioBuffer buf{ static_cast<float*>(audioData), numFrames, stream->getChannelCount() };
    m_output->process(buf);

    return oboe::DataCallbackResult::Continue;
}