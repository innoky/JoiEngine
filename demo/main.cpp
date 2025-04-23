#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <tuple>

#include "vulkan/VulkanApp.hpp"

#include <pybind11/embed.h>

namespace py = pybind11;

void runHandTracking()
{
    py::scoped_interpreter guard{}; // Инициализация интерпретатора Python

    try
    {
        py::module handTracker = py::module::import("hand_tracker_bind");

        while (true)
        {
            auto result = handTracker.attr("get_hand_landmarks")().cast<std::vector<std::tuple<float, float, float>>>();

            std::cout << "== Hand landmarks ==\n";
            for (const auto &[x, y, z] : result)
            {
                std::cout << "x: " << x << ", y: " << y << ", z: " << z << '\n';
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (py::error_already_set &e)
    {
        std::cerr << "[Python error] " << e.what() << std::endl;
    }
}

int main()
{
    std::cout << "Выберите режим:\n";
    std::cout << "1 - Запустить Vulkan\n";
    std::cout << "2 - Запустить Hand Tracking\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    if (choice == 1)
    {
        VulkanApp VA;
        VA.run();
    }
    else if (choice == 2)
    {
        runHandTracking();
    }
    else
    {
        std::cout << "Неверный выбор.\n";
    }

    return 0;
}
