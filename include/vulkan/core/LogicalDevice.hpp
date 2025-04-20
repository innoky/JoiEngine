//Logical device creation

#pragma once
#include <vulkan/resources/QueueFamilyIndices.hpp>
#include <vulkan/VulkanApp.hpp>

class LogicalDevice
{
public:
    LogicalDevice(VulkanApp &app);
    void createLogicalDevice();

private:
    VulkanApp &app;
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};