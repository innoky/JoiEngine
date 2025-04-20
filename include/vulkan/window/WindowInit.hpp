#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

class WindowInit
{
public:
    static GLFWwindow *create(uint32_t width, uint32_t height, const char *title);
};
