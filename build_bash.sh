#!/bin/bash

mkdir build
cd build/
cmake ..
make -j
chmod 777 json_cpp
cp json_cpp ../