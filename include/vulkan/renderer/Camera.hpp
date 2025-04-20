#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    glm::vec3 position{0.0f, 0.0f, 5.0f};
    glm::vec3 target{0.0f, 0.0f, 0.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};

    float fov = 60.0f;
    float aspect = 1.2f;
    float nearClip = 0.1f;
    float farClip = 100.0f;

    float yaw = -90.0f; //  horizontal rotate 
    float pitch = 0.0f; // vetical rotate

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void moveZ(float deltaTime);
    void moveX(float deltaTime);
    void moveY(float deltaTime);
    void updateDirectionFromAngles();
};