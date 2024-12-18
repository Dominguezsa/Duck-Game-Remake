#!/bin/bash

sudo apt-get install libmodplug-dev

echo "Installing SDL2 and additional libraries..."
cd SDL2_image-2.6.3  # .../external/modules/SDL2_image-2.6.3
sudo mkdir build
cd build
sudo cmake ..
sudo make -j4
sudo make install
cd ..
cd ..

cd SDL2_mixer-2.6.3  # .../external/modules/SDL2_mixer-2.6.3
sudo mkdir build
cd build
sudo cmake ..
sudo make -j4
sudo make install
cd ..
cd ..

cd SDL2_ttf-2.20.2  # .../external/modules/SDL2_ttf-2.20.2
sudo mkdir build
cd build
sudo cmake ..
sudo make -j4
sudo make install
cd ..
cd ..

echo "Installing libyaml---"
cd yaml-lib
mkdir build
cd build
cmake ..
sudo make -j4
sudo make install
cd ..
cd ..

