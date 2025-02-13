cmake_minimum_required (VERSION 3.30)
project (Coya LANGUAGES C VERSION 0.1.0)
set(CMAKE_C_STANDARD 23)

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