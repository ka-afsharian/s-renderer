#!/bin/bash

cmake -B buildtree
cmake --build buildtree
cmake --install buildtree
cmake -S . -B buildtree -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cd bin/RELEASE/
time GALLIUM_HUD=fps,cpu,tmp ./run
#time mangohud ./run
