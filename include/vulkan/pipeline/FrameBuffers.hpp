#pragma once
#include <vulkan/VulkanApp.hpp>

class FrameBuffers
{
public:
    FrameBuffers(VulkanApp &app);
    void createFrameBuffers();

private:
    VulkanApp &app;
};