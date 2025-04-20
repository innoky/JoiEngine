#pragma once

#include <vulkan/VulkanApp.hpp>

class SyncObjects
{
public:
    SyncObjects(VulkanApp &app);
    void createSyncObjects();

private:
    VulkanApp &app;
};