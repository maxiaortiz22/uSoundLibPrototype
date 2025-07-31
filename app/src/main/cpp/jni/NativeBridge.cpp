#include <jni.h>
#include "engine/AudioEngine.h"

static AudioEngine gEngine;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_usound_audio_NativeBridge_nativeStartEngine(JNIEnv*, jobject, jint sr, jint ch, jint fpb){
    return gEngine.open(sr, ch, fpb);
}

extern "C" JNIEXPORT void JNICALL
Java_com_usound_audio_NativeBridge_nativeStopEngine(JNIEnv*, jobject){
    gEngine.close();
}

// ==== Parámetros Sine ====
extern "C" JNIEXPORT void JNICALL
Java_com_usound_audio_NativeBridge_nativeSetSineFreq(JNIEnv*, jobject, jfloat f){
    gEngine.enqueue({Command::SetSineFreq, f});
}
extern "C" JNIEXPORT void JNICALL
Java_com_usound_audio_NativeBridge_nativeSetSineGain(JNIEnv*, jobject, jfloat g){
    gEngine.enqueue({Command::SetSineGain, g});
}
extern "C" JNIEXPORT void JNICALL
Java_com_usound_audio_NativeBridge_nativeSetSineLeft(JNIEnv*, jobject, jfloat g){
    gEngine.enqueue({Command::SetSineL, g});
}
extern "C" JNIEXPORT void JNICALL
Java_com_usound_audio_NativeBridge_nativeSetSineRight(JNIEnv*, jobject, jfloat g){
    gEngine.enqueue({Command::SetSineR, g});
}

// ==== Parámetros Noise ====
extern "C" JNIEXPORT void JNICALL
Java_com_usound_audio_NativeBridge_nativeSetNoiseGain(JNIEnv*, jobject, jfloat g){
    gEngine.enqueue({Command::SetNoiseGain, g});
}
extern "C" JNIEXPORT void JNICALL
Java_com_usound_audio_NativeBridge_nativeSetNoiseLeft(JNIEnv*, jobject, jfloat g){
    gEngine.enqueue({Command::SetNoiseL, g});
}
extern "C" JNIEXPORT void JNICALL
Java_com_usound_audio_NativeBridge_nativeSetNoiseRight(JNIEnv*, jobject, jfloat g){
    gEngine.enqueue({Command::SetNoiseR, g});
}