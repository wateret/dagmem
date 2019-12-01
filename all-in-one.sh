#!/usr/bin/env bash

set -e

# Configure
cmake . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run test cases
cmake --build build --target run_tcs

set +e
