# Match Theory
A portable open source match 3 game.

## Building

Regardless of platform, building the game starts with cloning the git repo with the following command:

```
git clone https://github.com/JoseTomasTocino/freegemas.git
```

### Linux

On Linux make sure the following packages are installed:

```
libsdl2-dev libsdl2-image-dev
```

After cloning use the following commands to build for Linux:

```
cd match-theory
mkdir build
cd build
cmake ..
make
```

Then the game can be run with:

```
./match-theory
```

### Playstation Vita

Make sure the [VitaSDK](https://vitasdk.org/) is installed for building for the Vita.

After cloning use the following commands to build for the Vita:

```
cd match-theory
mkdir vita
cd vita
cmake -DCMAKE_TOOLCHAIN_FILE="${VITASDK}/share/vita.toolchain.cmake" ..
make
```

The copy the ``match-theory.vpk`` file to the Vita and install it to be able to play.

## Attribution
Thanks to [Buch](https://opengameart.org/users/buch) for making the gem sprites used.

### assets/images/background.jpg
Photo by Pixabay from Pexels
