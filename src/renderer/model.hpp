//
// Created by Gianni on 13/01/2025.
//

#ifndef VULKANRENDERINGENGINE_MODEL_HPP
#define VULKANRENDERINGENGINE_MODEL_HPP

#include <glm/glm.hpp>
#include "../app/simple_notification_service.hpp"
#include "../app/uuid_registry.hpp"
#include "../vk/vulkan_texture.hpp"
#include "../vk/vulkan_descriptor.hpp"
#include "instanced_mesh.hpp"

constexpr uint32_t PerModelMaxTextureCount = 512;

struct Material
{
    alignas(4) int32_t baseColorTexIndex;
    alignas(4) int32_t metallicTexIndex;
    alignas(4) int32_t roughnessTexIndex;
    alignas(4) int32_t normalTexIndex;
    alignas(4) int32_t aoTexIndex;
    alignas(4) int32_t emissionTexIndex;
    alignas(4) float metallicFactor;
    alignas(4) float roughnessFactor;
    alignas(16) glm::vec4 baseColorFactor;
    alignas(16) glm::vec4 emissionFactor;
    alignas(8) glm::vec2 tiling;
    alignas(8) glm::vec2 offset;
};

struct SceneNode
{
    std::string name;
    glm::mat4 transformation;
    std::vector<uint32_t> meshIndices;
    std::vector<SceneNode> children;
};

struct Mesh
{
    uuid32_t meshID;
    std::string name;
    InstancedMesh mesh;
    int32_t materialIndex;
};

struct Texture
{
    std::string name;
    VulkanTexture vulkanTexture;
    VkDescriptorSet descriptorSet;
};

class Model : public SubscriberSNS
{
public:
    uuid32_t id;
    std::string name;
    std::filesystem::path path;

    SceneNode root;
    std::vector<Texture> textures;
    std::vector<Material> materials;
    std::vector<std::string> materialNames;
    std::vector<Mesh> meshes;

public:
    Model(const VulkanRenderDevice* renderDevice);
    ~Model();

    void updateMaterial(index_t matIndex);

    void notify(const Message &message) override;

    void createMaterialsSSBO();
    void createTextureDescriptorSets(VkDescriptorSetLayout dsLayout);
    void createMaterialsDescriptorSet(VkDescriptorSetLayout dsLayout);

    void render(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t matDsIndex) const;

    Mesh* getMesh(uuid32_t meshID);

private:
    const VulkanRenderDevice* mRenderDevice;
    VkDescriptorSet mMaterialsDS;
    VulkanBuffer mMaterialsSSBO;
};

#endif //VULKANRENDERINGENGINE_MODEL_HPP
