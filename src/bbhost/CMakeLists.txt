enable_language(RC)
add_executable(bbhost main.c cmdline.c hostwin.c billbrd.c hooks.c version.rc)

add_compile_definitions(UNICODE)

configure_file(version.h.in version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_path(DETOURS_INCLUDE_DIR NAMES "detours/detours.h")
find_library(DETOURS_LIBRARY detours)
include_directories(${DETOURS_INCLUDE_DIR})
target_link_libraries(bbhost ${DETOURS_LIBRARY} uxtheme.dll version.dll)

set_property(TARGET bbhost PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

install(TARGETS bbhost)