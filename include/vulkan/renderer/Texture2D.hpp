#pragma once

#include <vulkan/vulkan.h>
#include "vulkan/VulkanApp.hpp"
#include "extern/stb_image.h"

#include <string>

class VulkanApp;

class Texture2D
{
public:
    Texture2D(VulkanApp &app, const std::string &path);
    ~Texture2D();

    VkImageView getImageView() const { return imageView; }
    VkSampler getSampler() const { return sampler; }

    void bindToDescriptorSet(VkDescriptorSet descriptorSet, uint32_t binding);
    void cleanup();

private:
    VulkanApp &app;
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory imageMemory = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;


    void createTextureImageView();
    void createTextureSampler();
    void createTextureImage(unsigned char *pixels, int width, int height);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};
