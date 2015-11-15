#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

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

enum NameItem
{
	DRINK,
	PISTOL,
	RIFLE,
	AMMO,
	KEY,
	MIXTURE,
	GRENADE,
};

enum HeroState
{
	DAMAGED,
	NORMAL,
	TRANSFORMING,  //changing from normal to beast
	BEAST
};

enum ZombieState
{
	NOTSPAWNED,  //cant move (raising from the ground)
	ACTIVE,
	DEAD,      //cant move (health = 0)
	EXPLODED  //if animation has finished (for deleting from list)
};

//map
const int width = 640;
const int height = 480;
const int HEIGHT_MAP = 30;
const int WIDTH_MAP = 76;
const int STEP = 32;

const int TIME_GAME_STEP = 10;


//movespeed
const int STEP_HERO = 6;
const int STEP_HERO_HULK = 8;
const int STEP_ZOMBIE = 2;
const int STEP_ZOMBIE_ACTIVE = 2;
const int STEP_SHOT = 12;

const int ZOMBIE_DAMAGE = 30;
const int ZOMBIE_MAX_HP = 100;
const int ZOMBIE_VISION_DISTANCE = 300;

const int HP_PER_DRINK = 30;

const int SHOT_MAX_DISTANCE = 400;

const int WEAPON_RELOAD_TIME = 3;

const int BEAST_MAX_TIME = 30;
const int HERO_BEAST_DAMAGE = 100;
const float HERO_BEAST_ATTACK_TIME = 0.7;

const int DMG_ITEM[7] = { 0, 35, 50,0, 0,0,100 };
const int AMMO_PACKS = 4;
const int MAX_AMMO[7] = { 1,12,30,1,1,1,1 };

const string ITEM_NAMES[7] = { "drink", "pistol", "rifle", "ammo", "key", "mixture", "grenade"};