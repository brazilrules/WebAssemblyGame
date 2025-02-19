#include "constants.h"
#include "Sprite.h"
//#include <math.h>

Sprite::Sprite(int x, int y, int speed, std::string imageFile, int frameCount, int dirCount) :
	frameCount(frameCount), currentFrame(0), currentDirection(DOWN), speed(speed), moving(STOPPED)
{
	image = IMG_Load(imageFile.c_str());
	position.x = x;
	position.y = y;
	sampleRect.x = 0;
	sampleRect.y = 0;
	sampleRect.h = image->h / dirCount;
	sampleRect.w = image->w / frameCount;
}

Sprite::~Sprite()
{
	SDL_FreeSurface(image);
}

SDL_Rect* Sprite::getSampleRect() { return &sampleRect; }

SDL_Rect* Sprite::getPosition() { return &position; }

int Sprite::getX() { return position.x; }
void Sprite::setX(int x) { position.x = x; }
int Sprite::getY() { return position.y; }
void Sprite::setY(int y) { position.y = y; }
SDL_Point Sprite::getCenter() {
	return SDL_Point{ position.x + sampleRect.w / 2, position.y + sampleRect.h / 2 };
}

char Sprite::getCurrentFrame() { return currentFrame; }
char Sprite::getCurrentDirection() { return currentDirection; }
void Sprite::setCurrentDirection(DIRECTION direction) { currentDirection = direction; }
int Sprite::getFrameHeight() { return sampleRect.h; }
int Sprite::getFrameWidth() { return sampleRect.w; }

int Sprite::getSpeed() { return speed; }
void Sprite::setSpeed(int speed) { speed = speed; }
bool Sprite::isMoving() { return moving; }

SDL_Surface* Sprite::getImage() { return image; }

void Sprite::move(DIRECTION direction) {
	switch (direction) {
		case DOWN:
			moving = WALKING;
			ySpeed = speed;
			break;
		case LEFT:
			moving = WALKING;
			xSpeed = -speed;
			break;
		case RIGHT:
			moving = WALKING;
			xSpeed = speed;
			break;
		case UP:
			moving = WALKING;
			ySpeed = -speed;
			break;
	}
}

void Sprite::moveTowards(SDL_Point target) {
	moving = WALKING;
	if (target.x > getCenter().x) {
		xSpeed = std::min(speed, target.x - getCenter().x);
	}
	else {
		xSpeed = -std::min(speed, getCenter().x - target.x);
	}

	if (target.y > getCenter().y) {
		ySpeed = std::min(speed, target.y - getCenter().y);
	}
	else {
		ySpeed = -std::min(speed, getCenter().y - target.y);
	}
}

void Sprite::stop() {
	xSpeed = 0;
	ySpeed = 0;

	moving = STOPPED;
}

void Sprite::stop(DIRECTION direction) {
	if (direction == LEFT || direction == RIGHT) xSpeed = 0;
	if (direction == UP || direction == DOWN) ySpeed = 0;

	if (!xSpeed && ! ySpeed) moving = STOPPED;
}

void Sprite::update(int frame) {
	if (moving) {
		if (std::abs(xSpeed) > std::abs(ySpeed)) {
			currentDirection = xSpeed > 0 ? RIGHT : LEFT;
		}
		else {
			currentDirection = ySpeed > 0 ? DOWN : UP;
		}

		sampleRect.y = currentDirection * sampleRect.h;

		if (frame % (FPS/frameCount) == 0) {
			currentFrame = currentFrame < (frameCount - 1) ? currentFrame + 1 : 0;
			sampleRect.x = currentFrame * sampleRect.w;
			
		}
		
		position.x += xSpeed;
		position.y += ySpeed;
	}
	else {
		sampleRect.x = 0;
	}
}