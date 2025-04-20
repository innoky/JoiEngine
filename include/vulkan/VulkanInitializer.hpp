#pragma once

#include "vulkan/VulkanApp.hpp"


class VulkanInitializer
{
public:
    VulkanInitializer(VulkanApp &app);
    void init();

    void createDepthResources();

private : 
    VulkanApp &app;
    VkFormat findDepthFormat();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};
