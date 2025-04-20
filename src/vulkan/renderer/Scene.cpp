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
    // auto cube = std::make_unique<Mesh>(app, cubeVertices, "fcube");
    // cube->create();
    // addMesh(std::move(cube));

    float alpha_toroidal = 0.5f;
    float beta_toroidal = 0.2f;

    float alpha_mobius = 1.1f;
    float beta_mobius = 1.2f;

    float alpha_spiral = 1.1f;
    float beta_spiral = 1.2f;

    float alpha_shell = 0.5f;
    float beta_shell = 0.2f;

    float alpha_kleyan = 0.5f;
    float beta_kleyan = 0.2f;

    int uSegments = 150;
    int vSegments = 150;

    // auto [vertices, indices] = LoadGLTFMesh_All("../resources/gltfModels/scene.gltf");
    // auto car = std::make_unique<Mesh>(app, vertices, indices, "cat");
    // car->create();
    // car->transform.rotationEuler.z = 180.0f;
    // car->transform.scale.x = 0.1f;
    // car->transform.scale.y = 0.1f;
    // car->transform.scale.z = 0.1f;
    // car->transform.rotationEuler.z = 90.0f;
    // car->transform.rotationEuler.x = -270.0f;
    // addMesh(std::move(car));
    
    auto [vertices_mobius, indices_mobius] = GenerateMobiusSurface(alpha_mobius, beta_mobius, uSegments, vSegments);
    auto [vertices_toroidal, indices_toroidal] = GenerateToroidalSurface(alpha_toroidal, beta_toroidal, uSegments, vSegments);
    auto [vertices_spiral, indices_spiral] = GenerateSpiralSurface(alpha_spiral, beta_spiral, uSegments, vSegments);
    auto [vertices_shell, indices_shell] = GenerateShellSurface(alpha_shell, beta_shell, uSegments, vSegments);
    auto [vertices_kleyn, indices_kleyn] = GenerateKleynovSurface(alpha_kleyan, beta_kleyan, uSegments, vSegments);

    auto mobius = std::make_unique<Mesh>(app, vertices_mobius, indices_mobius, "mobius");
    auto toroidal = std::make_unique<Mesh>(app, vertices_toroidal, indices_toroidal, "toroid");
    auto spiral = std::make_unique<Mesh>(app, vertices_spiral, indices_spiral, "spiral");
    auto shell = std::make_unique<Mesh>(app, vertices_shell, indices_shell, "shell");
    auto kleyn = std::make_unique<Mesh>(app, vertices_kleyn, indices_kleyn, "klyen");

    mobius->create();
    toroidal->create();
    spiral->create();
    shell->create();
    kleyn->create();

    toroidal->transform.position.x = 3.0f;
    spiral->transform.position.x = -6.0f;
    shell->transform.position.x = 10.0f;
    kleyn->transform.position.x = 15.0f;
   

    addMesh(std::move(mobius));
    addMesh(std::move(toroidal));
    addMesh(std::move(spiral));
    addMesh(std::move(shell));
    addMesh(std::move(kleyn));
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

    // for (auto &mesh : meshes)
    // {
        
    // }
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

