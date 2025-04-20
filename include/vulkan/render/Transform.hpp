#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

struct Transform
{
    glm::vec3 position{0.0f};
    glm::vec3 rotationEuler{0.0f}; 
    glm::vec3 scale{1.0f};
    
    float rotationAccumulator = 0.0f;

    glm::mat4 getMatrix() const
    {
        glm::mat4 t = glm::translate(glm::mat4(1.0f), position);

        glm::mat4 r = glm::yawPitchRoll(
            glm::radians(rotationEuler.y),  // yaw — Y
            glm::radians(rotationEuler.x),  // pitch — X
            glm::radians(rotationEuler.z)); // roll — Z

        glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);

        return t * r * s;
    }
};
