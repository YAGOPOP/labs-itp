#!/usr/bin/env bash

cmake -B "build/Release" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -G Ninja
cmake -B "build/Debug" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -G Ninja
