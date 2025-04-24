#pragma once
#include <string>
#include <vector>
#include <vulkan/VulkanApp.hpp>
#include <vulkan/vulkan.h>

class PipelineBuilder
{
public:
    PipelineBuilder(VulkanApp &app);
    void createGraphicsPipeline();
    void createGridPipeline();

private:
    VulkanApp &app;
    std::vector<char> readFile(const std::string &filename);
    VkShaderModule createShaderModule(const std::vector<char> &code);
};