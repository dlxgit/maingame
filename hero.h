#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

enum Direction
{
	NONE,
	UP,
	UPRIGHT,
	RIGHT,
	DOWNRIGHT,
	DOWN,
	DOWNLEFT,
	LEFT,
	UPLEFT,
	FOLLOW
};

enum HeroState
{
	DAMAGED,
	NORMAL,
	TRANSFORMING,  //changing from normal to beast
	BEAST
};

struct Hero
{
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	Direction dir;
	Direction dirLast;
	int slotNo;
	int nSlots;		//total slots in inventory
	int health;
	float currentFrame;
	HeroState state;
	bool isBeastAttack;
	float beastTimer;
	float lastAttackTime;
	bool isAmmo;
	float lastReloadTime;
	bool isReloading;
};

void InitializeHero(Hero & hero)
{
	hero.slotNo = 0;
	hero.nSlots = 1;
	hero.health = 100;
	hero.texture.loadFromFile("images/hero.png");
	hero.sprite.setTexture(hero.texture);
	hero.sprite.setTextureRect(IntRect(4, 4, 32, 32));
	hero.dir = NONE;
	hero.dirLast = DOWN;
	hero.currentFrame = 0;
	hero.sprite.setPosition(6 * 32, 9 * 32);  //start position
	hero.state = NORMAL;
	hero.isBeastAttack = false;
	hero.lastAttackTime = 0;
	hero.lastReloadTime = 0;
}