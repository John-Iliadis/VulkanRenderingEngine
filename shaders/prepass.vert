#version 460 core

#include "vertex_input.glsl"

layout (location = 0) out vec3 vNormal;

layout (set = 0, binding = 0) uniform CameraUBO {
    mat4 view;
    mat4 projection;
    mat4 viewProj;
    vec4 cameraPos;
    vec4 cameraDir;
    float nearPlane;
    float farPlane;
};

void main()
{
    vNormal = normalMat * normal;
    gl_Position = viewProj * modelMat * vec4(position, 1.0);
}
