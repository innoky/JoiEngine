#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/VulkanInitializer.hpp"
#include "vulkan/resources/QueueFamilyIndices.hpp"
#include <stdexcept>
#include <vector>
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
#include "vulkan/renderer/Texture2D.hpp"

#include "vulkan/renderer/Mesh.hpp"
#include "vulkan/data/TriangleData.hpp"
#include "vulkan/data/CubeData.hpp"

#include "vulkan/renderer/Camera.hpp"
#include "vulkan/renderer/Scene.hpp"




VulkanInitializer::VulkanInitializer(VulkanApp &app) : app(app) {}

void VulkanInitializer::init()
{
    
    InstanceCreator(app).createInstance();

    if (glfwCreateWindowSurface(app.instance, app.window, nullptr, &app.surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }

    
    PhysicalDevice(app).pickPhysicalDevice();
    LogicalDevice(app).createLogicalDevice();
    Swapchain(app).createSwapchain();
    ImageViews(app).createImageViews();
   

    RenderPass(app).createRenderPass();
    PipelineBuilder(app).createGraphicsPipeline();


    DepthResources(app).createDepthResources();
    FrameBuffers(app).createFrameBuffers();
    CommandPool(app).createCommandPool();
    

    
    SyncObjects(app).createSyncObjects();

    app.createDescriptorPool();

    app.scene = std::make_unique<Scene>(app.window, app);
    
    app.scene->camera.aspect = static_cast<float>(app.swapchainExtent.width) / app.swapchainExtent.height;

    app.drawFrameHandler = new DrawFrame(app);

    
    app.scene->init();

    CommandBuffers(app).createCommandBuffers();
    std::cout << "[INIT] Vulkan instance created\n";
    std::cout << "[INIT] Surface created\n";
    std::cout << "[INIT] Physical device selected\n";
    std::cout << "[INIT] Logical device created\n";
    std::cout << "[INIT] Swapchain created\n";
    std::cout << "[INIT] Image views created\n";
    std::cout << "[INIT] Render pass created\n";
    std::cout << "[INIT] Graphics pipeline created\n";
    std::cout << "[INIT] Framebuffers created\n";
    std::cout << "[INIT] Command pool created\n";
    std::cout << "[INIT] Command buffers allocated\n";
    std::cout << "[INIT] Sync objects created\n";
    std::cout << "[INIT] Mesh created and added to app.meshes\n";

   
    glfwSetWindowUserPointer(app.window, app.scene.get());

    glfwSetScrollCallback(app.window, [](GLFWwindow *win, double /*xoffset*/, double yoffset){
            Scene* scene = static_cast<Scene*>(glfwGetWindowUserPointer(win));
            if (scene)
                scene->scrollOffset = static_cast<float>(yoffset); });
    glfwSetMouseButtonCallback(app.window, [](GLFWwindow *window, int button, int action, int mods)
                               {
    Scene* scene = static_cast<Scene*>(glfwGetWindowUserPointer(window));
    if (!scene) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            scene->isDragging = true;
            glfwGetCursorPos(window, &scene->lastX, &scene->lastY);
        } else if (action == GLFW_RELEASE) {
            scene->isDragging = false;
        }
    } });
    
}

void VulkanInitializer::createDepthResources()
{
    VkFormat depthFormat = findDepthFormat();

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = app.swapchainExtent.width;
    imageInfo.extent.height = app.swapchainExtent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = depthFormat;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(app.device, &imageInfo, nullptr, &app.depthImage) != VK_SUCCESS)
        throw std::runtime_error("failed to create depth image!");

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(app.device, app.depthImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(app.device, &allocInfo, nullptr, &app.depthImageMemory) != VK_SUCCESS)
        throw std::runtime_error("failed to allocate depth image memory!");

    vkBindImageMemory(app.device, app.depthImage, app.depthImageMemory, 0);

    // Создание image view
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = app.depthImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = depthFormat;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(app.device, &viewInfo, nullptr, &app.depthImageView) != VK_SUCCESS)
        throw std::runtime_error("failed to create depth image view!");
}

uint32_t VulkanInitializer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(app.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

VkFormat VulkanInitializer::findDepthFormat()
{
    std::vector<VkFormat> candidates = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT};

    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(app.physicalDevice, format, &props);

        if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported depth format!");
}
