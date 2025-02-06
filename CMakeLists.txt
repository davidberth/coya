cmake_minimum_required (VERSION 3.30)
project ("coya" LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
add_subdirectory(src)

