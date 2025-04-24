#include "vulkan/core/Surface.hpp"
#include <stdexcept> 
#include <iostream> 

void Surface::create(VkInstance instance, GLFWwindow *window)
{
    if (surface != VK_NULL_HANDLE)
    {
        // уже создан — удалим старый, чтобы не утечь
        vkDestroySurfaceKHR(instance, surface, nullptr);
        surface = VK_NULL_HANDLE;
    }

    VkResult res = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    if (res != VK_SUCCESS || surface == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Failed to create GLFW Vulkan surface");
    }

    // Для отладки можно вывести:
    // std::cout << "[VULKAN] Surface created: " << surface << "\n";
}

void Surface::destroy(VkInstance instance)
{
    if (surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        surface = VK_NULL_HANDLE;
    }
}

VkSurfaceKHR Surface::get() const
{
    return surface;
}
