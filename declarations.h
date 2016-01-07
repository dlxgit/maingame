#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "iostream"

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
	LIVING,
	KILLED,
	SURVIVED,
};


//spawn settings
const float ZOMBIE_SPAWN_RADIUS_COLLISION = 80.f;


//map
const Vector2i WINDOW_SIZE = { 1280,1024 };
const Vector2i TILEMAP_SIZE = { 50,27};
const int HEIGHT_MAP = 30;
const int WIDTH_MAP = 76;
const int STEP_TILE = 64;
const int STEP = 48;


//minimap
const Vector2f MINIMAP_START_POS = { 700.f,700.f };
const float MINIMAP_DISTANCE_SCALE = 5.f;
const float MINIMAP_CIRCLE_RADIUS = 50.f;

//movespeed of objects
const float STEP_HERO = 5.f;
const float STEP_HERO_HULK = 7.f;
const float STEP_ZOMBIE = 2.f;
const float STEP_ZOMBIE_ACTIVE = 3.f;
const float STEP_SHOT = 12.f;
const float STEP_GRENADE = 5.f;

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
const float ITEM_REUSE_COOLDOWN[7] = { 0.35f, 0.35f,0.15f,0.15f,0.15f,0.15f };
const int BEAST_MAX_TIME = 17;
const float HERO_BEAST_ATTACK_TIME = 0.7f;
const float WEAPON_RELOAD_TIME = 1.5f;
const int TIME_GAME_STEP = 10;
const float SCREEN_UPDATE_TIME = 15.f;
//const float SCREEN_UPDATE_TIME = 150.f;
const float GRENADE_MAX_TIME = 1.f;  //throwtime
const int ZOMBIE_DIR_CHANGE_TIME = 5;

//item quantity settings
const int AMMO_PACKS = 4;
const int MAX_AMMO[7] = { 1,12,30,1,1,1,1 };

//counts
const int MAX_NUMBER_OF_NEIGHBORS = 8;

//math
const float DIAGONAL_STEP = 0.66f;  //

//names
const sf::String ITEM_NAMES[7] = { "drink", "pistol", "rifle", "ammo", "key", "mixture", "grenade" };