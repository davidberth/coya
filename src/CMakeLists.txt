file(GLOB core_sources "core/*.c")
file(GLOB input_sources "input/*.c")
file(GLOB math_sources "math/*.c")
file(GLOB platform_sources "platform/*.c")
file(GLOB renderer_sources "renderer/*.c")
file(GLOB resources_sources "resources/*.c")
file(GLOB util_sources "util/*.c")

set(all_c_sources main.c camera.c ${core_sources} ${input_sources} ${math_sources} ${platform_sources} ${renderer_sources} ${resources_sources} ${util_sources})

if(WIN32)
    file(GLOB vulkan_sources "renderer/vulkan/*.c")
    file(GLOB platform_windows_sources "platform/windows/*.c")
    file(GLOB shader_sources "renderer/vulkan/shader/*.c")
    set(all_c_sources ${all_c_sources} ${vulkan_sources} ${platform_windows_sources} ${shader_sources})
endif()

add_executable(coya
    ${all_c_sources}
)
target_include_directories(coya
    PUBLIC
        "${CMAKE_SOURCE_DIR}/src"
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/build/x64-$<CONFIG>/src>"
        "${Vulkan_INCLUDE_DIRS}"
)
target_link_libraries(coya PRIVATE Vulkan::Vulkan)
add_subdirectory("configure")
set_property(TARGET coya PROPERTY C_STANDARD 23)

