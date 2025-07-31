package com.usound.audio

enum class Ear { LEFT, RIGHT, BOTH, NONE }

object NativeBridge {
    init { System.loadLibrary("audioengine") }

    external fun nativeStartEngine(sampleRate: Int, channels: Int, framesPerBurst: Int): Boolean
    external fun nativeStopEngine()

    // Sine
    external fun nativeSetSineFreq(freq: Float)
    external fun nativeSetSineGain(gain: Float)
    external fun nativeSetSineLeft(gain: Float)
    external fun nativeSetSineRight(gain: Float)

    // Noise
    external fun nativeSetNoiseGain(gain: Float)
    external fun nativeSetNoiseLeft(gain: Float)
    external fun nativeSetNoiseRight(gain: Float)

    // Helpers de alto nivel
    fun setSineEar(ear: Ear, level: Float = 1f) {
        when (ear) {
            Ear.LEFT  -> { nativeSetSineLeft(level); nativeSetSineRight(0f) }
            Ear.RIGHT -> { nativeSetSineLeft(0f);   nativeSetSineRight(level) }
            Ear.BOTH  -> { nativeSetSineLeft(level); nativeSetSineRight(level) }
            Ear.NONE  -> { nativeSetSineLeft(0f);   nativeSetSineRight(0f) }
        }
    }
    fun setNoiseEar(ear: Ear, level: Float = 1f) {
        when (ear) {
            Ear.LEFT  -> { nativeSetNoiseLeft(level); nativeSetNoiseRight(0f) }
            Ear.RIGHT -> { nativeSetNoiseLeft(0f);   nativeSetNoiseRight(level) }
            Ear.BOTH  -> { nativeSetNoiseLeft(level); nativeSetNoiseRight(level) }
            Ear.NONE  -> { nativeSetNoiseLeft(0f);   nativeSetNoiseRight(0f) }
        }
    }
}