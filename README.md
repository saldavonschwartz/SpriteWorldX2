# SpriteWorldX2
An SDL2 port of SpriteWorld / SpriteWorldX.

### About:

This is an SDL2-compatible port of [https://sourceforge.net/projects/spriteworld/]Spriteworld / [https://sourceforge.net/projects/spriteworldx/](SpriteWorldX). It was done as a somewhat quick stepping stone in getting a game based on the original SpriteWorld to run in current hardware and SDL2.

SpriteWorldX2 is released under MIT license. It's ancestors were released under zlipb.

### Known Limitations:

* Particles are disabled.
* The library largely depends on `SDL_Surface`, which back in SDL1 could be hardware-accelerated. SDL2 surfaces however are software-only, so the performance is not mind-blowing. To improve performance, eventually either the library should be based entirely on GL or a substantial part of the codebase whould be rewritten to at least use `SDL_Texture` most of the time.

### Dependencies:

* [https://www.libsdl.org/download-2.0.php](SDL2)
* [https://www.libsdl.org/projects/SDL_ttf/](SDL2_ttf)
* [https://www.libsdl.org/projects/SDL_image/](SDL2_image)
* OpenGL

### How to Build:
Compile all sources and link against the frameworks / libs mentioned above. (make / cmake might be coming).

**Acknowledgements:**

Thanks to [https://github.com/afb](Anders F Björklund) and [https://www.bhencke.com/](Ben Hencke) and the other authors of the original version of SpriteWorld and SpriteWorld X on which this SDL 2 port was based.
