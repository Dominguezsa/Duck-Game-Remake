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

cp build/taller_server /usr/bin
cp build/taller_client /usr/bin
cp build/taller_editor /usr/bin
cp -r build/_deps /usr/bin

echo "Binaries moved to /usr/bin directory successfully!"

echo "Moving the game assets to /var/duck_game..."

cp -r ./client/data /var/duck_game

echo "Game assets moved to /var/duck_game directory successfully!"
echo " "
echo " "
echo "#######################################################"
echo "#         Installation completed successfully!        #"
echo "#######################################################"

