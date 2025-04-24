#pragma once
#include "vulkan/renderer/Mesh.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <optional>
#include <array>
#include <vector>
#include <iostream>
#include <set>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/Vertex.hpp"
#include <memory>

#include "vulkan/core/Instance.hpp"
#include "vulkan/VulkanInitializer.hpp"
#include "vulkan/core/PhysicalDevice.hpp"
#include "vulkan/core/LogicalDevice.hpp"
#include "vulkan/core/Swapchain.hpp"
#include "vulkan/pipeline/ImageViews.hpp"
#include "vulkan/pipeline/RenderPass.hpp"
#include "vulkan/pipeline/PipelineBuilder.hpp"
#include "vulkan/pipeline/FrameBuffers.hpp"
#include "vulkan/commands/CommandPool.hpp"
#include "vulkan/commands/CommandBuffers.hpp"
#include "vulkan/sync/SyncObjects.hpp"
#include "vulkan/renderer/DrawFrame.hpp"
#include "vulkan/resources/DepthResources.hpp"

#include "vulkan/renderer/Scene.hpp"
#include "vulkan/commands/UniformBuffer.hpp"


class Scene;

class Mesh;

class DrawFrame;

class VulkanApp
{
    friend class InstanceCreator;
    friend class VulkanInitializer;
    friend class PhysicalDevice;
    friend class LogicalDevice;
    friend class Swapchain;
    friend class ImageViews;
    friend class RenderPass;
    friend class PipelineBuilder;
    friend class FrameBuffers;
    friend class CommandPool;
    friend class VertexBuffer;
    friend class CommandBuffers;
    friend class SyncObjects;
    friend class DrawFrame;
    friend class Mesh;
    friend class DepthResources;


   

public:
    void run(uint32_t screenWidth, uint32_t screenHeight);

    double lastX = 0.0;
    double lastY = 0.0;
    bool firstMouse = true;
    float yaw = 0.0f;
    float pitch = 0.0f;


    VkDevice device = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkCommandPool commandPool;
    VkQueue graphicsQueue = VK_NULL_HANDLE;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void createDescriptorPool();
    void createDescriptorSet(VkBuffer uniformBuffer, VkImageView imageView, VkSampler sampler);

    UniformBuffer ubo;

private:   
    GLFWwindow *window = nullptr;
    VkInstance instance;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout;

  
    VkQueue presentQueue = VK_NULL_HANDLE;

    VkSwapchainKHR swapchain;
    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    VkRenderPass renderPass;

    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapchainFramebuffers;

    
    std::vector<VkCommandBuffer> commandBuffers;

    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    

    DrawFrame *drawFrameHandler = nullptr;
    
    std::unique_ptr<Scene> scene;

    void initWindow(uint32_t width, uint32_t height);
    void initVulkan();
    void mainLoop();
    void cleanup();
};