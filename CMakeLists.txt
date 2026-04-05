cmake_minimum_required(VERSION 3.20)

project(SpiritSpace
    VERSION 0.1.0
    DESCRIPTION "Real-universe space simulation game"
    LANGUAGES CXX
)

# ── Language standard ─────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Output directories ────────────────────────────────────────────────────────
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ── Compiler warnings ─────────────────────────────────────────────────────────
if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# ── vcpkg packages (populated per sprint) ────────────────────────────────────
# EP-01-S01: none yet — GTest via FetchContent
# EP-01-S02: glfw3, glad, glm, entt
# EP-01-S03: nlohmann-json, fast-cpp-csv, curl, tinyxml2, stb
# EP-02+:    assimp, fastnoise2, imgui
# EP-11+:    miniaudio

# ── Google Test (FetchContent — no vcpkg needed) ──────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ── Main executable ───────────────────────────────────────────────────────────
add_executable(spirit_space
    src/main.cpp
)

target_include_directories(spirit_space PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_features(spirit_space PRIVATE cxx_std_17)

# Copy config directory next to the executable
add_custom_command(TARGET spirit_space POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config
        $<TARGET_FILE_DIR:spirit_space>/config
    COMMENT "Copying config files..."
)

# ── Tests ─────────────────────────────────────────────────────────────────────
enable_testing()

add_executable(spirit_space_tests
    tests/test_smoke.cpp
)

target_include_directories(spirit_space_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(spirit_space_tests
    PRIVATE
    GTest::gtest_main
)

target_compile_features(spirit_space_tests PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(spirit_space_tests)
