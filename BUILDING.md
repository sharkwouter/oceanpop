# Building

This document contains information on how to build Match Theory for each supported platform.

For each platform the instrutions assume that the code has already been downloaded. When this isn't the case yet, head over to the [Github page](https://github.com/match-theory/match-theory) to download the latest version of the code or clone it with the following git command: 

```
git clone https://github.com/match-theory/match-theory.git
```

## Ubuntu

On Ubuntu install the required packages with the following command:

```
sudo apt-get install cmake pkgconf g++ libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libjsoncpp-dev
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
./match-theory
```

## Windows

On Windows make sure [vcpkg](https://vcpkg.io/) and cmake are installed and in your PATH. Then install the following packages with it:

```
vcpkg install --triplet x64-windows sdl2 sdl2-image[libjpeg-turbo] sdl2-ttf sdl2-mixer[libvorbis,mpg123] jsoncpp
```

To build execute the following commands from a terminal in the directory where the code is found:

```
cmake -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake .
cmake --build .
```

Then the game can be started through ``match-theory.exe`` in the resulting ``Debug`` directory.


## Playstation Vita

To build for the Playstation Vita latest version of the [VitaSDK](https://vitasdk.org/) needs to be installed.

To build execute the following commands from a terminal in the directory where the code is found:

```
mkdir vita
cd vita
cmake -DCMAKE_TOOLCHAIN_FILE="${VITASDK}/share/vita.toolchain.cmake" ..
make
```

The resulting ``match-theory.vpk`` file can be copied to the Vita and installed with Vitashell. Then it can be started from the home screen.

## Playstation Portable

**Note**: The PSP version is currently not working, because of some bugs in the SDL2 port.

To build for the Playstation Portable latest version of the [PSPDEV SDK](https://github.com/pspdev/pspdev/releases) needs to be installed.

To build execute the following commands from a terminal in the directory where the code is found:

```
mkdir psp
cd psp
cmake -DCMAKE_TOOLCHAIN_FILE=$PSPDEV/psp/share/pspdev.cmake ..
make
```

The resulting ``EBOOT.PBP`` and ``assets`` directory can be copied into the ``PSP/GAME/Match-Theory`` directory on the PSP memory card. If this directory does not exist yet, create it.
