cmake_minimum_required(VERSION 3.22)
project(audioengine)

set(CMAKE_CXX_STANDARD 17)
add_library(audioengine SHARED
    engine/AudioEngine.cpp
    jni/NativeBridge.cpp
    )

# Headers de grafo
target_include_directories(audioengine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Añadir todos los .h/.cpp del grafo si los separás
file(GLOB GRAPH_SRC graph/*.h graph/*.cpp graph/**/*.h graph/**/*.cpp)
foreach(f ${GRAPH_SRC})
    # header-only en este ejemplo, pero si hay .cpp, añadirlos
endforeach()

# Oboe
add_subdirectory(${CMAKE_SOURCE_DIR}/oboe ./oboe-build)
target_link_libraries(audioengine oboe::oboe log)