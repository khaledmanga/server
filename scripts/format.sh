#!/bin/bash

find src/ -iname "*.cpp" -o -name "*.hpp" | xargs clang-format -i