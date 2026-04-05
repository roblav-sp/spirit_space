#!/bin/bash
# Warn if Phase 2 headers are included in Phase 1 source files.
# Runs after Bash tool calls (e.g. after a build).
# Exits 0 always — this is advisory only, never blocks.

PHASE2_HEADERS=("EnergySystem" "ShieldSystem" "DamageModel" "WeaponSystem"
                "AlienSpawner" "ReputationSystem" "TechTree" "SpiritRealmRenderer")

if [[ "${CLAUDE_TOOL_INPUT:-}" == *"cmake"* ]] || [[ "${CLAUDE_TOOL_INPUT:-}" == *"make"* ]]; then
  for header in "${PHASE2_HEADERS[@]}"; do
    matches=$(grep -rl "$header" src/ 2>/dev/null | grep -v "Phase2\|game_layer")
    if [[ -n "$matches" ]]; then
      echo "PHASE WARNING: Phase 2 symbol '$header' found in Phase 1 source:" >&2
      echo "$matches" >&2
    fi
  done
fi

exit 0
