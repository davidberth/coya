
file(GLOB platform_windows_sources "platform/windows/*.c")
file(GLOB core_sources "core/*.c")
set(all_c_sources main.c ${platform_windows_sources} ${core_sources})

add_executable(coya
    ${all_c_sources}
)
target_include_directories(coya 
    PUBLIC 
        "c:/db/coya/src"
        "c:/db/coya/build/x64-Debug/src"
)
add_subdirectory("configure")
set_property(TARGET coya PROPERTY C_STANDARD 23)
