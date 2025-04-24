#include "vulkan/renderer/Scene.hpp"
#include "vulkan/render/PushConstants.hpp"
#include "vulkan/render/GridVertex.hpp"
#include "vulkan/renderer/Texture2D.hpp"
#include "vulkan/data/SimulationMeshes/Lighthing/LightingData.hpp"

#include <algorithm>
#include <memory>



Scene::Scene(GLFWwindow *window, VulkanApp &app) : window(window), app(app) {}
void Scene::init()
{
    app.ubo.create(app);

    auto texture = std::make_shared<Texture2D>(app, "../../resources/textures/basic/whiteStriped.png");


    std::vector<GridVertex> gridVertices;
    const int count = 20;
    const float size = 1.0f;
    const float half = (count * size) / 2.0f;

    for (int i = 0; i <= count; ++i)
    {
        float pos = -half + i * size;
        // вертикальные линии
        gridVertices.push_back({glm::vec3(pos, 0.0f, -half)});
        gridVertices.push_back({glm::vec3(pos, 0.0f, half)});
        // горизонтальные линии
        gridVertices.push_back({glm::vec3(-half, 0.0f, pos)});
        gridVertices.push_back({glm::vec3(half, 0.0f, pos)});
    }
    gridMesh = std::make_unique<Mesh>(app, gridVertices.data(), sizeof(GridVertex), static_cast<uint32_t>(gridVertices.size()), "grid");

    lightning = std::make_unique<LightningSimulator>();
    std::cout << "[GRID] gridVertices.size(): " << gridVertices.size() << std::endl;
}
void Scene::draw(VkCommandBuffer cmd, VkPipelineLayout layout)
{
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = camera.getProjectionMatrix();
    proj[1][1] *= -1; // Vulkan NDC: инвертировать Y

    // 1. Рисуем сетку
    if (gridMesh)
    {
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, app.gridPipeline);
        if (app.gridPipeline == VK_NULL_HANDLE)
        {
            std::cerr << "[GRID] gridPipeline is NULL\n";
        }
        PushConstantData pcData{};
        pcData.model = gridMesh->transform.getMatrix();
        pcData.view = view;
        pcData.proj = proj;

        vkCmdPushConstants(cmd, app.gridPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstantData), &pcData);
        std::cout << "[DRAW] Drawing grid mesh with vertex count: " << gridMesh->vertexCount() << "\n";

        gridMesh->draw(cmd);
    }

    // 2. Рисуем все меши
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
    camera.updateDirectionFromAngles(); 

    if (lightning)
        lightning->update(*this, deltaTime);
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

