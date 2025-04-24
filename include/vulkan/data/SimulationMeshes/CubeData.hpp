#pragma once

#include "vulkan/render/Vertex.hpp"
#include <vector>
#include <glm/glm.hpp>

inline std::pair<std::vector<Vertex>, std::vector<uint32_t>> GenerateCube(float size = 1.0f)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float h = size / 2.0f;

    std::vector<glm::vec3> positions = {
        {-h, -h, -h}, {h, -h, -h}, {h, h, -h}, {-h, h, -h}, 
        {-h, -h, h},
        {h, -h, h},
        {h, h, h},
        {-h, h, h},
    };

    for (const auto &pos : positions)
        vertices.push_back(Vertex{pos, glm::vec3(1.0f), glm::vec2(0.0f)});


    indices = {
        0, 1, 2, 2, 3, 0, 
        4, 5, 6, 6, 7, 4,
        0, 4, 7, 7, 3, 0, 
        1, 5, 6, 6, 2, 1, 
        3, 2, 6, 6, 7, 3, 
        0, 1, 5, 5, 4, 0  
    };

    return {vertices, indices};
}
