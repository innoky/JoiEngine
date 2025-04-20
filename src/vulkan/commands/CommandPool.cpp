#include "vulkan/commands/CommandPool.hpp"

CommandPool::CommandPool(VulkanApp &app) : app(app) {}

QueueFamilyIndices CommandPool::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, app.surface, &presentSupport);
        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) break;
        i++;
    }

    return indices;
}

void CommandPool::createCommandPool()
{
    std::cout << "[DEBUG] Creating command pool...\n";

    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(app.physicalDevice);

    if (!queueFamilyIndices.graphicsFamily.has_value())
    {
        throw std::runtime_error("[CRITICAL] No graphicsFamily found in QueueFamilyIndices!");
    }

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; 
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    VkResult result = vkCreateCommandPool(app.device, &poolInfo, nullptr, &app.commandPool);

    if (result != VK_SUCCESS)
    {
        std::cerr << "[ERROR] vkCreateCommandPool failed with error code: " << result << "\n";
        throw std::runtime_error("failed to create command pool!");
    }

    std::cout << "[DEBUG] Command pool successfully created: " << app.commandPool << "\n";
}
