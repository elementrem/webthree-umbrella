#!/bin/bash

set -e

git submodule sync                    
git submodule update --init --remote .
mkdir -p build
cd build
cmake ..
make -j 4
cd ..
# Only update if tests do not fail (disabled for now)
#./webthree-helpers/scripts/eletests.sh all --umbrella
git checkout -B develop
git commit -am 'updated submodules'   
git push

