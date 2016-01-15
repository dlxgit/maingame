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

enum EnemyType
{
	COMMON,
	AXE,
	MUMMY,
	FIRE,
	//BOSS.
};

enum BossState
{
	SPAWNING,
	MOVING,
	STOMPING,
	SHOOTING,
	SHRINKING,
	DYING
};

enum BossEvent
{
	MOVE_OUT,
	MOVE_TO,
	CHARGE,
	SHOOT,
	STOMP_FOR_ZOMBIES,
};

enum EnemyState
{
	NOTSPAWNED,  //cant move (raising from the ground)
	ACTIVE,
	DEAD,      //cant move (health = 0)
	EXPLODED  //if animation has finished (for deleting from list)
};

enum HeroState
{
	DAMAGED,
	SMASHED,
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
	FINISH,
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
	SODA
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
const Vector2i WINDOW_SIZE = { 800,600 };
const Vector2i TILEMAP_SIZE = { 50,27};
const int STEP_TILE = 64;
const int STEP = 48;

const int HEIGHT_MAP = 30;
const int WIDTH_MAP = 76;
const Vector2i LEVEL_BOSS_SIZE = { 20, 12};

const int LEVEL_BOSS_MAX_LOOT_QUANTITY = 2;

//boss
const int BOSS_MAX_HEALTH = 10000;
const float BOSS_STAGE_TIME = 10.f;

const float STEP_BOSS = 3.f;
const float STEP_BOSS_FOLLOW = 5.f;
const float STEP_BOSS_CHARGE = 12.f;

const float BOSS_DIR_CHANGE_TIME = 2.f;

const float BOSS_EVENT_COME_SHOOT_DISTANCE = 300;


const float BOSS_MILK_MAX_DISTANCE = 0;   //

const float BOSS_SPAWN_ENEMY_INTERVAL = 1.f;   //time for 1 enemy spawn while stomping

const float BOSS_FOLLOW_FOR_SHOOT_TIME = 7.f;


const float BOSS_SHOT_COOLDOWN = 1.f;

const float BOSS_SHOOTING_COOLDOWN = 0.3f;


const Vector2f LEVEL1_CENTER_POS = { float(12 * 64), float(6 * 64) };
const Vector2f LEVEL1_ZOMBIE_SPAWN_SPOTS[4] = { {11 * 64, 5 * 64}, { 13 * 64, 5 * 64 }, { 11 * 64, 7 * 64 }, { 13 * 64, 7 * 64 } };

//boss_bar
const Vector2f BOSS_BAR_POSITION = { float(WINDOW_SIZE.x / 2) - 132, 40.f };
const Vector2f BOSS_INDICATOR_POSITION = { float(WINDOW_SIZE.x / 2) - 132 + 9, 46.f };



//minimap
const Vector2f MINIMAP_START_POS = { 500.f,100.f };
const float MINIMAP_DISTANCE_SCALE = 5.f;
const float MINIMAP_CIRCLE_RADIUS = 50.f;

//movespeed of objects
const float STEP_HERO = 5.f;
const float STEP_HERO_BEAST = 7.f;
const float STEP_ZOMBIE = 2.f;
const float STEP_ZOMBIE_ACTIVE = 3.f;
const float STEP_SHOT = 12.f;
const float STEP_GRENADE = 5.f;
const float STEP_ENEMY_AXE = 1.f;


//coefficients
const float ZOMBIE_INCREASE_RATE_AXE = 2;


//enemies
const int ENEMY_AXE_ATTACK_DAMAGE = 20;
const float ENEMY_AXE_SPAWN_DELAY = 3.f;

const float ENEMY_AXE_ATTACK_COOLDOWN = 3.f;
const float ENEMY_AXE_ACTION_COOLDOWN = 3.f;
const float ENEMY_AXE_THROWING_MAX_TIME = 2.f;
const float ENEMY_AXE_THROWING_SPEED = 5.f;
const float ENEMY_AXE_CHARGE_TIME = 3.f;
const float ENEMY_AXE_CHARGE_COOLDOWN = 8;
const float ENEMY_AXE_CHARGE_SPEED_INCREASE_PER_LOOP = 0.2f;

const float ENEMY_AXE_THROWING_STEP_PER_LOOP = 5;

//damage
const float BOSS_SMASH_DAMAGE = 30.f;
const float ZOMBIE_DAMAGE = 30.f;
const int DMG_ITEM[8] = { 0, 100, 100, 0, 0, 0, 200, 100 };
const int HERO_BEAST_DAMAGE = 150;

const float THROWING_AXE_DAMAGE = 15.f;
const float BOSS_MILK_DAMAGE = 7.f;

const float HERO_BEAST_DAMAGE_RESISTANCE = 0.2f;


//distance
const int SHOT_MAX_DISTANCE = 400;
const int ZOMBIE_VISION_DISTANCE = 300;

//health of objects
const int ZOMBIE_MAX_HP = 100;
const int HP_PER_DRINK = 40;
const int NPC_MAX_HEALTH = 50;

//time 
const float ITEM_REUSE_COOLDOWN[8] = { 0.35f, 0.35f,0.15f,0.15f,0.15f,0.15f, 0.1f };
const int BEAST_MAX_TIME = 12;
const float HERO_BEAST_ATTACK_TIME = 0.3f;

//heroTime
const float HERO_SMASH_DURATION = 1.f;


//const float WEAPON_RELOAD_TIME = 1.5f;
const float WEAPON_RELOAD_TIME = 0.1f;

const int TIME_GAME_STEP = 10;
const float SCREEN_UPDATE_TIME = 15.f;
//const float SCREEN_UPDATE_TIME = 150.f;
const float GRENADE_MAX_TIME = 1.f;  //throwtime
const int ZOMBIE_DIR_CHANGE_TIME = 5;

//item quantity settings
const int AMMO_PACKS = 4;
const int MAX_AMMO[8] = { 1,12,30,1,1,1,1,1 };

//counts
const int MAX_NUMBER_OF_NEIGHBORS = 8;

//math
const float DIAGONAL_STEP = 0.66f;  //

//names
const sf::String ITEM_NAMES[8] = { "drink", "pistol", "rifle", "ammo", "key", "mixture", "grenade", "soda" };