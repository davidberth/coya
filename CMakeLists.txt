cmake_minimum_required (VERSION 3.30)
project (Coya LANGUAGES C VERSION 0.1.0)
set(CMAKE_C_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Werror=vla)
endif()


# add debugging symbols in release mode
if(CMAKE_BUILD_TYPE MATCHES "Release")
  if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-g)
endif()
endif()
# address sanitizer
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
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