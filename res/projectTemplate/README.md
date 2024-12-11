# Charlie2D-Project-Template
The official Charlie2D Project Template

# Compile Instructions
## Linux
Install cmake, sdl2, sdl2_ttf, sdl2_mixer, sdl2_gfx, sdl2_image
Note: When you distriute your game the users will also have to install those packages (besides cmake of course), I recomend you don't distribute linux builds for this reason

Compile
```
mkdir build
cd build
cmake ..
make
./index
```
## Windows
Download and install Cmake ([Downloads](https://cmake.org/download/))

Download and install the Mingw Compiler for 
x86_64 ([Downloads](https://github.com/niXman/mingw-builds-binaries/releases))

Extract and add the bin folder to your PATH env variable ([Guide](https://www.rose-hulman.edu/class/csse/resources/MinGW/installation.htm))

Compile
```
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
cmake --build .
.\index.exe
```
