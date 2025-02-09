cmake_minimum_required (VERSION 3.30)
project (Coya LANGUAGES C VERSION 0.1.0)
set(RENDERER "Vulkan")
set(PLATFORM ${CMAKE_SYSTEM_NAME})
set(CMAKE_C_STANDARD 23)

add_subdirectory(src)