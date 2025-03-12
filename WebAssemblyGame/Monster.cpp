#include "utils.h"
#include "Monster.h"

Monster::Monster(int hp, int mp, int attack, int defense, int attackRange, std::string spriteFileName, TTF_Font* popUpFont, int xp, int aggroRange, int x, int y, int speed)
	:Character(hp, mp, attack, defense, attackRange, spriteFileName, popUpFont, x, y, speed, xp), aggroRange(aggroRange)
{
}

Monster::~Monster()
{
}

void Monster::checkPlayerProximity(Character& player) {
	if (currentHp) {
		SDL_Point distance = distanceBetweenPoints(getSprite().getCenter(), player.getSprite().getCenter());

		if (distance.x < aggroRange && distance.y < aggroRange) attackEnemy(player);
		else getSprite().stop();
	}
}