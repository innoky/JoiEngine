#pragma once

#include "vulkan/render/Vertex.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <utility>

inline std::pair<std::vector<Vertex>, std::vector<uint32_t>> GenerateKleynovSurface(
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
            float v = vStart + j * dv;

            float delta = 0.1f; 
            float pi = glm::pi<float>();
            float t = glm::smoothstep(pi - delta, pi + delta, u);

            float cos_u = cos(u);
            float sin_u = sin(u);
            float cos_v = cos(v);
            float sin_v = sin(v);
            float cos_half_u = cos(u / 2.0f);
            float sin_half_u = sin(u / 2.0f);
         
            float r = 1.0f;

            
            float x1 = 6 * cos_u * (1 + sin_u) + 6 * r * (1 - (cos_u / 2)) * cos_u * cos_v;
            float y1 = 6 * sin_u * u + 6 * r * (1 - (cos_u / 2)) * sin_u * cos_u;
            float z1 = 6 * r * (1 - (cos_u / 2)) * sin_v;

            
            float x2 = 6 * cos_u * (1 + sin_u) - 4 * r * (1 - (cos_u / 2)) * cos_v;
            float y2 = 16 * sin_u;
            float z2 = 8 * r * (1 - (cos_u / 2)) * sin_v;

         
            float x = glm::mix(x1, x2, t);
            float y = glm::mix(y1, y2, t);
            float z = glm::mix(z1, z2, t);
            glm::vec3 pos = glm::vec3(x/10, y/10, z/10);
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
