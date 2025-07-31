#include <pybind11/pybind11.h>
#include <pybind11/stl.h>   // para std::vector
#include <pybind11/numpy.h> // para numpy array
#include "offline/OfflineEngine.h"

namespace py = pybind11;

PYBIND11_MODULE(py_audioengine, m) {
    py::class_<OfflineEngine>(m, "OfflineEngine")
        .def(py::init<int,int,int>(),
             py::arg("sample_rate") = 48000,
             py::arg("channels")    = 2,
             py::arg("block_size")  = 512)

        // setters
        .def("set_sine_freq",  &OfflineEngine::setSineFreq)
        .def("set_sine_gain",  &OfflineEngine::setSineGain)
        .def("set_noise_gain", &OfflineEngine::setNoiseGain)
        .def("set_sine_channel_gain",  &OfflineEngine::setSineChannelGain)
        .def("set_noise_channel_gain", &OfflineEngine::setNoiseChannelGain)

        // process
        .def("process", [](OfflineEngine& eng, int frames){
            auto vec = eng.process(frames);
            // devolver numpy array 2‑D (frames, channels)
            return py::array({frames, eng.getChannels()}, vec.data());
        }, py::arg("frames"));
}