#!/bin/bash

echo "Removing the build folder"
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
echo ${SCRIPT_DIR}
cd ${SCRIPT_DIR}
cd ..

if [[ -d "build" ]]; then
    rm -rf build/*
fi

echo "Generating linux project:"
cmake -S "." -B "build" 
