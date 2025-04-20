//Physical device creation

#pragma once
#include <vulkan/VulkanApp.hpp>
#include <vulkan/resources/QueueFamilyIndices.hpp>

class PhysicalDevice
{
public:
    PhysicalDevice(VulkanApp &app);
    void pickPhysicalDevice();
    

private:
    VulkanApp &app;
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};