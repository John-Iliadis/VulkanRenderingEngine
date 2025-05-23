//
// Created by Gianni on 4/01/2025.
//

#include "camera.hpp"

static constexpr float PanVarDivisor = 10000.f;
static constexpr float RotateVarDivisor = 200.f;
static constexpr float FlySpeedDivisor = 20.f;

Camera::Camera(glm::vec3 position, float fovY, float width, float height, float nearZ, float farZ)
    : mView()
    , mAxisX(1.0, 0.f, 0.f)
    , mAxisY(0.f, 1.f, 0.f)
    , mAxisZ(0.f, 0.f, 1.f)
    , mPosition(position)
    , mTheta()
    , mPhi()
    , mProjection(glm::perspective(glm::radians(fovY), width / height, nearZ, farZ))
    , mFovY(fovY)
    , mAspectRatio(width / height)
    , mNearZ(nearZ)
    , mFarZ(farZ)
    , mPreviousMousePos()
    , mState(Camera::State::VIEW_MODE)
    , mFlySpeed(150.f)
    , mPanSpeed(40.f)
    , mZScrollOffset(1.f)
    , mRotateSensitivity(10.f)
    , mLeftButtonDown()
    , mRightButtonDown()
{
}

void Camera::setState(Camera::State state)
{
    mState = state;
}

void Camera::update(float dt)
{
    if (ImGui::IsWindowHovered())
    {
        mLeftButtonDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
        mRightButtonDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        mLeftButtonDown = false;

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        mRightButtonDown = false;

    switch (mState)
    {
        case FIRST_PERSON:
            updateFirstPerson(dt);
            break;
        case VIEW_MODE:
            updateViewMode(dt);
            break;
        case EDITOR_MODE:
            updateEditorMode(dt);
            break;
    }

    if (ImGui::IsWindowHovered())
        scroll(ImGui::GetIO().MouseWheelH, ImGui::GetIO().MouseWheel);

    calculateViewProjection();

    mPreviousMousePos.x = ImGui::GetMousePos().x;
    mPreviousMousePos.y = ImGui::GetMousePos().y;
}

const glm::mat4 &Camera::viewProjection() const
{
    return mViewProjection;
}

const glm::mat4 &Camera::view() const
{
    return mView;
}

const glm::mat4& Camera::projection() const
{
    return mProjection;
}

const glm::vec3 &Camera::position() const
{
    return mPosition;
}

const glm::vec3 &Camera::front() const
{
    return mAxisZ;
}

const Camera::State Camera::state() const
{
    return mState;
}

float *Camera::fov()
{
    return &mFovY;
}

float *Camera::nearPlane()
{
    return &mNearZ;
}

float *Camera::farPlane()
{
    return &mFarZ;
}

float *Camera::flySpeed()
{
    return &mFlySpeed;
}

float *Camera::panSpeed()
{
    return &mPanSpeed;
}

float *Camera::zScrollOffset()
{
    return &mZScrollOffset;
}

float *Camera::rotateSensitivity()
{
    return &mRotateSensitivity;
}

void Camera::resize(uint32_t width, uint32_t height)
{
    mAspectRatio = static_cast<float>(glm::max(1u, width)) / static_cast<float>(glm::max(1u,height));
}

void Camera::scroll(float x, float y)
{
    if (!y) return;

    if (mState == VIEW_MODE || mState == EDITOR_MODE)
    {
        mPosition += mAxisZ * mZScrollOffset * glm::sign(y);
    }
}

void Camera::updateFirstPerson(float dt)
{
    int z = 0;
    int x = 0;

    if (ImGui::IsWindowFocused())
    {
        if (ImGui::IsKeyDown(ImGuiKey_W))
            --z;
        if (ImGui::IsKeyDown(ImGuiKey_S))
            ++z;
        if (ImGui::IsKeyDown(ImGuiKey_A))
            --x;
        if (ImGui::IsKeyDown(ImGuiKey_D))
            ++x;
    }

    glm::vec2 mouseDt{};
    if (mLeftButtonDown)
    {
        mouseDt.x = ImGui::GetMousePos().x - mPreviousMousePos.x;
        mouseDt.y = ImGui::GetMousePos().y - mPreviousMousePos.y;
    }

    if (z || x)
    {
        float xOffset = -mFlySpeed * dt * glm::sign(x) / FlySpeedDivisor;
        float zOffset = -mFlySpeed * dt * glm::sign(z) / FlySpeedDivisor;

        mPosition += mAxisX * xOffset + mAxisZ * zOffset;
    }

    if (mouseDt.x || mouseDt.y)
    {
        mTheta += -mouseDt.x * mRotateSensitivity / RotateVarDivisor;
        mPhi += mouseDt.y * mRotateSensitivity / RotateVarDivisor;
        mPhi = glm::clamp(mPhi, -89.f, 89.f);
    }
}

void Camera::updateViewMode(float dt)
{
    glm::vec2 mouseDt;
    mouseDt.x = ImGui::GetMousePos().x - mPreviousMousePos.x;
    mouseDt.y = ImGui::GetMousePos().y - mPreviousMousePos.y;

    if (mouseDt.x || mouseDt.y)
    {
        if (mRightButtonDown)
        {
            mTheta += -mouseDt.x * mRotateSensitivity / RotateVarDivisor;
            mPhi += mouseDt.y * mRotateSensitivity / RotateVarDivisor;
            mPhi = glm::clamp(mPhi, -89.f, 89.f);
        }
        else if (mLeftButtonDown)
        {
            float xOffset = mPanSpeed * mouseDt.x / PanVarDivisor;
            float yOffset = mPanSpeed * mouseDt.y / PanVarDivisor;

            mPosition += mAxisX * xOffset + mAxisY * yOffset;
        }
    }
}

void Camera::updateEditorMode(float dt)
{
    glm::vec2 mouseDt{};
    if (mRightButtonDown)
    {
        mouseDt.x = ImGui::GetMousePos().x - mPreviousMousePos.x;
        mouseDt.y = ImGui::GetMousePos().y - mPreviousMousePos.y;
    }

    if (mouseDt.x || mouseDt.y)
    {
        mTheta += -mouseDt.x * mRotateSensitivity / RotateVarDivisor;
        mPhi += mouseDt.y * mRotateSensitivity / RotateVarDivisor;
        mPhi = glm::clamp(mPhi, -89.f, 89.f);
    }
}

void Camera::calculateBasis()
{
    float theta = glm::radians(mTheta);
    float phi = glm::radians(mPhi);

    mAxisZ = {
        glm::sin(theta) * glm::cos(phi),
        glm::sin(phi),
        glm::cos(theta) * glm::cos(phi)
    };

    mAxisZ *= -1.f;
    mAxisX = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), mAxisZ));
    mAxisY = glm::normalize(glm::cross(mAxisX, -mAxisZ));
}

void Camera::calculateViewProjection()
{
    calculateBasis();
    mView = glm::lookAt(mPosition, mPosition + mAxisZ, glm::vec3(0.f, 1.f, 0.f));
    mProjection = glm::perspective(glm::radians(mFovY), mAspectRatio, mNearZ, mFarZ);
    mProjection[1].y *= -1.f;
    mViewProjection = mProjection * mView;
}

const CameraRenderData Camera::renderData() const
{
    return {
        .view = mView,
        .projection = mProjection,
        .viewProj = mViewProjection,
        .position = glm::vec4(mPosition, 1.f),
        .direction = glm::vec4(mAxisZ, 1.f),
        .nearPlane = mNearZ,
        .farPlane = mFarZ
    };
}
