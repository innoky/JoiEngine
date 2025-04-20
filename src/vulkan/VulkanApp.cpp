#include "vulkan/VulkanApp.hpp"

#include "TUtils/TUtils.hpp"

#include "vulkan/data/TriangleData.hpp"
#include "vulkan/window/WindowInit.hpp"
#include "vulkan/VulkanInitializer.hpp"
#include "vulkan/renderer/DrawFrame.hpp"

#include <thread>
#include <chrono>
#include <iostream>




void VulkanApp::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void VulkanApp::initWindow()
{
    window = WindowInit::create(1200, 1000, "Visualizer");
}
void VulkanApp::initVulkan()
{
    VulkanInitializer initializer(*this);
    initializer.init();


}

void VulkanApp::mainLoop()
{
    if (!drawFrameHandler)
    {
        std::cerr << "[CRASH] drawFrameHandler is nullptr\n";
        return;
    }

    if (!scene)
    {
        std::cerr << "[CRASH] scene is nullptr\n";
        return;
    }

    const float targetFrameTime = 1.0f / 60.0f; 

    while (!glfwWindowShouldClose(window))
    {
        auto frameStart = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        float time = static_cast<float>(glfwGetTime());

        scene->update(time);
        drawFrameHandler->drawFrame(scene.get());

        auto frameEnd = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float>(frameEnd - frameStart).count();

        // Ограничение FPS
        if (frameTime < targetFrameTime)
        {
            float sleepTime = targetFrameTime - frameTime;
            std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
        }

        // Вывод FPS
        // static float timeAccumulator = 0.0f;
        // static int frameCount = 0;
        // timeAccumulator += frameTime;
        // frameCount++;
        // if (timeAccumulator >= 1.0f)
        // {
        //     // std::cout << "[INFO] FPS: " << frameCount << std::endl;
        //     timeAccumulator = 0.0f;
        //     frameCount = 0;
        // }
    }

    vkDeviceWaitIdle(device);
}
void VulkanApp::cleanup()
{

    vkDeviceWaitIdle(device);

    if (scene)
        scene->cleanup();

    for (auto framebuffer : swapchainFramebuffers)
        vkDestroyFramebuffer(device, framebuffer, nullptr);

    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

    vkDestroyRenderPass(device, renderPass, nullptr);

    for (auto imageView : swapchainImageViews)
        vkDestroyImageView(device, imageView, nullptr);

    vkDestroySwapchainKHR(device, swapchain, nullptr);

    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
    vkDestroyFence(device, inFlightFence, nullptr);

    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}
