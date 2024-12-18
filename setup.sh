echo "Updating package list..."
sudo apt update

if ! command -v gcc &> /dev/null; then
    echo "Installing GCC..."
    sudo apt install -y gcc
else
    echo "GCC is already installed."
fi

if ! command -v g++ &> /dev/null; then
    echo "Installing G++..."
    sudo apt install -y g++
else
    echo "G++ is already installed."
fi

if ! command -v cmake &> /dev/null; then
    echo "Installing CMake..."
    sudo apt install -y cmake
else
    echo "CMake is already installed."
fi

# Install SDL2 and related libraries
sudo apt install -y libsdl2-dev
cd external/modules/
sudo ./install_modules.sh
cd ..
cd ..

echo "Installing QtCreator..."
sudo apt install -y qtbase5-dev qt5-qmake qttools5-dev-tools qtmultimedia5-dev
#sudo apt install -y libqt5widgets5-dev libqt5gui5-dev

echo "All required dependencies have been installed."
