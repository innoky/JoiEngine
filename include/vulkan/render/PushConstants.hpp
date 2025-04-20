#pragma once
#include <glm/glm.hpp>

struct PushConstantData
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};
