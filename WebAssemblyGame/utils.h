#pragma once
#include <SDL.h>
#include <string>

SDL_Point distanceBetweenPoints(SDL_Point a, SDL_Point b);

std::string makeHUDString(std::string label, std::string value);