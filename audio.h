#pragma once
#include "SFML/Audio.hpp"
#include "declarations.h"

struct AudioResources
{
	Music menu;
	Music level0;
	Music level1;
	Music level2;
	SoundBuffer loot;
	Sound lootSound;
	SoundBuffer npcSurvive;
	Sound npcSurviveSound;
	SoundBuffer npcDeath;
	Sound npcDeathSound;
	SoundBuffer shot;
	Sound shotSound;
	SoundBuffer beastAttack;
	Sound beastAttackSound;
	SoundBuffer finish;
	Sound finishSound;
	SoundBuffer takenDamage;
	Sound takenDamageSound;
	SoundBuffer enemyExplosion;
	Sound enemyExplosionSound;
	//grenade
};

void InitializeSoundResouces(AudioResources & audio);