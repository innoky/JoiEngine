#include "vulkan/data/SimulationMeshes/Lighthing/LightingData.hpp"
#include "vulkan/renderer/Mesh.hpp"
#include "vulkan/renderer/Scene.hpp"
#include "vulkan/renderer/Texture2D.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <iostream>

inline void LightningSimulator::generateFractalLighting(
    const glm::vec3 &start,
    const glm::vec3 &end,
    std::vector<glm::vec3> &outPoints,
    float displacement,
    float minDisplacement,
    float decay,
    int depth)
{
    if (displacement < minDisplacement || depth > 10)
    {
        outPoints.push_back(start);
        outPoints.push_back(end);
        return;
    }

    glm::vec3 midpoint = (start + end) * 0.5f;
    static std::default_random_engine engine(std::random_device{}());
    std::uniform_real_distribution<float> dist(-displacement, displacement);
    midpoint += glm::vec3(dist(engine), dist(engine), dist(engine));

    generateFractalLighting(start, midpoint, outPoints, displacement * decay, minDisplacement, decay, depth + 1);
    generateFractalLighting(midpoint, end, outPoints, displacement * decay, minDisplacement, decay, depth + 1);
}

void LightningSimulator::update(Scene &scene, float dt)
{
    elapsed += dt;

    if (points.empty())
    {
        generateFractalLighting(glm::vec3(0, 0, 0), glm::vec3(0, -5, 8), points);
    }

    if (createdSegments < maxSegments && elapsed >= spawnInterval)
    {
        elapsed = 0.0f;

        if (createdSegments * 2 + 1 >= points.size())
        {
            std::cerr << "[LIGHTNING] Reached end of points\n";
            return;
        }
        std::vector<Vertex> lineVerts = {
            {points[createdSegments * 2], glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            {points[createdSegments * 2 + 1], glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)}};

        auto mesh = std::make_unique<Mesh>(
            scene.app,
            lineVerts.data(), sizeof(Vertex), static_cast<uint32_t>(lineVerts.size()),
            "light_seg_" + std::to_string(createdSegments));

        mesh->transform.scale = glm::vec3(1.0f); // uniform scale
        scene.addMesh(std::move(mesh));

        createdSegments++;
    }
}
