//
// Created by Gianni on 28/02/2025.
//

#ifndef VULKANRENDERINGENGINE_LIGHTS_HPP
#define VULKANRENDERINGENGINE_LIGHTS_HPP

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

enum class LightType
{
    Directional,
    Point,
    Spot
};

struct DirectionalLight
{
    alignas(16) glm::vec4 color;
    alignas(16) glm::vec4 direction;
    alignas(4) float intensity;
};

struct PointLight
{
    alignas(16) glm::vec4 color;
    alignas(16) glm::vec4 position;
    alignas(4) float intensity;
    alignas(4) float range;
};

struct SpotLight
{
    alignas(16) glm::vec4 color;
    alignas(16) glm::vec4 position;
    alignas(16) glm::vec4 direction;
    alignas(4) float intensity;
    alignas(4) float range;
    alignas(4) float innerAngle;
    alignas(4) float outerAngle;
};

inline glm::vec3 calcLightDir(const glm::vec3& angles)
{
    static constexpr glm::vec3 sInitialDir(0.f, 0.f, -1.f);
    glm::mat3 rotation = glm::toMat3(glm::quat(glm::radians(angles)));
    return rotation * sInitialDir;
}

#endif //VULKANRENDERINGENGINE_LIGHTS_HPP
