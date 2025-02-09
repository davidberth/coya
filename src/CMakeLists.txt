add_executable(coya
    "main.c"
    "platform/window.c"
    "core/logger.c"
)
target_include_directories(coya 
    PUBLIC 
        "c:/db/coya/src"
        "c:/db/coya/build/x64-Debug/src"
)
add_subdirectory("configure")
set_property(TARGET coya PROPERTY C_STANDARD 23)
