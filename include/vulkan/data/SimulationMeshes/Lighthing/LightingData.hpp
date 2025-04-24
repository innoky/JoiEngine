#pragma once
#include "vulkan/renderer/Scene.hpp"

class Scene;


class LightningSimulator
{
public:
    void update(Scene &scene, float dt);
    
    void generateFractalLighting(
         const glm::vec3& start,
    const glm::vec3& end,
    std::vector<glm::vec3>& outPoints,
    float displacement,
    float minDisplacement,
    float decay,
    int depth
    );

    std::vector<glm::vec3> points;
private:
    float elapsed = 0.0f;
    int createdSegments = 0;
    float spawnInterval = 0.0001f; 
    int maxSegments = 10000;
};
