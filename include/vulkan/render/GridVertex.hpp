#pragma once
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <array>

struct GridVertex
{
    glm::vec3 pos;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription binding{};
        binding.binding = 0;
        binding.stride = sizeof(GridVertex);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return binding;
    }

    static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 1> attributes{};
        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributes[0].offset = offsetof(GridVertex, pos);
        return attributes;
    }
};
