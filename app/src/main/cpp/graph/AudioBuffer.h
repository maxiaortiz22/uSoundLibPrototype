#pragma once
#include <cstddef>

struct AudioBuffer {
    float* data;        // interleaved
    int    numFrames;
    int    numChannels; // usado por nodos multicanal (Router/Mixer/Output)
};

struct MonoBuffer {
    float* data;
    int    numFrames;
};