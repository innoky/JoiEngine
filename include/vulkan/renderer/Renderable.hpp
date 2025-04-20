#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

class VulkanApp;

class Renderable
{
public:
    virtual void update(float deltaTime) = 0;
    virtual void recordDrawCommands(VkCommandBuffer cmd) = 0;
    virtual ~Renderable() = default;
};
