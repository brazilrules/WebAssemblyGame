// WebAssemblyGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define SDL_MAIN_HANDLED
#include "constants.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "Sprite.h"
#include "Player.h"
#include "Monster.h"
#include "HUDElement.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

TTF_Font* font;

SDL_Event e;

Character* player;
Character* monster;

HUDElement* hudHP;
HUDElement* deathMessage;
HUDElement* winMessage;

int frame = 0;
SDL_Point target;
bool quit;
bool clicked;
bool holding;

void mainLoop() {
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                player->getSprite().move(UP);
                break;
            case SDLK_RIGHT:
                player->getSprite().move(RIGHT);
                break;
            case SDLK_DOWN:
                player->getSprite().move(DOWN);
                break;
            case SDLK_LEFT:
                player->getSprite().move(LEFT);
                break;
            case SDLK_r:
                player->takeDamage(-100);
                player->getSprite().setX(0);
                player->getSprite().setY(0);
                monster->takeDamage(-20);
                monster->getSprite().setX(SCREEN_WIDTH / 2);
                monster->getSprite().setY(SCREEN_HEIGHT / 2);
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                player->getSprite().stop(UP);
                break;
            case SDLK_RIGHT:
                player->getSprite().stop(RIGHT);
                break;
            case SDLK_DOWN:
                player->getSprite().stop(DOWN);
                break;
            case SDLK_LEFT:
                player->getSprite().stop(LEFT);
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (e.button.button) {
            case SDL_BUTTON_LEFT:
                target = { e.button.x, e.button.y };
                clicked = true;
                holding = true;
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch (e.button.button) {
            case SDL_BUTTON_LEFT:
                holding = false;
                break;
            }
            break;
        case SDL_QUIT:
            quit = true;
            break;
        }
    }

    if ((holding || player->isAttacking()) && monster->getSprite().isInBounds(target)) {
        player->attackEnemy(*monster);
        clicked = false;
    }

    if (clicked && (target.x != player->getSprite().getCenter().x || target.y != player->getSprite().getCenter().y)) {
        player->getSprite().moveTowards(target);
    }
    else if (clicked) {
        player->getSprite().stop();
        clicked = false;
    }

    ((Monster*)monster)->checkPlayerProximity(*player);



    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0x8F, 0x8F));

    player->update(frame, screenSurface);
    monster->update(frame, screenSurface);

    SDL_Rect textPos{ 200, 200, 400, 100 };

    hudHP->update(frame, std::to_string(player->getHP()), screenSurface);

    if (!player->getHP()) deathMessage->update(frame, "", screenSurface);

    if (!monster->getHP()) winMessage->update(frame, "", screenSurface);

    SDL_UpdateWindowSurface(window);

    frame++;
}

int main()
{

    std::cout << "Initializing SDL!" << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cout << "SDL couldn't initialize! SDL Error: " << SDL_GetError() << std::endl;

        return EXIT_FAILURE;
    }

    std::cout << "Initializing SDL_TTF!" << std::endl;
    if (TTF_Init() < 0) {
        std::cerr << "Failed to initialize SDL_TTF: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    std::cout << "Initializing SDL_IMG!" << std::endl;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Failed to initialize SDL_Image for PNG: " << IMG_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    
    std::cout << "Creating Window!" << std::endl;
    window = SDL_CreateWindow("X Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cout << "Window couldn't be created! SDL Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return EXIT_FAILURE;
    }

    std::cout << "Creating Font!" << std::endl;
    // Load a font
    font = TTF_OpenFont("assets/fonts/Ldfcomicsans-jj7l.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }   

    std::cout << "Creating Screen Surface!" << std::endl;

    screenSurface = SDL_GetWindowSurface(window);
 
    std::cout << "Creating Units!" << std::endl;
    player = new Player(100, 50, 10, 10, 10, 5, 5, 5, "assets/gfx/Player.png", font, std::vector<Skill>());
    monster = new Monster(20, 0, 30, 10, 5, "assets/gfx/Monster.png", font, 10, 150, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    std::cout << "Creating HUD Elements!" << std::endl;
    hudHP = new HUDElement(font, "HP", std::to_string(player->getHP()), SDL_Rect{ 10, 10 });
    deathMessage = new HUDElement(font, "You Died, Press R to try again.", "", SDL_Rect{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0,0 }, SDL_Color{ 255,0,0,255 });
    winMessage = new HUDElement(font, "You Won!, Press R to play again.", "", SDL_Rect{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0,0 }, SDL_Color{ 0,255,0,255 });

    if (player->getSprite().getImage() == NULL) {
        std::cout << "SDL could not load image: " << IMG_GetError() << std::endl;
        
        return EXIT_FAILURE;
    }

    if (monster->getSprite().getImage() == NULL) {
        std::cout << "SDL could not load image: " << IMG_GetError() << std::endl;

        return EXIT_FAILURE;
    }
    
    Uint64 start;
    target = player->getSprite().getCenter();
    quit = false;
    clicked = false;
    holding = false;
    std::cout << "Main Loop!" << std::endl;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else
    while (!quit)
    {
        start = SDL_GetTicks64();
        
        mainLoop();

        if (1000 / FPS > SDL_GetTicks64() - start) {
            SDL_Delay(1000 / FPS - (SDL_GetTicks64() - start));
        }

    }
#endif

    delete player;
    delete monster;
    delete hudHP;
    delete deathMessage;
    delete winMessage;

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
