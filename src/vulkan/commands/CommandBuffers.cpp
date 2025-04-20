#include <vulkan/commands/CommandBuffers.hpp>
#include <vulkan/render/PushConstants.hpp>

CommandBuffers::CommandBuffers(VulkanApp &app) : app(app) {}

void CommandBuffers::createCommandBuffers()
{
    if (app.commandPool == VK_NULL_HANDLE)
    {
        std::cerr << "[CRITICAL] Command pool is VK_NULL_HANDLE. Cannot allocate command buffers.\n";
        throw std::runtime_error("commandPool is null");
    }

    size_t framebufferCount = app.swapchainFramebuffers.size();
    if (framebufferCount == 0)
    {
        std::cerr << "[CRITICAL] No framebuffers available to allocate command buffers for.\n";
        throw std::runtime_error("no swapchain framebuffers");
    }

    app.commandBuffers.resize(framebufferCount);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = app.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(framebufferCount);

    std::cout << "[COMMAND BUFFERS] Allocating " << framebufferCount << " command buffers...\n";

    if (vkAllocateCommandBuffers(app.device, &allocInfo, app.commandBuffers.data()) != VK_SUCCESS)
    {
        std::cerr << "[CRITICAL] Failed to allocate command buffers.\n";
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for (size_t i = 0; i < app.commandBuffers.size(); ++i)
    {
        if (app.commandBuffers[i] == VK_NULL_HANDLE)
        {
            std::cerr << "[ERROR] commandBuffers[" << i << "] is VK_NULL_HANDLE after allocation!\n";
        }
        else
        {
            std::cout << "[COMMAND BUFFERS] commandBuffer[" << i << "] successfully allocated.\n";
        }
    }

    std::cout << "[COMMAND BUFFERS] All command buffers allocated.\n";
}
