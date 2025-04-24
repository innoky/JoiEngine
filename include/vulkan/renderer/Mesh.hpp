#pragma once
#include <vulkan/vulkan.h>
#include "vulkan/render/Transform.hpp"
#include <memory>
#include <string>

class Texture2D;
class VulkanApp;

class Mesh
{
public:
    // Только вершины
    Mesh(VulkanApp &app,
         const void *vertexData, size_t vertexSize, uint32_t vertexCount,
         const std::string &name);

    // Вершины + индексы
    Mesh(VulkanApp &app,
         const void *vertexData, size_t vertexSize, uint32_t vertexCount,
         const void *indexData, size_t indexSize, uint32_t indexCount,
         const std::string &name);

    ~Mesh();

    void draw(VkCommandBuffer commandBuffer) const;
    void cleanup();

    void setTexture(std::shared_ptr<Texture2D> tex);
    void createDescriptorSet(VkBuffer uboBuffer);

    std::string name;
    glm::vec3 localCenter{0.0f};
    Transform transform;

    size_t vertexCount() const { return vertexCount_; }
    size_t indexCount() const { return indexCount_; }

private:
    VulkanApp &app;

    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;

    uint32_t vertexCount_ = 0;
    size_t vertexSize_ = 0;

    uint32_t indexCount_ = 0;
    size_t indexSize_ = 0;

    std::shared_ptr<Texture2D> texture = nullptr;
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};
