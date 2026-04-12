cmake_minimum_required(VERSION 3.20)

# ── vcpkg dependencies (P1-S02) ───────────────────────────────────────────────
find_package(glad  CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm   CONFIG REQUIRED)

# ── Engine static library ─────────────────────────────────────────────────────
add_library(engine STATIC
    src/core/Window.cpp
)

# PUBLIC include dir: any target that links engine gets engine/include on its
# include path automatically — no need to repeat target_include_directories.
target_include_directories(engine
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# PUBLIC link: consumers of engine (spirit_space, engine_tests) also get
# gl/glfw/glm headers and link targets without re-finding the packages.
# Note: glfw3 vcpkg port installs as CMake target 'glfw' (not 'glfw3::glfw3').
target_link_libraries(engine
    PUBLIC
        glad::glad
        glfw
        glm::glm
)

target_compile_features(engine PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(engine PRIVATE /W4 /WX /permissive-)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
