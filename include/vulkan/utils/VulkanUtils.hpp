#pragma once
#include <vulkan/vulkan.h>

class VulkanApp;

namespace VulkanUtils
{
    void createImage(VulkanApp &app,
                     uint32_t width, uint32_t height,
                     VkFormat format, VkImageTiling tiling,
                     VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
                     VkImage &image, VkDeviceMemory &imageMemory);

    VkImageView createImageView(VulkanApp &app,
                                VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags);

    void transitionImageLayout(VulkanApp &app,
                               VkImage image, VkFormat format,
                               VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(VulkanApp &app,
                           VkBuffer buffer, VkImage image,
                           uint32_t width, uint32_t height);

    void createBuffer(VulkanApp &app,
                      VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties,
                      VkBuffer &buffer, VkDeviceMemory &bufferMemory);
                      
    uint32_t findMemoryType(VkPhysicalDevice physicalDevice,
                            uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);
}
