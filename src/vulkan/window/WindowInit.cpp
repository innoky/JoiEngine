#include <vulkan/window/WindowInit.hpp>
#include <stdexcept>
#include <iostream>

GLFWwindow *WindowInit::create(uint32_t width, uint32_t height, const char *title)
{
    if (!glfwInit())
        throw std::runtime_error("GLFW initialization failed");

    // Проверим поддержку Vulkan
    if (!glfwVulkanSupported())
        throw std::runtime_error("GLFW: Vulkan not supported!");

    // Отключаем создание OpenGL контекста
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window)
        throw std::runtime_error("GLFW window creation failed");

    return window;
}
