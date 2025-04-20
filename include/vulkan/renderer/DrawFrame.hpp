#pragma once

#include <vulkan/VulkanApp.hpp>
#include "vulkan/renderer/Scene.hpp"
class Scene;

class DrawFrame
{
public:
    DrawFrame(VulkanApp &app);
    void drawFrame(Scene *scene);

    private : VulkanApp &app;
};