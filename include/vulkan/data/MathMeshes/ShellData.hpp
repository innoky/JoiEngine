#pragma once

#include "vulkan/render/Vertex.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <utility>
inline std::pair<std::vector<Vertex>, std::vector<uint32_t>> GenerateShellSurface(
    float alpha, float beta, int uSegments, int vSegments)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float uStart = 0.0f;
    float uEnd = 2.0f * glm::pi<float>();
    float vStart = 0.0f;
    float vEnd = 2.0f * glm::pi<float>();

    float du = (uEnd - uStart) / static_cast<float>(uSegments);
    float dv = (vEnd - vStart) / static_cast<float>(vSegments);

    for (int i = 0; i <= uSegments; ++i)
    {
        float u = uStart + i * du;
        for (int j = 0; j <= vSegments; ++j)
        {
            float e = 2.7182;
            float v = vStart + j * dv;

            float x = alpha * pow(e, (beta * v)) * cos(v) * (1 + cos(u));
            float y = alpha * pow(e, (beta * v)) * sin(v) * (1 + cos(u));
            float z = alpha * pow(e, beta * v) * sin(u);

            glm::vec3 pos = glm::vec3(x/2, y/2, z/2);
            
            if (i % 5 == 0 || j % 5 == 0)
            {
                glm::vec3 color = glm::vec3(
                    0.0f,
                    0.0f,
                    0.0f);
                vertices.push_back({pos, color});
            }
            else
            {
                glm::vec3 color = glm::vec3(
                    0.75f,
                    0.75f,
                    0.75f);
                vertices.push_back({pos, color});
            }
        }
    }

    for (int i = 0; i < uSegments; ++i)
    {
        for (int j = 0; j < vSegments; ++j)
        {
            uint32_t row1 = i * (vSegments + 1);
            uint32_t row2 = (i + 1) * (vSegments + 1);

            indices.push_back(row1 + j);
            indices.push_back(row2 + j);
            indices.push_back(row2 + j + 1);

            indices.push_back(row1 + j);
            indices.push_back(row2 + j + 1);
            indices.push_back(row1 + j + 1);
        }
    }

    return {vertices, indices};
}
