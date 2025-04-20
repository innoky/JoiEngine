#include <vulkan/pipeline/FrameBuffers.hpp>

FrameBuffers::FrameBuffers(VulkanApp &app) : app(app) {}

void FrameBuffers::createFrameBuffers()
{
    app.swapchainFramebuffers.resize(app.swapchainImageViews.size());
    if (app.depthImageView == VK_NULL_HANDLE)
    {
        std::cerr << "[CRASH] depthImageView is VK_NULL_HANDLE before creating framebuffers!\n";
        throw std::runtime_error("depthImageView is NULL");
    }

    for (size_t i = 0; i < app.swapchainImageViews.size(); i++)
    {
        std::array<VkImageView, 2> attachments = {
            app.swapchainImageViews[i],
            app.depthImageView};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = app.renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = app.swapchainExtent.width;
        framebufferInfo.height = app.swapchainExtent.height;
        framebufferInfo.layers = 1;

                if (vkCreateFramebuffer(app.device, &framebufferInfo, nullptr, &app.swapchainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}