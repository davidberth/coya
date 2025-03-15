cmake_minimum_required (VERSION 3.30)
project (Coya LANGUAGES C VERSION 0.1.0)
set(CMAKE_C_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Werror=vla)
endif()

# Define custom build configuration properties
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;ReleaseWithDebInfo" CACHE STRING "" FORCE)

# Add debugging symbols and sanitizer for ReleaseWithDebInfo configuration
# Display the current build type
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE MATCHES "ReleaseWithDebInfo")
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-g -O1)
        message(STATUS "Building with debug symbols in optimized mode")
        
        # Enable Address Sanitizer for Clang in this configuration
        if(CMAKE_C_COMPILER_ID MATCHES "Clang")
            add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
            add_link_options(-fsanitize=address)
            message(STATUS "Address Sanitizer enabled")
        endif()
    endif()
endif()

# variables
set(RENDERER "Vulkan")
set(PLATFORM ${CMAKE_SYSTEM_NAME})
set(BUILDTYPE ${CMAKE_BUILD_TYPE})
string(TIMESTAMP current_date "%Y-%m-%d")
set(BUILDDATE ${current_date})

# Vulkan dependency (this is the only dependency!)
find_package(Vulkan REQUIRED)

# add the source directories
add_subdirectory(src)

add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${CMAKE_BINARY_DIR}/compile_commands.json
                    ${CMAKE_BINARY_DIR}/../compile_commands.json
)

add_subdirectory("shaders")