#include "vulkan/renderer/Mesh.hpp"
#include "vulkan/VulkanApp.hpp"
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>

Mesh::Mesh(VulkanApp &app, const std::vector<Vertex> &vertices, std::string name)
    : app(app), vertices(vertices), name(name) {}

Mesh::Mesh(VulkanApp &app, const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, std::string name)
    : app(app), vertices(vertices), indices(indices), name(name) {}

Mesh::~Mesh()
{
    cleanup();
}

void Mesh::create()
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(app.device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS)
        throw std::runtime_error("failed to create vertex buffer!");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(app.device, vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(app.device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate vertex buffer memory!");

    vkBindBufferMemory(app.device, vertexBuffer, vertexBufferMemory, 0);

    void *data;
    vkMapMemory(app.device, vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(app.device, vertexBufferMemory);

    std::cout << "[MESH] Vertex buffer created. Vertex count: " << vertices.size() << "\n";

    // === INDEX BUFFER (optional) ===
    if (!indices.empty())
    {
        VkDeviceSize indexBufferSize = sizeof(indices[0]) * indices.size();

        VkBufferCreateInfo indexBufferInfo{};
        indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        indexBufferInfo.size = indexBufferSize;
        indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        indexBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(app.device, &indexBufferInfo, nullptr, &indexBuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to create index buffer!");

        VkMemoryRequirements indexMemRequirements;
        vkGetBufferMemoryRequirements(app.device, indexBuffer, &indexMemRequirements);

        VkMemoryAllocateInfo indexAllocInfo{};
        indexAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        indexAllocInfo.allocationSize = indexMemRequirements.size;
        indexAllocInfo.memoryTypeIndex = findMemoryType(
            indexMemRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(app.device, &indexAllocInfo, nullptr, &indexBufferMemory) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate index buffer memory!");

        vkBindBufferMemory(app.device, indexBuffer, indexBufferMemory, 0);

        void *indexData;
        vkMapMemory(app.device, indexBufferMemory, 0, indexBufferSize, 0, &indexData);
        memcpy(indexData, indices.data(), (size_t)indexBufferSize);
        vkUnmapMemory(app.device, indexBufferMemory);

        std::cout << "[MESH] Index buffer created. Index count: " << indices.size() << "\n";
    }

        
        glm::vec3 center3D{0.0f, 0.0f, 0.0f};
        for (const auto &vertex : vertices)
        center3D += glm::vec3(vertex.pos.x, vertex.pos.y, 0.0f);
        center3D /= static_cast<float>(vertices.size());
        localCenter = center3D;
}
void Mesh::draw(VkCommandBuffer commandBuffer) const
{
    if (vertexBuffer == VK_NULL_HANDLE)
    {
        std::cerr << "[MESH] vertexBuffer is NULL — not drawing\n";
        return;
    }

    if (descriptorSet == VK_NULL_HANDLE)
    {
        std::cerr << "[MESH] descriptorSet is NULL — SEGFAULT PREVENTED\n";
        return;
    }

    if (!texture)
    {
        std::cerr << "[MESH] texture is nullptr — SEGFAULT PREVENTED\n";
        return;
    }

    if (texture->getImageView() == VK_NULL_HANDLE)
    {
        std::cerr << "[MESH] texture image view is NULL — draw skipped\n";
        return;
    }

    if (texture->getSampler() == VK_NULL_HANDLE)
    {
        std::cerr << "[MESH] texture sampler is NULL — draw skipped\n";
        return;
    }

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        app.pipelineLayout,
        0, 1, &descriptorSet,
        0, nullptr);

    if (!indices.empty())
    {
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    }
    else
    {
        vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
    }
}

void Mesh::cleanup()
{
    if (indexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(app.device, indexBuffer, nullptr);
        indexBuffer = VK_NULL_HANDLE;
    }

    if (indexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(app.device, indexBufferMemory, nullptr);
        indexBufferMemory = VK_NULL_HANDLE;
    }

    if (vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(app.device, vertexBuffer, nullptr);
        vertexBuffer = VK_NULL_HANDLE;
    }

    if (vertexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(app.device, vertexBufferMemory, nullptr);
        vertexBufferMemory = VK_NULL_HANDLE;
    }
}

uint32_t Mesh::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(app.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void Mesh::setTexture(std::shared_ptr<Texture2D> tex)
{
    texture = tex;
    std::cout << "[MESH] Texture set\n";
}

void Mesh::createDescriptorSet(VkBuffer uboBuffer)
{
    std::cout << "[MESH] → Begin createDescriptorSet\n";

    if (!texture)
    {
        std::cerr << "[MESH] ERROR: texture is nullptr in createDescriptorSet\n";
        return;
    }

    std::cout << "[MESH] ImageView: " << texture->getImageView() << "\n";
    std::cout << "[MESH] Sampler: " << texture->getSampler() << "\n";

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = app.descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &app.descriptorSetLayout;
    if (app.descriptorPool == VK_NULL_HANDLE)
    {
        std::cerr << "[MESH] ERROR: descriptorPool == VK_NULL_HANDLE\n";
    }
    if (app.descriptorSetLayout == VK_NULL_HANDLE)
    {
        std::cerr << "[MESH] ERROR: descriptorSetLayout == VK_NULL_HANDLE\n";
    }
    if (vkAllocateDescriptorSets(app.device, &allocInfo, &descriptorSet) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate descriptor set!");

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uboBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(glm::mat4) * 3;

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texture->getImageView();
    imageInfo.sampler = texture->getSampler();

    std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptorSet;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = descriptorSet;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;
    
    if (uboBuffer == VK_NULL_HANDLE)
    {
        std::cerr << "[MESH] ERROR: uboBuffer is VK_NULL_HANDLE — cannot bind!\n";
        return;
    }
    std::cout << "[MESH] Final sanity check before update:\n";
    std::cout << "  ↳ descriptorSet: " << descriptorSet << "\n";
    std::cout << "  ↳ uboBuffer:     " << bufferInfo.buffer << "\n";
    std::cout << "  ↳ imageView:     " << imageInfo.imageView << "\n";
    std::cout << "  ↳ sampler:       " << imageInfo.sampler << "\n";

    vkUpdateDescriptorSets(app.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    std::cout << "[MESH] Descriptor set created: " << descriptorSet << std::endl;
    
}
