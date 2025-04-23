#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <tuple>

namespace py = pybind11;

// Просто подключаешь реализацию
std::vector<std::tuple<float, float, float>> get_hand_landmarks();

PYBIND11_MODULE(hand_tracker_bind, m)
{
    m.def("get_hand_landmarks", &get_hand_landmarks);
}
