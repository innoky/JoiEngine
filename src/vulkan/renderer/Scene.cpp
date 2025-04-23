#include "vulkan/renderer/Scene.hpp"
#include "vulkan/render/PushConstants.hpp"

// Objects data
#include "vulkan/data/CubeData.hpp"
#include "vulkan/data/MobiusData.hpp"
#include "vulkan/data/SpiralData.hpp"
#include "vulkan/data/ToroidalData.hpp"
#include "vulkan/data/ScrewData.hpp"
#include "vulkan/data/ShellData.hpp"
#include "vulkan/data/KleynData.hpp"
#include "vulkan/data/ModelGenerator.hpp"

#include <algorithm>
#include <memory>



Scene::Scene(GLFWwindow *window, VulkanApp &app) : window(window), app(app) {}

void Scene::init()
{
    app.ubo.create(app);
    // auto cube = std::make_unique<Mesh>(app, cubeVertices, "fcube");
    // cube->create();
    // addMesh(std::move(cube));

    // auto texture = std::make_shared<Texture2D>(app, "../resources/textures/blackrat_eyes_baseColor.png");

    // float alpha_mobius = 0.5f;
    // float beta_mobius = 0.2f;

    // int uSegments = 150;
    // int vSegments = 150;
    auto [vertices, indices] = LoadGLTFMesh_All("../resources/gltfModels/scene.gltf");
    auto car = std::make_unique<Mesh>(app, vertices, indices, "rat");
    // car->setTexture(texture);
    car->create();
    // car->transform.scale.x = 10.0f;
    // car->transform.scale.y = 10.0f;
    // car->transform.scale.z = 10.0f;
    // car->transform.rotationEuler.z = 180;
    car->createDescriptorSet(app.ubo.buffer);
    addMesh(std::move(car));
    
}

void Scene::draw(VkCommandBuffer cmd, VkPipelineLayout layout)
{
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = camera.getProjectionMatrix();
    proj[1][1] *= -1; // for vulkan orientation

    for (auto &mesh : meshes)
    {
        glm::mat4 model = mesh->transform.getMatrix();

        PushConstantData pcData{};
        pcData.model = model;
        pcData.view = view;
        pcData.proj = proj;

        vkCmdPushConstants(cmd, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantData), &pcData);
        mesh->draw(cmd);
    }
}

void Scene::update(float time)
{
    float currentFrame = static_cast<float>(glfwGetTime());
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float cam_speed = 2.0f;

    if (!window)
    {
        std::cerr << "[ERROR] Scene::update(): window is nullptr\n";
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveZ(+(deltaTime * cam_speed));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveZ(-(deltaTime * cam_speed));

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveX(+(deltaTime * cam_speed));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveX(-(deltaTime * cam_speed));

    if (scrollOffset != 0.0f)
    {
        // std::cout << "[SCROLL] Scroll: " << scrollOffset << "\n";
        camera.moveZ(scrollOffset * 0.1f);
        scrollOffset = 0.0f;
    }

    if (isDragging)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        float xoffset = static_cast<float>(xpos - lastX);
        float yoffset = static_cast<float>(lastY - ypos); 

        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.09f; 
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.yaw -= xoffset;
        camera.pitch += yoffset;

        if (camera.pitch > 89.0f)
            camera.pitch = 89.0f;
        if (camera.pitch < -89.0f)
            camera.pitch = -89.0f;
    }
    camera.updateDirectionFromAngles(); // каждый кадр

    for (auto &mesh : meshes)
    {
        // mesh->transform.rotationEuler.y += 0.5;
        // mesh->transform.rotationEuler.x += 0.5;
    }
}

void Scene::addMesh(std::unique_ptr<Mesh> mesh)
{
    meshes.push_back(std::move(mesh));
}

void Scene::cleanup()
{
    for (auto &mesh : meshes)
        mesh->cleanup();
}

