#!/bin/bash
rm -rf build
rm compile_commands.json
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ln -s build/compile_commands.json compile_commands.json
