#!/usr/bin/env bash

cmake -S . -B build -DENABLE_COVERAGE=ON &&
cmake --build build &&
ctest --test-dir build --output-on-failure &&
mkdir -p coverage &&
gcovr -r . build     --exclude 'build/_deps/.*'     --exclude 'test/.*'     --html-details     -o coverage/index.html
