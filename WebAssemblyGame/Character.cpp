#include "utils.h";
#include "Character.h"

Character::Character(int hp, int mp, int attack, int defense, int attackRange, std::string spriteFileName, TTF_Font* popUpFont, int x, int y, int speed, int xp)
	:hp(hp), mp(mp), baseAttack(attack), baseDefense(defense), attackRange(attackRange), attacking(false), currentHp(hp),
	currentMp(mp), sprite(Sprite(x, y, speed, spriteFileName)), xp(xp)
{
	SDL_Rect popUpPos{ sprite.getCenter().x, sprite.getCenter().y};
	damagePopUp = new PopUpHUDElement(popUpFont, "", "", popUpPos, SDL_Color {255, 0, 0, 255});
}

Character::~Character()
{
}

int Character::getHP() { return currentHp; }

int Character::getMP() { return currentMp; }

int Character::getAttack() { return baseAttack; }

int Character::getDefense() { return baseDefense; }

//int Character::getAttackRange() { return attackRange; }

bool Character::isAttacking() { return attacking; }

Sprite& Character::getSprite() {
	return sprite;
}

int Character::getXP() { return xp; }

void Character::attackEnemy(Character& target) {
	if (currentHp && target.getHP()) {
		if (!attacking) attacking = true;

		SDL_Point distance = distanceBetweenPoints(sprite.getCenter(), target.getSprite().getCenter());

		if (distance.x > (attackRange + sprite.getFrameWidth() / 2) || distance.y > (attackRange + sprite.getFrameHeight() / 2)) {
			sprite.moveTowards(target.getSprite().getCenter());
		}
		else {
			sprite.stop();
			if (!attackTime) {
				target.takeDamage(std::max(getAttack() - target.getDefense(), 0));
				SDL_Point targetPos{ target.getSprite().getCenter().x, target.getSprite().getCenter().y };
				target.damagePopUp->setPosition(targetPos);
				((PopUpHUDElement*)target.damagePopUp)->show();
				attackTime = 100;
				attacking = false;
			}
		}
	}
}

void Character::takeDamage(int damage) {
	damagePopUp->setValue(std::to_string(damage));
	currentHp = std::min(std::max(currentHp - damage, 0), hp);
}

void Character::update(int frame, SDL_Surface* screen) {
	if (attackTime) attackTime--;
	if (currentHp) {
		sprite.update(frame, screen);
	}
	damagePopUp->update(frame, "", screen);
}