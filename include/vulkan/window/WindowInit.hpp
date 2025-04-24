// include/vulkan/window/WindowInit.hpp
#pragma once
#include <GLFW/glfw3.h>

class WindowInit
{
public:
    static GLFWwindow *create(uint32_t width, uint32_t height, const char *title);
};
