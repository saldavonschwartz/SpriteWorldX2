# SpriteWorldX2
An SDL2 port of SpriteWorld / SpriteWorldX.


### About:

This is an SDL2-compatible port of [SpriteWorld](https://sourceforge.net/projects/spriteworld/) / [SpriteWorldX](https://sourceforge.net/projects/spriteworldx/). It was done as a somewhat quick stepping stone in getting a game based on the original SpriteWorld to run in current hardware and SDL2.

SpriteWorldX2 is released under MIT license. It's ancestors were released under zlib.

### Known Limitations:

* Particles are disabled.

### Dependencies:

* [SDL2<sup>1</sup>](https://www.libsdl.org/download-2.0.php)
* [SDL2_ttf<sup>1</sup>](https://www.libsdl.org/projects/SDL_ttf/)
* [SDL2_image<sup>1</sup>](https://www.libsdl.org/projects/SDL_image/)


<sup>1</sup>: OSX: automatically downloaded by cmake if paths are not specified. Windows: you need to manually download and set their paths.


### How to Build:
The project uses camke to build (OSX only as of now). So you can do:

```
git clone <repo url>
cd <local-repo>
mkdir build
cd build
cmake -G <generator> ..
<open Xcode project and build>
```

**Acknowledgements:**

Thanks to [Anders F Björklund](https://github.com/afb), [Ben Hencke](https://www.bhencke.com/) and the other authors of the original version of SpriteWorld and SpriteWorld X on which this SDL 2 port was based.
