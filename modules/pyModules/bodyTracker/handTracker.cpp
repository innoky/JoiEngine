#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <vector>
#include <tuple>
#include <iostream>

namespace py = pybind11;

std::vector<std::tuple<float, float, float>> get_hand_landmarks()
{
    try
    {
        // Добавляем путь к .py-модулю
        py::module_ sys = py::module_::import("sys");
        sys.attr("path").attr("append")("../modules/pyModules/bodyTracker");

        // Импортируем модуль
        py::module_ tracker = py::module_::import("hand_tracker");
        py::object result = tracker.attr("get_hand_landmarks")();

        // Проверка типа
        if (!py::isinstance<py::list>(result))
        {
            std::cerr << "[PYBIND] Python вернул не список\n";
            return {};
        }

        return result.cast<std::vector<std::tuple<float, float, float>>>();
    }
    catch (const py::error_already_set &e)
    {
        std::cerr << "[PYTHON ERROR] " << e.what() << "\n";
        return {};
    }
}
