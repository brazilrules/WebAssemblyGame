# WebAssemblyGame
A game made to run in the WebAssembly platform Using the SDL library for graphics, audio and input.

Test Online: http://brazilrules.net/x-game/

Player sprite is © 2024 Leonardo Rodrigues All rights reserved.

# How to compile
* If you want to run locally, you have to download SLD 2.30.11, SDL_Image 2.8.4 and SDL_TTF 2.24.0 and point to their lib and include folders.
* If you want to run on WebAssembly, you have to download emsdk to install Emscripten and run the following command on the folder with the cpp files:
```em++ -s USE_SDL=2 --use-port=sdl2_image:formats=png -s USE_SDL_TTF=2 --preload-file assets Character.cpp HUDElement.cpp Monster.cpp Player.cpp PopUpHUDElement.cpp Skill.cpp Sprite.cpp StartGameState.cpp utils.cpp WebAssemblyGame.cpp -o game.html```
