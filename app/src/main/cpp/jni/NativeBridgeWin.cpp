#include "engine/AudioEngineWin.h"
#include <iostream>

static AudioEngineWin gEngine;

// Funciones C++ normales (sin JNI)
extern "C" {
    bool nativeStartEngine(int sr, int ch, int fpb) {
        return gEngine.open(sr, ch, fpb);
    }

    void nativeStopEngine() {
        gEngine.close();
    }

    // ==== Parámetros Sine ====
    void nativeSetSineFreq(float f) {
        gEngine.enqueue({Command::SetSineFreq, f});
    }
    
    void nativeSetSineGain(float g) {
        gEngine.enqueue({Command::SetSineGain, g});
    }
    
    void nativeSetSineLeft(float g) {
        gEngine.enqueue({Command::SetSineL, g});
    }
    
    void nativeSetSineRight(float g) {
        gEngine.enqueue({Command::SetSineR, g});
    }

    // ==== Parámetros Noise ====
    void nativeSetNoiseGain(float g) {
        gEngine.enqueue({Command::SetNoiseGain, g});
    }
    
    void nativeSetNoiseLeft(float g) {
        gEngine.enqueue({Command::SetNoiseL, g});
    }
    
    void nativeSetNoiseRight(float g) {
        gEngine.enqueue({Command::SetNoiseR, g});
    }
}