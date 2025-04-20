#pragma once
#include <vulkan/vulkan.h>
#include "vulkan/render/Vertex.hpp"
#include "vulkan/render/Transform.hpp"
#include <vector>
#include <string>

class VulkanApp; 


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

private:
    VulkanApp &app;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    

    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};
