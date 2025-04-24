#include "vulkan/data/SimulationMeshes/Lighthing/LightingData.hpp"
#include "vulkan/data/SimulationMeshes/CubeData.hpp"
#include "vulkan/renderer/Mesh.hpp"
#include <memory>
#include <random>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

inline void LightningSimulator::generateFractalLighting(
    const glm::vec3 &start,
    const glm::vec3 &end,
    std::vector<glm::vec3> &outPoints,
    float displacement = 1.8f,
    float minDisplacement = 0.03f,
    float decay = 0.55f,
    int depth = 0)
{
    if (displacement < minDisplacement || depth > 10)
    {
        outPoints.push_back(start);
        outPoints.push_back(end);
        return;
    }

    glm::vec3 midpoint = (start + end) * 0.5f;

    // Добавим случайное смещение
    static std::default_random_engine engine(std::random_device{}());
    std::uniform_real_distribution<float> dist(-displacement, displacement);

    glm::vec3 offset(dist(engine), dist(engine), dist(engine));
    midpoint += offset;

    // Рекурсивно разбиваем отрезок
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

        auto [vertices, indices] = GenerateCube();

        auto texture = std::make_shared<Texture2D>(scene.app, "../../resources/textures/basic/lighting.png");
        auto mesh = std::make_unique<Mesh>(scene.app, vertices, indices, "light_seg_" + std::to_string(createdSegments));

        mesh->setTexture(texture); // это поле можно добавить в Scene
        float x = static_cast<float>(createdSegments);

        if (createdSegments < points.size())
        {
            mesh->transform.position = points[createdSegments];
        }
        else
        {
            std::cerr << "[LIGHTNING] Index out of bounds: " << createdSegments << " >= " << points.size() << "\n";
            return; 
        }

        mesh->transform.scale = glm::vec3(0.02f, 0.02f, 0.02f);
        mesh->create();
        mesh->createDescriptorSet(scene.app.ubo.buffer);
       
        scene.addMesh(std::move(mesh));
        createdSegments++;
    }
}
