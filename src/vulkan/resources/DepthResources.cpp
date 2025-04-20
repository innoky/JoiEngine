#pragma once
#include <vulkan/vulkan.h>
#include "vulkan/resources/DepthResources.hpp"


DepthResources::DepthResources(VulkanApp &app) : app(app) {}
void DepthResources::createDepthResources()
{
    std::cout << "[DEPTH] Creating depth resources...\n";

    VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;

    // Image
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
        throw std::runtime_error("[DEPTH] Failed to create depth image");

    std::cout << "[DEPTH] Image created: " << app.depthImage << "\n";

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(app.device, app.depthImage, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(app.device, &allocInfo, nullptr, &app.depthImageMemory) != VK_SUCCESS)
        throw std::runtime_error("[DEPTH] Failed to allocate memory for depth image");

    vkBindImageMemory(app.device, app.depthImage, app.depthImageMemory, 0);

    // Image View
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
        throw std::runtime_error("[DEPTH] Failed to create image view");

    std::cout << "[DEPTH] Depth image view created: " << app.depthImageView << "\n";
}
uint32_t DepthResources::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
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
