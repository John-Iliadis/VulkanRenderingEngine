//
// Created by Gianni on 11/01/2025.
//

#ifndef VULKANRENDERINGENGINE_INSTANCED_MESH_HPP
#define VULKANRENDERINGENGINE_INSTANCED_MESH_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "../vk/vulkan_render_device.hpp"
#include "../vk/vulkan_buffer.hpp"
#include "../app/types.hpp"
#include "vertex.hpp"

class InstancedMesh
{
public:
    struct InstanceData
    {
        glm::mat4 modelMatrix;
        glm::mat3 normalMatrix;
        uuid32_t id;
    };

public:
    InstancedMesh();
    InstancedMesh(const VulkanRenderDevice& renderDevice,
                  const std::vector<Vertex>& vertices,
                  const std::vector<uint32_t>& indices);
    InstancedMesh(const VulkanRenderDevice& renderDevice,
                  VulkanBuffer&& vertexBuffer,
                  VulkanBuffer&& indexBuffer);

    void addInstance(uuid32_t id);
    void updateInstance(uuid32_t id, const glm::mat4& transformation);
    void removeInstance(uuid32_t id);
    void setDebugName(const std::string& debugName);
    void render(VkCommandBuffer commandBuffer) const;

    static std::vector<VkVertexInputBindingDescription> bindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> attributeDescriptions();

private:
    void checkResize();

private:
    const VulkanRenderDevice* mRenderDevice;

    VulkanBuffer mVertexBuffer;
    VulkanBuffer mIndexBuffer;
    VulkanBuffer mInstanceBuffer;

    uint32_t mInstanceCount;
    uint32_t mInstanceBufferCapacity;

    std::unordered_map<uuid32_t, index_t> mInstanceIdToIndexMap;
    std::unordered_map<index_t, uuid32_t> mInstanceIndexToIdMap;
};

#endif //VULKANRENDERINGENGINE_INSTANCED_MESH_HPP
