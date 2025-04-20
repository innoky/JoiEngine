// Command list for GPU

#pragma once
#include <vulkan/VulkanApp.hpp>


class CommandBuffers
{
public:
    CommandBuffers(VulkanApp &app);
    void createCommandBuffers();

private:
    VulkanApp &app;
};