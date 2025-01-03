cmake_minimum_required(VERSION 3.28)
project(VulkanRenderingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(DEPENDENCY_DIR ${CMAKE_SOURCE_DIR}/dependencies)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(FATAL_ERROR "MinGW compiler required")
endif()

file(GLOB INCLUDE_DIRS ${DEPENDENCY_DIR}/*/include)
file(GLOB LIBS ${DEPENDENCY_DIR}/*/lib/*.a)
file(GLOB IMGUI_SRC ${IMGUI_DIR}/include/imgui/*.cpp)

add_executable(${PROJECT_NAME}
        ${IMGUI_SRC}
        src/main.cpp
        src/pch.cpp
        src/application.cpp
        src/application.hpp
        src/utils.cpp
        src/utils.hpp
        src/input.cpp
        src/input.hpp
        src/vk/vulkan_instance.cpp
        src/vk/vulkan_instance.hpp
        src/vk/include.hpp
        src/vk/vulkan_render_device.cpp
        src/vk/vulkan_render_device.hpp
        src/vk/vulkan_swapchain.cpp
        src/vk/vulkan_swapchain.hpp
        src/vk/vulkan_image.cpp
        src/vk/vulkan_image.hpp
        src/vk/vulkan_buffer.cpp
        src/vk/vulkan_buffer.hpp
        src/vk/vulkan_utils.cpp
        src/vk/vulkan_utils.hpp
        src/vk/vulkan_texture.cpp
        src/vk/vulkan_texture.hpp
)

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX -d)

find_package(Vulkan REQUIRED)
message(STATUS "Vulkan include directory: ${Vulkan_INCLUDE_DIR}")
message(STATUS "Vulkan libraries: ${Vulkan_LIBRARIES}")

target_include_directories(${PROJECT_NAME} PRIVATE
    ${Vulkan_INCLUDE_DIR}
    ${INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Vulkan_LIBRARIES}
    ${LIBS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    GLFW_INCLUDE_VULKAN
    STB_IMAGE_IMPLEMENTATION
    GLM_FORCE_DEPTH_ZERO_TO_ONE
    GLM_FORCE_RADIANS
    GLM_FORCE_LEFT_HANDED
    $<$<CONFIG:Debug>:DEBUG_MODE>
)
