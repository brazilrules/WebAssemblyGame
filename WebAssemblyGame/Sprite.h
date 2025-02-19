#pragma once

#include <SDL_surface.h>
#include <SDL_image.h>
#include <string>

enum DIRECTION {
	DOWN,
	LEFT,
	RIGHT,
	UP
};

enum MOVEMENT {
	STOPPED,
	WALKING,
	RUNNING
};

class Sprite
{
public:
	Sprite(int, int, int, std::string, int = 4, int = 4);
	~Sprite();

	SDL_Rect* getSampleRect();
	SDL_Rect* getPosition();

	int getX();
	void setX(int x);
	int getY();
	void setY(int y);
	SDL_Point getCenter();

	char getCurrentFrame();
	char getCurrentDirection();
	void setCurrentDirection(DIRECTION direction);
	int getFrameHeight();
	int getFrameWidth();

	int getSpeed();
	void setSpeed(int speed);
	bool isMoving();

	SDL_Surface* getImage();

	void move(DIRECTION direction);
	void moveTowards(SDL_Point target);
	void stop();
	void stop(DIRECTION direction);

	void update(int frame);


private:
	SDL_Rect sampleRect;
	SDL_Rect position;
	char frameCount;
	char currentFrame;
	char currentDirection;
	int speed;
	char xSpeed;
	char ySpeed;
	char moving;

	
	SDL_Surface* image;

};