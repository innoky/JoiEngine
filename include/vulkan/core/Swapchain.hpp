//Swapchain creation

#pragma once
#include <vulkan/VulkanApp.hpp>
#include <vulkan/resources/QueueFamilyIndices.hpp>

class Swapchain 
{
public:
    Swapchain(VulkanApp &app);
    void createSwapchain();

private:
    VulkanApp &app;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};