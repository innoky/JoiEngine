#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

class VulkanApp;

class UniformBuffer
{
public:
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;

    void create(VulkanApp &app);
    void update(VulkanApp &app, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj);
    void cleanup(VulkanApp &app);
};
