#!/bin/bash
echo " "
echo " "
echo "#######################################################"
echo "#               Installation started...               #"
echo "#######################################################"

echo " "
echo " "
echo "#######################################################"
echo "#               Running setup script...               #"
echo "#######################################################"
echo " "
echo " "

chmod +x setup.sh
./setup.sh
echo " "
echo " "
echo "#######################################################"
echo "#         Setup script completed successfully!        #"
echo "#######################################################"
echo " "
echo " "
echo "#######################################################"
echo "#              Running build script...                #"
echo "#######################################################"
echo " "
echo " "

chmod +x build.sh
./build.sh

echo " "
echo " "
echo "#######################################################"
echo "#         Build script completed successfully!        #"
echo "#######################################################"
echo " "
echo " "
echo "#######################################################"
echo "#          Moving the binaries to /usr/bin...         #"
echo "#######################################################"
echo " "
echo " "

cp build/duck_game_server /usr/bin
cp build/duck_game /usr/bin
cp build/map_editor /usr/bin
cp -r build/_deps /usr/bin

echo "Binaries moved to /usr/bin directory successfully!"

echo "Moving the game assets to /var/duck_game..."

mkdir -p /var/duck_game
cp -r ./resources/* /var/duck_game
chmod 700 /var/duck_game/
chmod 777 /var/duck_game/maps/

echo "Game assets moved to /var/duck_game directory successfully!"
echo " "
echo " "
echo "#######################################################"
echo "#         Installation completed successfully!        #"
echo "#######################################################"

