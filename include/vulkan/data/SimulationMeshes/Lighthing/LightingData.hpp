#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Scene;
class Mesh;

class LightningSimulator
{
public:
    void update(Scene &scene, float dt);

private:
    std::vector<glm::vec3> points;
    float elapsed = 0.0f;
    int createdSegments = 0;
    const int maxSegments = 256;
    const float spawnInterval = 0.02f;

    void generateFractalLighting(
        const glm::vec3 &start,
        const glm::vec3 &end,
        std::vector<glm::vec3> &outPoints,
        float displacement = 1.8f,
        float minDisplacement = 0.03f,
        float decay = 0.55f,
        int depth = 0);
};
