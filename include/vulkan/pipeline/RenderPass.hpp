#pragma once

#include <vulkan/VulkanApp.hpp>

class RenderPass
{
public:
    RenderPass(VulkanApp &app);
    void createRenderPass();

private:
    VulkanApp &app;
};