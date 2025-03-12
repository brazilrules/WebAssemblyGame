#include "utils.h";

SDL_Point distanceBetweenPoints(SDL_Point a, SDL_Point b) {
	int x = 0, y = 0;
	if (a.x > b.x) {
		x = a.x - b.x;
	}
	else {
		x = b.x - a.x;
	}

	if (a.y > b.y) {
		y = a.y - b.y;
	}
	else {
		y = b.y - a.y;
	}
	
	return SDL_Point{ x, y };
}

std::string makeHUDString(std::string label, std::string value) {
	std::string text = label;
	if (!label.empty() && !value.empty()) text += ": ";
	if (!value.empty()) text += value;
	return text;
}