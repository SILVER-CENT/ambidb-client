#!/usr/bin/env bash
# Fetches third-party dependencies into third_party/ for local add_subdirectory builds.
# Run from project root: ./scripts/fetch-deps.sh

set -e
cd "$(dirname "$0")/.."
THIRD_PARTY="${PWD}/third_party"
mkdir -p "$THIRD_PARTY"
cd "$THIRD_PARTY"

fetch() {
    local name="$1"
    local url="$2"
    local tag="$3"
    if [[ -d "$name" ]]; then
        echo "[$name] already exists, skipping"
        return
    fi
    echo "[$name] cloning $url @ $tag"
    git clone --depth 1 --branch "$tag" "$url" "$name"
}

fetch googletest   https://github.com/google/googletest.git   v1.15.2
fetch glfw         https://github.com/glfw/glfw.git          3.4
fetch imgui        https://github.com/ocornut/imgui.git       v1.91.1
fetch imtui        https://github.com/ggerganov/imtui.git    master

echo "Done. Dependencies are in third_party/"
