//
// Created by Gianni on 3/01/2025.
//

#include "vulkan_utils.hpp"

void vulkanCheck(VkResult result, const char* msg, std::source_location location)
{
    check(result == VK_SUCCESS, msg, location);
}

void loadDebugUtilsFunctionPointers(VkInstance instance)
{
#ifdef DEBUG_MODE
    pfnSetDebugUtilsObjectNameEXT = reinterpret_cast<PFN_vkSetDebugUtilsObjectNameEXT>(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
    pfnCmdBeginDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdBeginDebugUtilsLabelEXT>(vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
    pfnCmdEndDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdEndDebugUtilsLabelEXT>(vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
    pfnCmdInsertDebugUtilsLabelEXT = reinterpret_cast<PFN_vkCmdInsertDebugUtilsLabelEXT>(vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));
#endif
}

void setDebugVulkanObjectName(VkDevice device, VkObjectType type, const std::string& name, const void* handle)
{
#ifdef DEBUG_MODE
    VkDebugUtilsObjectNameInfoEXT objectNameInfo {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .objectType = type,
        .objectHandle = (uint64_t)handle,
        .pObjectName = name.c_str()
    };

    VkResult result = pfnSetDebugUtilsObjectNameEXT(device, &objectNameInfo);
    vulkanCheck(result, "Failed to create Vulkan object name.");
#endif
}

std::optional<uint32_t> findSuitableMemoryType(VkPhysicalDeviceMemoryProperties memoryProperties,
                                               uint32_t resourceSupportedMemoryTypes,
                                               VkMemoryPropertyFlags desiredMemoryProperties)
{
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
    {
        if (resourceSupportedMemoryTypes & (1 << i) &&
            (desiredMemoryProperties & memoryProperties.memoryTypes[i].propertyFlags) == desiredMemoryProperties)
            return i;
    }

    return std::optional<uint32_t>();
}