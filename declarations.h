#pragma once
#include <SFML\Graphics.hpp>


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
enum ZombieState
{
	NOTSPAWNED,  //cant move (raising from the ground)
	ACTIVE,
	DEAD,      //cant move (health = 0)
	EXPLODED  //if animation has finished (for deleting from list)
};
enum HeroState
{
	DAMAGED,
	NORMAL,
	TRANSFORMING,  //changing from normal to beast
	BEAST
};

enum ShotType
{
	BULLET,
	USED_GRENADE
};

enum GameState
{
	START_GAME,
	PLAY,
	LEVEL_FINISH,
	END_GAME,
	RESTART,
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

enum NpcType
{
	PHOTOGRAPHS,
	BABY,
	TEACHER,
	GIRL,
	DOG,
	SOLDIER,
	SEARCHER,
	COOK,
};

enum NpcState
{
	NPC_LIVING,
	NPC_DEAD,
	NPC_SURVIVED,
};


//movespeed of objects
const int STEP_HERO = 5;
const int STEP_HERO_HULK = 7;
const int STEP_ZOMBIE = 2;
const int STEP_ZOMBIE_ACTIVE = 3;
const int STEP_SHOT = 12;
const int STEP_GRENADE = 5;

//damage
const int ZOMBIE_DAMAGE = 30;
const int DMG_ITEM[7] = { 0, 35, 50,0, 0,0,100 };
const int HERO_BEAST_DAMAGE = 100;

//distance
const int SHOT_MAX_DISTANCE = 400;
const int ZOMBIE_VISION_DISTANCE = 300;

//health of objects
const int ZOMBIE_MAX_HP = 100;
const int HP_PER_DRINK = 30;
const int NPC_MAX_HEALTH = 50;

//time 
const int BEAST_MAX_TIME = 30;
const float HERO_BEAST_ATTACK_TIME = 0.7;
const float WEAPON_RELOAD_TIME = 1.5;
const int TIME_GAME_STEP = 10;
const float SCREEN_UPDATE_TIME = 15;
const float GRENADE_MAX_TIME = 1;  //throwtime
const int ZOMBIE_DIR_CHANGE_TIME = 5;

//item quantity settings
const int AMMO_PACKS = 4;
const int MAX_AMMO[7] = { 1,12,30,1,1,1,1 };

//counts
const int MAX_NUMBER_OF_NEIGHBORS = 8;


//names
const sf::String ITEM_NAMES[7] = { "drink", "pistol", "rifle", "ammo", "key", "mixture", "grenade" };