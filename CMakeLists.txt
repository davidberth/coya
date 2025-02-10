cmake_minimum_required (VERSION 3.30)
project (Coya LANGUAGES C VERSION 0.1.0)

# variables
set(RENDERER "Vulkan")
set(PLATFORM ${CMAKE_SYSTEM_NAME})
set(BUILDTYPE ${CMAKE_BUILD_TYPE})
string(TIMESTAMP current_date "%Y-%m-%d")
set(BUILDDATE ${current_date})

set(CMAKE_C_STANDARD 23)
add_subdirectory(src)