cmake_minimum_required(VERSION 3.12)

project(AudioEngine)

set(CMAKE_CXX_STANDARD 17)

add_library(
    audioengine
    
    SHARED STATIC
    
    # Engine files
    "../cpp/engine/AudioEngineWin.cpp"
    
    # JNI Bridge
    "../cpp/jni/NativeBridgeWin.cpp"

    # Graph files
    #"../cpp/graph/SineNode.cpp"
    #"../cpp/graph/NoiseNode.cpp"
    #"../cpp/graph/MonoGainNode.cpp"
    #"../cpp/graph/ChannelRouterNode.cpp"
    #"../cpp/graph/MixerNode.cpp"
    #"../cpp/graph/OutputNode.cpp"
)

include_directories(
    # Main cpp directory
    "../cpp/"
    
    # Engine headers
    "../cpp/engine/"
    
    # Graph headers
    "../cpp/graph/"
    
    # JNI headers
    "../cpp/jni/"
)

# Platform-specific libraries for audio (Windows)
if(WIN32)
    target_link_libraries(audioengine)
elseif(UNIX AND NOT APPLE)
    # Linux - you might need ALSA
    # target_link_libraries(audioengine asound)
elseif(APPLE)
    # macOS - you might need Core Audio
    # target_link_libraries(audioengine "-framework CoreAudio")
endif()