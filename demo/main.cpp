#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <tuple>
#include <windows.h>

#include "vulkan/VulkanApp.hpp"
#include <pybind11/embed.h>
namespace py = pybind11;

// Запускает встроенный Python и вызывает нашу функцию из hand_tracker_bind.pyd
// void runHandTracking()
// {
//     // Указываем, что корень CPython — это текущая папка с JoyApp.exe
//     Py_SetPythonHome(L".");
//     std::cout << "[INFO] Before interpreter init\n";

//     // Инициализируем интерпретатор
//     py::scoped_interpreter guard{};
//     std::cout << "[INFO] Python interpreter initialized\n";

//     // Выводим sys.path до поправки (должно содержать ".", "python310.zip" и т.д.)
//     auto sys = py::module::import("sys");
//     std::cout << "[INFO] sys.path before fix:\n";
//     for (auto entry : sys.attr("path"))
//         std::cout << "  > " << std::string(py::str(entry)) << "\n";

//     // Добавляем в путь папку site-packages вашего venv-а, чтобы Python увидел cv2, mediapipe и т.п.
//     sys.attr("path").attr("append")("D:\\MezeraEngine\\MezeraEngine\\venv\\Lib\\site-packages");
//     std::cout << "[INFO] sys.path after fix:\n";
//     for (auto entry : sys.attr("path"))
//         std::cout << "  > " << std::string(py::str(entry)) << "\n";

//     // Импортируем наш pybind11-модуль (.pyd)
//     std::cout << "[INFO] Importing hand_tracker_bind...\n";
//     auto hand = py::module::import("hand_tracker_bind");
//     std::cout << "[INFO] Module imported successfully\n";

//     // Теперь в цикле каждые 1 с читаем hand landmarks и печатаем
//     while (true)
//     {
//         auto landmarks = hand.attr("get_hand_landmarks")()
//                              .cast<std::vector<std::tuple<float, float, float>>>();

//         std::cout << "== Hand landmarks ==\n";
//         for (auto &pt : landmarks)
//         {
//             float x, y, z;
//             std::tie(x, y, z) = pt;
//             std::cout << "x: " << x << "; y: " << y << "; z: " << z << "\n";
//         }

//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
// }

int main()
{
    // Чтобы вывод в консоли корректно показывал UTF-8
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "Выберите режим:\n"
                 "1 - Запустить Vulkan\n"
                 "2 - Запустить Hand Tracking\n"
                 "> ";

    int choice;
    std::cin >> choice;

    if (choice == 1)
    {

        try
        {
            uint32_t width = 3840;
            uint32_t height = 2160;
            VulkanApp VA;
            VA.run(width, height);
        }
        catch (const std::exception &e)
        {
            std::cerr << "[CRASH] " << e.what() << "\n";
        }
    }
    else if (choice == 2)
    {
        // runHandTracking();
        std::cout << "Заглушка";
    }
    else
    {
        std::cout << "Неверный выбор.\n";
    }

    return 0;
}
