#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <tuple>

namespace py = pybind11;

std::vector<std::tuple<float, float, float>> get_hand_landmarks()
{
    py::object tracker = py::module_::import("hand_tracker");
    py::object result = tracker.attr("get_hand_landmarks")();
    return result.cast<std::vector<std::tuple<float, float, float>>>();
}
