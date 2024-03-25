# Building

This document contains information on how to build OceanPop for each supported platform.

For each platform the instrutions assume that the code has already been downloaded. When this isn't the case yet, head over to the [Github page](https://github.com/sharkwouter/oceanpop) to download the latest version of the code or clone it with the following git command:

```
git clone https://github.com/sharkwouter/oceanpop.git
```

## Ubuntu

On Ubuntu install the required packages with the following command:

```
sudo apt-get install cmake build-essential pkgconf libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libjsoncpp-dev
```

To build execute the following commands from a terminal in the directory where the code is found:

```
mkdir build
cd build
cmake ..
make
```

Then the game can be started with:

```
./oceanpop
```

Installing can be done with:

```
make install
```

## Windows

On Windows make sure [vcpkg](https://vcpkg.io/) and cmake are installed and in your PATH. Then install the following packages with it:

```
vcpkg install --triplet x64-windows-static sdl2 sdl2-image[libjpeg-turbo] sdl2-ttf sdl2-mixer[libvorbis,mpg123] jsoncpp
```

To build execute the following commands from a terminal in the directory where the code is found:

```
cmake -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static .
cmake --build . --config Release
cp Release/oceanpop.exe .
```

Then the game can be started through ``oceanpop.exe`.


## Playstation Vita

To build for the Playstation Vita latest version of the [VitaSDK](https://vitasdk.org/) needs to be installed.

To build execute the following commands from a terminal in the directory where the code is found:

```
mkdir vita
cd vita
cmake -DCMAKE_TOOLCHAIN_FILE="${VITASDK}/share/vita.toolchain.cmake" ..
make
```

The resulting ``oceanpop.vpk`` file can be copied to the Vita and installed with Vitashell. Then it can be started from the home screen.

## Playstation Portable

To build for the Playstation Portable latest version of the [PSPDEV SDK](https://github.com/pspdev/pspdev/releases/tag/latest) needs to be installed.

To build execute the following commands from a terminal in the directory where the code is found:

```
mkdir psp
cd psp
psp-cmake ..
make
```

The resulting ``EBOOT.PBP`` and ``assets`` directory can be copied into the ``PSP/GAME/oceanpop`` directory on the PSP memory card. If this directory does not exist yet, create it.


## Nintendo Wii

To build for the Nintendo Wii you need to install the needed SDL2 dependencies via [devkitPro](https://devkitpro.org/):

```
sudo dkp-pacman -S wii-sdl2 wii-sdl2_image wii-sdl2_mixer wii-sdl2_ttf
```

You will also need to build `json-cpp` by yourself, since it's not part of devkitPro. This can be done with the following commands:

```
# This assumes that you are located in a clean checkout of the json-cpp repo:
source /etc/profile.d/devkit-env.sh
cmake -S. -Bbuild \
          -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/Wii.cmake" \
          -DJSONCPP_WITH_TESTS=OFF
cd build
make
sudo make install
```

Once this is done, change the current directory to the oceanpop source tree and type
```
./platform/add-new-platform.sh wii 640x480
source /etc/profile.d/devkit-env.sh
cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/Wii.cmake"
cd build
make
```

The resulting ``oceanpop.dol`` and ``assets`` directory can be copied into the ``apps/oceanpop`` directory on the Wii SD card. If this directory does not exist yet, create it.
