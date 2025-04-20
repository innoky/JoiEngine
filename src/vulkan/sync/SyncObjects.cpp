#include "vulkan/sync/SyncObjects.hpp"

SyncObjects::SyncObjects(VulkanApp &app) : app(app) {}

void SyncObjects::createSyncObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; 

    if (vkCreateSemaphore(app.device, &semaphoreInfo, nullptr, &app.imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(app.device, &semaphoreInfo, nullptr, &app.renderFinishedSemaphore) != VK_SUCCESS ||
        vkCreateFence(app.device, &fenceInfo, nullptr, &app.inFlightFence) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create sync objects!");
    }
}