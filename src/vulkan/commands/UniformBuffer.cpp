#include "vulkan/commands/UniformBuffer.hpp"
#include "vulkan/VulkanApp.hpp"
#include "vulkan/utils/VulkanUtils.hpp"
#include <cstring>
#include <stdexcept>

void UniformBuffer::create(VulkanApp &app)
{
    VkDeviceSize bufferSize = sizeof(glm::mat4) * 3; // model, view, proj

    VulkanUtils::createBuffer(
        app,
        bufferSize,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        buffer,
        memory);
}

void UniformBuffer::update(VulkanApp &app,
                           const glm::mat4 &model,
                           const glm::mat4 &view,
                           const glm::mat4 &proj)
{
    glm::mat4 mvp[3] = {model, view, proj};
    void *data;
    vkMapMemory(app.device, memory, 0, sizeof(mvp), 0, &data);
    memcpy(data, &mvp, sizeof(mvp));
    vkUnmapMemory(app.device, memory);
}

void UniformBuffer::cleanup(VulkanApp &app)
{
    if (buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(app.device, buffer, nullptr);
        buffer = VK_NULL_HANDLE;
    }
    if (memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(app.device, memory, nullptr);
        memory = VK_NULL_HANDLE;
    }
}
