#pragma once

#include <vulkan/VulkanApp.hpp>

class ImageViews
{
public:
    ImageViews(VulkanApp &app);
    void createImageViews();

private:
    VulkanApp &app;
};