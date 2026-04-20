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
#   make tests        build test executables
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
ENGINE_SRCS := engine/src/core/Window.cpp \
               engine/src/audio/AudioSystem.cpp
ENGINE_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(ENGINE_SRCS))
ENGINE_LIB  := $(BUILD)/libengine.a

# ── Main executable ───────────────────────────────────────────────────────────
APP_SRCS := src/main.cpp
APP_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(APP_SRCS))
APP_BIN  := $(BIN_DIR)/spirit_space.exe

# ── Test executables ──────────────────────────────────────────────────────────
SMOKE_SRCS    := tests/test_smoke.cpp
SMOKE_OBJS    := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SMOKE_SRCS))
SMOKE_BIN     := $(BIN_DIR)/spirit_space_tests.exe

ENG_TEST_SRCS := tests/engine/test_window_context.cpp
ENG_TEST_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(ENG_TEST_SRCS))
ENG_TEST_BIN  := $(BIN_DIR)/engine_tests.exe

AUDIO_TEST_SRCS := tests/engine/audio/test_audio_system.cpp
AUDIO_TEST_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(AUDIO_TEST_SRCS))
AUDIO_TEST_BIN  := $(BIN_DIR)/audio_tests.exe

ALL_OBJS := $(ENGINE_OBJS) $(APP_OBJS) $(SMOKE_OBJS) $(ENG_TEST_OBJS) $(AUDIO_TEST_OBJS)
DEPS     := $(ALL_OBJS:.o=.d)

# ── Phony targets ─────────────────────────────────────────────────────────────
.PHONY: all tests setup clean help _check-deps

# ── Default: build game executable ───────────────────────────────────────────
all: _check-deps $(APP_BIN)
	@echo "Built: $(APP_BIN)"

help:
	@echo "Targets:"
	@echo "  make            build $(APP_BIN)"
	@echo "  make tests      build test executables"
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
	$(CXX) $^ $(LDFLAGS) -lglad -lglfw3 $(WIN_LIBS) -o $@
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

# ── Tests ─────────────────────────────────────────────────────────────────────
# Smoke tests have no engine/GL dependency.
$(SMOKE_BIN): $(SMOKE_OBJS) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) -lgtest_main -lgtest -lpthread $(WIN_LIBS) -o $@

# Engine tests link the engine (brings in glad + glfw3).
$(ENG_TEST_BIN): $(ENG_TEST_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) -lgtest_main -lgtest -lglad -lglfw3 -lpthread $(WIN_LIBS) -o $@

# Audio tests link the engine (brings in miniaudio via AudioSystem).
$(AUDIO_TEST_BIN): $(AUDIO_TEST_OBJS) $(ENGINE_LIB) | $(BIN_DIR)
	$(CXX) $^ $(LDFLAGS) -lgtest_main -lgtest -lglad -lglfw3 -lpthread $(WIN_LIBS) -o $@

tests: _check-deps $(SMOKE_BIN) $(ENG_TEST_BIN) $(AUDIO_TEST_BIN)
	@echo "Test binaries built in $(BIN_DIR)/"
	@echo "Run them on a Windows machine or via:  wine $(SMOKE_BIN)"

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
