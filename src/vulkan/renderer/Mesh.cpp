#include "vulkan/renderer/Mesh.hpp"
#include "vulkan/VulkanApp.hpp"
#include <cstring>
#include <stdexcept>
#include <iostream>

Mesh::Mesh(VulkanApp &app,
           const void *vertexData, size_t vertexSize, uint32_t vertexCount,
           const std::string &name)
    : Mesh(app, vertexData, vertexSize, vertexCount, nullptr, 0, 0, name) {}

Mesh::Mesh(VulkanApp &app,
           const void *vertexData, size_t vertexSize, uint32_t vertexCount,
           const void *indexData, size_t indexSize, uint32_t indexCount,
           const std::string &name)
    : app(app), name(name),
      vertexCount_(vertexCount), vertexSize_(vertexSize),
      indexCount_(indexCount), indexSize_(indexSize)
{
    // === Vertex Buffer ===
    VkDeviceSize vbSize = vertexSize * vertexCount;

    VkBufferCreateInfo vbInfo{};
    vbInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vbInfo.size = vbSize;
    vbInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    vbInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(app.device, &vbInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to create vertex buffer");

    VkMemoryRequirements vbMemReq;
    vkGetBufferMemoryRequirements(app.device, vertexBuffer, &vbMemReq);

    VkMemoryAllocateInfo vbAllocInfo{};
    vbAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vbAllocInfo.allocationSize = vbMemReq.size;
    vbAllocInfo.memoryTypeIndex = findMemoryType(vbMemReq.memoryTypeBits,
                                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(app.device, &vbAllocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate vertex buffer memory");

    vkBindBufferMemory(app.device, vertexBuffer, vertexBufferMemory, 0);

    void *vData;
    vkMapMemory(app.device, vertexBufferMemory, 0, vbSize, 0, &vData);
    memcpy(vData, vertexData, vbSize);
    vkUnmapMemory(app.device, vertexBufferMemory);

    std::cout << "[MESH] Vertex buffer created. Size: " << vbSize << " bytes\n";

    // === Index Buffer ===
    if (indexData && indexCount > 0)
    {
        VkDeviceSize ibSize = indexSize * indexCount;

        VkBufferCreateInfo ibInfo{};
        ibInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        ibInfo.size = ibSize;
        ibInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        ibInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(app.device, &ibInfo, nullptr, &indexBuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to create index buffer");

        VkMemoryRequirements ibMemReq;
        vkGetBufferMemoryRequirements(app.device, indexBuffer, &ibMemReq);

        VkMemoryAllocateInfo ibAllocInfo{};
        ibAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        ibAllocInfo.allocationSize = ibMemReq.size;
        ibAllocInfo.memoryTypeIndex = findMemoryType(ibMemReq.memoryTypeBits,
                                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(app.device, &ibAllocInfo, nullptr, &indexBufferMemory) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate index buffer memory");

        vkBindBufferMemory(app.device, indexBuffer, indexBufferMemory, 0);

        void *iData;
        vkMapMemory(app.device, indexBufferMemory, 0, ibSize, 0, &iData);
        memcpy(iData, indexData, ibSize);
        vkUnmapMemory(app.device, indexBufferMemory);

        std::cout << "[MESH] Index buffer created. Size: " << ibSize << " bytes\n";
    }
}

Mesh::~Mesh()
{
    cleanup();
}

void Mesh::cleanup()
{
    if (indexBuffer != VK_NULL_HANDLE)
        vkDestroyBuffer(app.device, indexBuffer, nullptr);
    if (indexBufferMemory != VK_NULL_HANDLE)
        vkFreeMemory(app.device, indexBufferMemory, nullptr);

    if (vertexBuffer != VK_NULL_HANDLE)
        vkDestroyBuffer(app.device, vertexBuffer, nullptr);
    if (vertexBufferMemory != VK_NULL_HANDLE)
        vkFreeMemory(app.device, vertexBufferMemory, nullptr);
}

void Mesh::draw(VkCommandBuffer cmd) const
{
    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmd, 0, 1, vertexBuffers, offsets);

    if (texture && descriptorSet != VK_NULL_HANDLE)
    {
        vkCmdBindDescriptorSets(
            cmd,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            app.pipelineLayout,
            0, 1, &descriptorSet, 0, nullptr);
    }

    if (indexCount_ > 0)
    {
        vkCmdBindIndexBuffer(cmd, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(cmd, indexCount_, 1, 0, 0, 0);
    }
    else
    {
        std::cout << "[GRID_DRAW] drawing " << name << " with " << vertexCount_ << " verts\n";

        vkCmdDraw(cmd, vertexCount_, 1, 0, 0);
    }
}

void Mesh::setTexture(std::shared_ptr<Texture2D> tex)
{
    texture = tex;
}

uint32_t Mesh::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags props)
{
    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceMemoryProperties(app.physicalDevice, &memProps);

    for (uint32_t i = 0; i < memProps.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProps.memoryTypes[i].propertyFlags & props) == props)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}
