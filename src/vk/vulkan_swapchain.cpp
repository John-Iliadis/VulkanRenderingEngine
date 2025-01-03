//
// Created by Gianni on 3/01/2025.
//

#include "vulkan_swapchain.hpp"

void VulkanSwapchain::create(GLFWwindow* window,
                             const VulkanInstance& instance,
                             const VulkanRenderDevice& renderDevice)
{
    createSurface(window, instance);
    createSwapchain(renderDevice);
    createSwapchainImages(renderDevice);
    createCommandBuffers(renderDevice);
}

void VulkanSwapchain::destroy(const VulkanInstance& instance, const VulkanRenderDevice& renderDevice)
{
    for (size_t i = 0; i < swapchainImageCount(); ++i)
    {
        vkDestroyImageView(renderDevice.device, imageViews.at(i), nullptr);
    }

    vkDestroySwapchainKHR(renderDevice.device, swapchain, nullptr);
    vkDestroySurfaceKHR(instance.instance, surface, nullptr);
}

void VulkanSwapchain::createSurface(GLFWwindow *window, const VulkanInstance &instance)
{
    VkResult result = glfwCreateWindowSurface(instance.instance, window, nullptr, &surface);
    vulkanCheck(result, "Failed to create Vulkan surface.");
}

void VulkanSwapchain::createSwapchain(const VulkanRenderDevice& renderDevice)
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(renderDevice.physicalDevice, surface, &surfaceCapabilities);

    format = VK_FORMAT_R8G8B8A8_UNORM;
    extent = surfaceCapabilities.currentExtent;

    VkSwapchainCreateInfoKHR swapchainCreateInfoKhr {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = swapchainImageCount(),
        .imageFormat = format,
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
        .preTransform = surfaceCapabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    VkResult result = vkCreateSwapchainKHR(renderDevice.device, &swapchainCreateInfoKhr, nullptr, &swapchain);
    vulkanCheck(result, "Failed to create swapchain.");
}

void VulkanSwapchain::createSwapchainImages(const VulkanRenderDevice& renderDevice)
{
    images.resize(swapchainImageCount());
    imageViews.resize(swapchainImageCount());

    uint32_t imgCount = swapchainImageCount();
    vkGetSwapchainImagesKHR(renderDevice.device, swapchain, &imgCount, images.data());

    for (size_t i = 0; i < swapchainImageCount(); ++i)
    {
        imageViews.at(i) = createImageView(renderDevice,
                                           images.at(i),
                                           VK_IMAGE_VIEW_TYPE_2D,
                                           format,
                                           VK_IMAGE_ASPECT_COLOR_BIT);

        setDebugVulkanObjectName(renderDevice.device,
                                 VK_OBJECT_TYPE_IMAGE_VIEW,
                                 std::format("Swapchain image view {}", i),
                                 imageViews.at(i));
    }
}

void VulkanSwapchain::createCommandBuffers(const VulkanRenderDevice& renderDevice)
{
    commandBuffers.resize(swapchainImageCount());

    VkCommandBufferAllocateInfo commandBufferAllocateInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = renderDevice.commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = swapchainImageCount()
    };

    VkResult result = vkAllocateCommandBuffers(renderDevice.device, &commandBufferAllocateInfo, commandBuffers.data());
    vulkanCheck(result, "Failed to allocate command buffers.");

    for (uint32_t i = 0; i < swapchainImageCount(); ++i)
    {
        setDebugVulkanObjectName(renderDevice.device,
                                 VK_OBJECT_TYPE_COMMAND_BUFFER,
                                 std::format("Swapchain command buffer {}", i),
                                 commandBuffers.at(i));
    }
}

constexpr uint32_t VulkanSwapchain::swapchainImageCount()
{
    return 3;
}
