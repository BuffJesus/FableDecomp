#!/usr/bin/env bash
# Build add_creature.exe against the prebuilt FableForge libforgecore.a.
set -e
FF="D:/Code/FableForge"
HERE="$(cd "$(dirname "$0")" && pwd)"
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ \
  -I"$FF/libs/forgecore/include" \
  -I"$FF/third_party" \
  "$HERE/02_add_creature.cpp" \
  "$FF/build/libforgecore.a" \
  -o "$HERE/add_creature.exe"
echo "built $HERE/add_creature.exe"
