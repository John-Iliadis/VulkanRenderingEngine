add_library(stb STATIC src/stb_image.cpp)

target_compile_definitions(stb PRIVATE STB_IMAGE_IMPLEMENTATION)

target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
