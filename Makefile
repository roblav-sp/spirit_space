# ─────────────────────────────────────────────────────────────────────────────
# Spirit Space — Makefile
# Cross-compiles from WSL/Linux to Windows x64 using MinGW-w64.
# Output: build/bin/spirit_space.exe  (Win64 PE binary)
#
# Prerequisites (WSL/Ubuntu):
#   sudo apt install mingw-w64 make git curl zip unzip tar
#   make setup        # clones vcpkg (if missing) and installs deps
#
# Usage:
#   make              build spirit_space.exe
#   make tests        build all test executables
#   make clean        remove build/
#   make setup        bootstrap vcpkg (if missing) + install deps
#   make help         list targets
# ─────────────────────────────────────────────────────────────────────────────

# ── Toolchain ─────────────────────────────────────────────────────────────────
CXX := x86_64-w64-mingw32-g++
AR  := x86_64-w64-mingw32-ar

# ── vcpkg ─────────────────────────────────────────────────────────────────────
VCPKG_ROOT ?= $(HOME)/vcpkg
TRIPLET    := x64-mingw-static
# vcpkg manifest mode installs into the project directory, not VCPKG_ROOT/installed.
VCPKG_INST := $(CURDIR)/vcpkg_installed/$(TRIPLET)
VCPKG_BIN  := $(VCPKG_ROOT)/vcpkg

# ── Flags ─────────────────────────────────────────────────────────────────────
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Werror
INCS     := -I$(VCPKG_INST)/include -Iengine/include
# -MMD -MP: emit a .d file next to each .o for automatic header tracking.
DEPFLAGS := -MMD -MP

# Static-link the GCC/C++ runtime so the .exe needs no MinGW DLLs.
# Windows system libs (gdi32, opengl32 etc.) are always import-linked —
# they live in Windows itself and have no static equivalents.
LDFLAGS  := -L$(VCPKG_INST)/lib -L$(VCPKG_INST)/lib/manual-link -static-libgcc -static-libstdc++

# System libs required by GLFW on Windows
WIN_LIBS := -lgdi32 -lopengl32 -luser32 -lshell32 -lwinmm -lole32 -ladvapi32

# ── Build layout ──────────────────────────────────────────────────────────────
BUILD   := build
BIN_DIR := $(BUILD)/bin
OBJ_DIR := $(BUILD)/obj

# ── Engine static library ─────────────────────────────────────────────────────
ENGINE_SRCS := \
	engine/src/core/Window.cpp \
	engine/src/audio/AudioSystem.cpp \
	engine/src/ecs/World.cpp \
	engine/src/core/Config.cpp \
	engine/src/core/GameStateMachine.cpp \
	engine/src/render/Shader.cpp \
	engine/src/render/GpuBuffer.cpp \
	engine/src/hud/HudSystem.cpp

ENGINE_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(ENGINE_SRCS))
ENGINE_LIB  := $(BUILD)/libengine.a

# ── Main executable ───────────────────────────────────────────────────────────
APP_SRCS := src/main.cpp
APP_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(APP_SRCS))
APP_BIN  := $(BIN_DIR)/spirit_space.exe

# ── Test executables ──────────────────────────────────────────────────────────

# Smoke — no engine deps
SMOKE_SRCS := tests/test_smoke.cpp
SMOKE_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SMOKE_SRCS))
SMOKE_BIN  := $(BIN_DIR)/spirit_space_tests.exe

# Window / GL context
ENG_TEST_SRCS := tests/engine/test_window_context.cpp
ENG_TEST_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(ENG_TEST_SRCS))
ENG_TEST_BIN  := $(BIN_DIR)/engine_tests.exe

# Audio
AUDIO_TEST_SRCS := tests/engine/audio/test_audio_system.cpp
AUDIO_TEST_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(AUDIO_TEST_SRCS))
AUDIO_TEST_BIN  := $(BIN_DIR)/audio_tests.exe

# ECS (EP-ENG-02-S01)
ECS_TEST_SRCS := \
	tests/ecs/test_entity_lifecycle.cpp \
	tests/ecs/test_component_add_remove.cpp \
	tests/ecs/test_view_query.cpp
ECS_TEST_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(ECS_TEST_SRCS))
ECS_TEST_BIN  := $(BIN_DIR)/ecs_tests.exe

# Config (EP-ENG-02-S02)
CONFIG_TEST_SRCS := \
	tests/core/test_config_read.cpp \
	tests/core/test_config_missing_key_default.cpp \
	tests/core/test_config_write_persist.cpp
CONFIG_TEST_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(CONFIG_TEST_SRCS))
CONFIG_TEST_BIN  := $(BIN_DIR)/config_tests.exe

# Render / Shader / DSA (EP-ENG-03-S01)
RENDER_TEST_SRCS := \
	tests/render/test_shader_compile.cpp \
	tests/render/test_shader_missing_file_error.cpp \
	tests/render/test_dsa_buffer_upload.cpp
RENDER_TEST_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(RENDER_TEST_SRCS))
RENDER_TEST_BIN  := $(BIN_DIR)/render_tests.exe

# HUD / ImGui (EP-ENG-04-S01)
HUD_TEST_SRCS := \
	tests/hud/test_imgui_init.cpp \
	tests/hud/test_panel_lifecycle.cpp
HUD_TEST_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(HUD_TEST_SRCS))
HUD_TEST_BIN  := $(BIN_DIR)/hud_tests.exe

