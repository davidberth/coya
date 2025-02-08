add_executable(coya
    "main.c"
    "platform/window.c"
    "core/logger.c"
)

include_directories(coya  
        "${CMAKE_SOURCE_DIR}/src" 
        "${CMAKE_BINARY_DIR}/src"

)
add_subdirectory(configure)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
target_compile_options(coya PRIVATE "/MP")
set_property(TARGET coya PROPERTY C_STANDARD 23)
