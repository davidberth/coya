
file(GLOB core_sources "core/*.c")
file(GLOB platform_windows_sources "platform/windows/*.c")
file(GLOB input_sources "input/*.c")
file(GLOB renderer_sources "renderer/*.c")
set(all_c_sources main.c ${platform_windows_sources} ${core_sources} ${input_sources} ${renderer_sources})

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