# State Machine (EP-ENG-05-S01)
SM_TEST_SRCS := \
	tests/core/test_state_push_pop.cpp \
	tests/core/test_state_lifecycle_hooks.cpp \
	tests/core/test_empty_stack_safe.cpp
SM_TEST_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SM_TEST_SRCS))
SM_TEST_BIN  := $(BIN_DIR)/statemachine_tests.exe

ALL_OBJS := $(ENGINE_OBJS) $(APP_OBJS) \
            $(SMOKE_OBJS) $(ENG_TEST_OBJS) $(AUDIO_TEST_OBJS) \
            $(ECS_TEST_OBJS) $(CONFIG_TEST_OBJS) $(RENDER_TEST_OBJS) \
            $(HUD_TEST_OBJS) $(SM_TEST_OBJS)
DEPS := $(ALL_OBJS:.o=.d)

# ── Phony targets ─────────────────────────────────────────────────────────────
.PHONY: all tests setup clean help _check-deps

# ── Default: build game executable ───────────────────────────────────────────
all: _check-deps $(APP_BIN)
	@echo "Built: $(APP_BIN)"

help:
	@echo "Targets:"
	@echo "  make            build $(APP_BIN)"
	@echo "  make tests      build all test executables"
	@echo "  make setup      bootstrap vcpkg (if needed) + install deps"
	@echo "  make clean      remove $(BUILD)/"

# Fail fast with a clear message if vcpkg deps aren't installed.
_check-deps:
	@if [ ! -d "$(VCPKG_INST)/include" ]; then \
	  echo ""; \
	  echo "ERROR: vcpkg dependencies not found at $(VCPKG_INST)"; \
	  echo ""; \
	  echo "Run:  make setup"; \
	  echo "(or set VCPKG_ROOT to an existing install: make VCPKG_ROOT=/path/to/vcpkg)"; \
	  echo ""; \
	  exit 1; \
	fi

$(APP_BIN): $(APP_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) -limgui -lglad -lglfw3 $(WIN_LIBS) -o $@
	@rm -rf $(BIN_DIR)/config && cp -r config $(BIN_DIR)/config
	@rm -rf $(BIN_DIR)/assets && cp -r assets $(BIN_DIR)/assets

$(ENGINE_LIB): $(ENGINE_OBJS) | $(BUILD)
	$(AR) rcs $@ $^

# Generic rule: compiles any .cpp under the project into a mirrored .o path.
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCS) $(DEPFLAGS) -c $< -o $@

$(BUILD) $(BIN_DIR):
	@mkdir -p $@

# ── Test link rules ───────────────────────────────────────────────────────────
GTEST_LIBS := -lgtest_main -lgtest -lpthread

$(SMOKE_BIN): $(SMOKE_OBJS) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) $(GTEST_LIBS) $(WIN_LIBS) -o $@

$(ENG_TEST_BIN): $(ENG_TEST_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) $(GTEST_LIBS) -lglad -lglfw3 $(WIN_LIBS) -o $@

$(AUDIO_TEST_BIN): $(AUDIO_TEST_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) $(GTEST_LIBS) -lglad -lglfw3 $(WIN_LIBS) -o $@

$(ECS_TEST_BIN): $(ECS_TEST_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) $(GTEST_LIBS) $(WIN_LIBS) -o $@

$(CONFIG_TEST_BIN): $(CONFIG_TEST_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) $(GTEST_LIBS) $(WIN_LIBS) -o $@

$(RENDER_TEST_BIN): $(RENDER_TEST_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) $(GTEST_LIBS) -lglad -lglfw3 $(WIN_LIBS) -o $@

$(HUD_TEST_BIN): $(HUD_TEST_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) $(GTEST_LIBS) -limgui -lglad -lglfw3 $(WIN_LIBS) -o $@

$(SM_TEST_BIN): $(SM_TEST_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) $(GTEST_LIBS) $(WIN_LIBS) -o $@

tests: _check-deps \
       $(SMOKE_BIN) $(ENG_TEST_BIN) $(AUDIO_TEST_BIN) \
       $(ECS_TEST_BIN) $(CONFIG_TEST_BIN) $(RENDER_TEST_BIN) \
       $(HUD_TEST_BIN) $(SM_TEST_BIN)
	@echo "All test binaries built in $(BIN_DIR)/"
	@echo "Run them on a Windows machine."

# ── First-time setup: install system tools, clone + bootstrap vcpkg, install deps
setup:
	@echo "Installing system prerequisites..."
	sudo apt-get install -y curl zip unzip tar git make mingw-w64 pkg-config
	@if [ ! -x "$(VCPKG_BIN)" ]; then \
	  if [ ! -d "$(VCPKG_ROOT)" ]; then \
	    echo "Cloning vcpkg into $(VCPKG_ROOT)..."; \
	    git clone https://github.com/microsoft/vcpkg "$(VCPKG_ROOT)"; \
	  fi; \
	  echo "Bootstrapping vcpkg..."; \
	  "$(VCPKG_ROOT)/bootstrap-vcpkg.sh" -disableMetrics; \
	fi
	"$(VCPKG_BIN)" install --triplet $(TRIPLET)

# ── Clean ─────────────────────────────────────────────────────────────────────
clean:
	rm -rf $(BUILD)

# ── Auto-generated header dependencies ────────────────────────────────────────
-include $(DEPS)
