#pragma once
#include <vulkan/vulkan.h>
#include "vulkan/render/Vertex.hpp"
#include "vulkan/render/Transform.hpp"
#include "vulkan/renderer/Texture2D.hpp"
#include "memory"
#include <vector>
#include <string>

class VulkanApp; 

class Texture2D;

class Mesh
{
public:
    Mesh(VulkanApp &app, const std::vector<Vertex> &vertices, std::string name);
    Mesh(VulkanApp &app, const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, std::string name);

    ~Mesh();

    void create();
    void draw(VkCommandBuffer commandBuffer) const;
    void cleanup();

    std::string name;
    
    glm::vec3 localCenter{0.0f};
    size_t vertexCount() const { return vertices.size(); }
    Transform transform;

    void setTexture(std::shared_ptr<Texture2D> tex);
    void createDescriptorSet(VkBuffer uboBuffer);

private:
    VulkanApp &app;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    

    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    std::shared_ptr<Texture2D> texture = nullptr;
    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
};
