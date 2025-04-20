// Creation of commands poll

#pragma once
#include <vulkan/VulkanApp.hpp>


class CommandPool
{
public:
    CommandPool(VulkanApp &app);
    void createCommandPool();

private:
    VulkanApp &app;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};