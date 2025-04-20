#pragma once

#include "vulkan/VulkanApp.hpp"

class DepthResources
{
public:
    DepthResources(VulkanApp &app);
    void createDepthResources();

private:
    VulkanApp &app;
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};
