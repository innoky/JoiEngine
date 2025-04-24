#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <tuple>
#include <windows.h>

#include "vulkan/VulkanApp.hpp"

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    std::cout << "Выберите режим:\n"
                 "1 - Запустить Vulkan\n"
                 "2 - Запустить Hand Tracking\n"
                 "> ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
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
        break;
    
    case 2:
        std::cout << "Nothing here";
        break;
    default:
        std::cout << "Incorrect choice";
    } 

    return 0;
}
