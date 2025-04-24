#pragma once

#include "vulkan/render/Vertex.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <utility>

inline std::pair<std::vector<Vertex>, std::vector<uint32_t>> GenerateSpiralSurface(
    float alpha, float beta, int uSegments, int vSegments)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float uStart = 0.0f;
    float uEnd = 10.0f * glm::pi<float>();
    float vStart = -0.5f;
    float vEnd = 0.5f;

    float du = (uEnd - uStart) / static_cast<float>(uSegments);
    float dv = (vEnd - vStart) / static_cast<float>(vSegments);

    for (int i = 0; i <= uSegments; ++i)
    {
        float u = uStart + i * du;
        for (int j = 0; j <= vSegments; ++j)
        {
            float v = vStart + j * dv;

            float cos_u = cos(u);
            float cos_v = cos(v);
            float sin_u = sin(u);
            float sin_v = sin(v);
            float cos_u2 = cos(u / 2.0f);
            float sin_u2 = sin(u / 2.0f);

            float x = (alpha + beta * cos_v) * cos_u;
            float y = (alpha + beta * cos_v) * sin_u;
            float z = (beta * sin_v) + alpha * u;

            glm::vec3 pos = glm::vec3(x, y, z);
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

    // indices
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
