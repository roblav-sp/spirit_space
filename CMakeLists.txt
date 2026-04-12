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

# ── vcpkg dependency timeline ─────────────────────────────────────────────────
# P1-S01: none — GTest via FetchContent
# P1-S02: glad, glfw3, glm              ← current sprint
# P1-S03: entt, nlohmann-json
# P1-S05: (input bindings — no new dep)
# P1-S07: (shader system — no new dep)
# P1-S08: assimp, stb
# P1-S10: imgui
# P1-S11: miniaudio

# ── Google Test (FetchContent — no vcpkg needed) ──────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ── Engine static library ─────────────────────────────────────────────────────
# Defines 'engine' target; pulls in glad, glfw, glm as PUBLIC dependencies.
# Consumers that link 'engine' automatically get those include dirs + libs.
add_subdirectory(engine)

# ── Main executable ───────────────────────────────────────────────────────────
add_executable(spirit_space
    src/main.cpp
)

target_link_libraries(spirit_space PRIVATE engine)
target_compile_features(spirit_space PRIVATE cxx_std_17)

# Copy config directory next to the executable after every build.
add_custom_command(TARGET spirit_space POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config
        $<TARGET_FILE_DIR:spirit_space>/config
    COMMENT "Copying config files..."
)

# ── Tests ─────────────────────────────────────────────────────────────────────
enable_testing()

# --- Smoke tests (P1-S01) ---
# No engine dependency — must always pass on CI regardless of GPU driver.
add_executable(spirit_space_tests
    tests/test_smoke.cpp
)
target_include_directories(spirit_space_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(spirit_space_tests PRIVATE GTest::gtest_main)
target_compile_features(spirit_space_tests PRIVATE cxx_std_17)

# --- Engine tests (P1-S02+) ---
# Links engine, so glad/glfw/glm are available. Tests that require an actual
# OpenGL 4.5 driver call GTEST_SKIP() when the context cannot be created
# (i.e., on CI runners without a GPU driver) — skip ≠ failure in ctest.
add_executable(engine_tests
    tests/engine/test_window_context.cpp
)
target_link_libraries(engine_tests PRIVATE engine GTest::gtest_main)
target_compile_features(engine_tests PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(spirit_space_tests)
gtest_discover_tests(engine_tests)
