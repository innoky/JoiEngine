#include "vulkan/renderer/DrawFrame.hpp"
#include "vulkan/render/PushConstants.hpp"
#include "vulkan/renderer/Scene.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

DrawFrame::DrawFrame(VulkanApp &app) : app(app) {}

void DrawFrame::drawFrame(Scene *scene)
{
    if (!scene)
    {
        std::cerr << "[ERROR] drawFrame: scene is nullptr\n";
        return;
    }

    vkWaitForFences(app.device, 1, &app.inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(app.device, 1, &app.inFlightFence);

    uint32_t imageIndex;
    if (vkAcquireNextImageKHR(app.device, app.swapchain, UINT64_MAX, app.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex) != VK_SUCCESS)
    {
        std::cerr << "[ERROR] Failed to acquire image!\n";
        return;
    }

    vkResetCommandBuffer(app.commandBuffers[imageIndex], 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    vkBeginCommandBuffer(app.commandBuffers[imageIndex], &beginInfo);

    VkClearValue clearValues[2];
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = app.renderPass;
    renderPassInfo.framebuffer = app.swapchainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = app.swapchainExtent;
    renderPassInfo.clearValueCount = 2;
    renderPassInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(app.commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(app.commandBuffers[imageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, app.graphicsPipeline);

    // 💡 Full scene controll
    scene->draw(app.commandBuffers[imageIndex], app.pipelineLayout);

    vkCmdEndRenderPass(app.commandBuffers[imageIndex]);
    vkEndCommandBuffer(app.commandBuffers[imageIndex]);

    VkSemaphore waitSemaphores[] = {app.imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &app.commandBuffers[imageIndex];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &app.renderFinishedSemaphore;

    if (vkQueueSubmit(app.graphicsQueue, 1, &submitInfo, app.inFlightFence) != VK_SUCCESS)
        throw std::runtime_error("failed to submit draw command buffer!");

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &app.renderFinishedSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &app.swapchain;
    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(app.presentQueue, &presentInfo);
}
