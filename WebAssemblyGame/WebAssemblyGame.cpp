// WebAssemblyGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define SDL_MAIN_HANDLED
#include "constants.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Sprite.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main()
{
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    //SDL_Surface* imageSurface = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL couldn't initialize! SDL Error: " << SDL_GetError() << std::endl;

        return EXIT_FAILURE;
    }
    

    window = SDL_CreateWindow("X Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cout << "Window couldn't be created! SDL Error: " << SDL_GetError() << std::endl;

        return EXIT_FAILURE;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Failed to initialize SDL_Image for PNG: " << IMG_GetError() << std::endl;
    }
        

    std::cout << "Hello World!\n";

    screenSurface = SDL_GetWindowSurface(window);

    SDL_Event e; 
    bool quit = false;

    //imageSurface = IMG_Load("gfx\\Player.png");
    Sprite player(0, 0, 5, std::string("gfx\\Player.png"));

    if (player.getImage() == NULL) {
        std::cout << "SDL could not load image: " << IMG_GetError() << std::endl;
        
        return EXIT_FAILURE;
    }

    SDL_Rect screenRect = { 400, 300, 0, 0 };
    SDL_Rect spriteRect = { 0, 0, 96, 96 };

    int frame = 0;
    Uint64 start;
    SDL_Point target = player.getCenter();
    bool clicked = false;

    while (!quit)
    {
        start = SDL_GetTicks64();
        
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            player.move(UP);
                            break;
                        case SDLK_RIGHT:
                            player.move(RIGHT);
                            break;
                        case SDLK_DOWN:
                            player.move(DOWN);
                            break;
                        case SDLK_LEFT:
                            player.move(LEFT);
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        player.stop(UP);
                        break;
                    case SDLK_RIGHT:
                        player.stop(RIGHT);
                        break;
                    case SDLK_DOWN:
                        player.stop(DOWN);
                        break;
                    case SDLK_LEFT:
                        player.stop(LEFT);
                        break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT:
                            target = {e.button.x, e.button.y};
                            clicked = true;
                            break;
                    }
                    break;
                case SDL_QUIT: 
                    quit = true;
                    break;
            }
        }

        if (clicked && (target.x != player.getCenter().x || target.y != player.getCenter().y)) {
            player.moveTowards(target);
        }
        else if (clicked) {
            player.stop();
            clicked = false;
        }

        player.update(frame);
        
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x8F, 0x8F));

        SDL_BlitSurface(player.getImage(), player.getSampleRect(), screenSurface, player.getPosition());

        SDL_UpdateWindowSurface(window);

        if (1000 / FPS > SDL_GetTicks64() - start) {
            SDL_Delay(1000 / FPS - (SDL_GetTicks64() - start));
        }

        frame++;
    }

    SDL_FreeSurface(screenSurface);
        
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
