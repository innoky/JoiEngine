#include "vulkan/renderer/Camera.hpp"
#include <glm/gtc/constants.hpp>

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
    glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
    proj[1][1] *= -1; // for Vulkan orientration
    return proj;
}

void Camera::moveX(float deltaTime)
{
    float speed = 1.0f;
    glm::vec3 forward = glm::normalize(target - position);
    glm::vec3 cordX = glm::normalize(glm::cross(forward, up));
    glm::vec3 velocity = cordX * speed;
    position += velocity * deltaTime;
    target += velocity * deltaTime;
}

void Camera::moveY(float deltaTime)
{
    float speed = 1.0f; 
    glm::vec3 cordY = glm::normalize(up);
    glm::vec3 velocity = cordY * speed;
    position += velocity * deltaTime;
    target += velocity * deltaTime;
}

void Camera::moveZ(float deltaTime)
{
    float speed = 1.0f;
    glm::vec3 direction = glm::normalize(target - position);
    glm::vec3 velocity = direction * speed;
    position += velocity * deltaTime;
    target += velocity * deltaTime;
}

void Camera::updateDirectionFromAngles()
{
    glm::vec3 dir;
    dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    dir.y = sin(glm::radians(pitch));
    dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    target = position + glm::normalize(dir);
}
