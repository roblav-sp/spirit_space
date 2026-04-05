#!/bin/bash
# Auto-format C++ files after every Edit/Write tool call.
# Only runs if clang-format is available and the changed file is C++.

CHANGED_FILE="${CLAUDE_TOOL_INPUT_FILE_PATH:-}"

# Only act on C++ source files
if [[ "$CHANGED_FILE" =~ \.(cpp|cxx|cc|h|hpp|hxx)$ ]]; then
  if command -v clang-format &>/dev/null; then
    clang-format -i "$CHANGED_FILE" 2>/dev/null
  fi
fi

exit 0
