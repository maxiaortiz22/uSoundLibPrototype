#pragma once
#include <atomic>
#include <cstddef>

// Agregar las definiciones que faltan
enum class CommandType {
    SetSineFreq,
    SetSineGain,
    SetSineL,
    SetSineR,
    SetNoiseGain,
    SetNoiseL,
    SetNoiseR
};

struct Command {
    CommandType type;
    float value;
};

template <typename T, size_t Capacity>
class SPSCQueue {
public:
    bool push(const T& v) {
        size_t head = m_head.load(std::memory_order_relaxed);
        size_t next = (head + 1) % Capacity;
        if (next == m_tail.load(std::memory_order_acquire)) return false; // full
        m_buffer[head] = v;
        m_head.store(next, std::memory_order_release);
        return true;
    }
    bool pop(T& out) {
        size_t tail = m_tail.load(std::memory_order_relaxed);
        if (tail == m_head.load(std::memory_order_acquire)) return false; // empty
        out = m_buffer[tail];
        m_tail.store((tail + 1) % Capacity, std::memory_order_release);
        return true;
    }
private:
    T m_buffer[Capacity];
    std::atomic<size_t> m_head{0};
    std::atomic<size_t> m_tail{0};
};

// Alias para usar en AudioEngine
using CommandQueue = SPSCQueue<Command, 256>;