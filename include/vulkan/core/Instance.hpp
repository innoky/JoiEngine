// Instance creation

#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>


class VulkanApp;

class InstanceCreator
{
public:
    InstanceCreator(VulkanApp &app);
    void createInstance();

private:
    VulkanApp &app;
};
