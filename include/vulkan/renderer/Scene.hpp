#pragma once

#include "vulkan/renderer/Mesh.hpp"
#include "vulkan/renderer/Camera.hpp"
#include "vulkan/VulkanApp.hpp"
#include "vulkan/render/Vertex.hpp"
#include "vulkan/render/Transform.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

class VulkanApp;
class Mesh;


class Scene
{
public:
    Scene(GLFWwindow *window, VulkanApp &app);
    void update(float time);
    void draw(VkCommandBuffer cmd, VkPipelineLayout layout);

    void addMesh(std::unique_ptr<Mesh> mesh);
    std::vector<std::unique_ptr<Mesh>> &getMeshes();

    void setAspect(float aspect); 
    Camera &getCamera();   
    void cleanup();

    Camera camera;
    GLFWwindow *window;
    

    void init();

    float lastFrame = 0.0f;
    float scrollOffset = 0.0f;

    bool isDragging = false;
    double lastX = 0.0f, lastY = 0.0f;
    


private:
    std::vector<std::unique_ptr<Mesh>> meshes;
    VulkanApp &app;
    
};
