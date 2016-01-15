#include "audio.h"

void InitializeSoundResouces(AudioResources & audio)
{
	audio.level0.openFromFile("resources/sounds/level0.ogg");
	audio.level1.openFromFile("resources/sounds/level1.ogg");
	audio.level2.openFromFile("resources/sounds/level2.ogg");
	//audio.finish.o
	audio.menu.openFromFile("resources/sounds/menu.ogg");
	

	audio.level0.setLoop(true);
	audio.level1.setLoop(true);
	audio.level2.setLoop(true);

	audio.beastAttack.loadFromFile("resources/sounds/beast_attack.wav");
	audio.loot.loadFromFile("resources/sounds/loot.wav");
	audio.npcDeath.loadFromFile("resources/sounds/npc_death.wav");
	audio.npcSurvive.loadFromFile("resources/sounds/npc_survive.wav");
	audio.shot.loadFromFile("resources/sounds/shot.wav");
	audio.finish.loadFromFile("resources/sounds/finish.ogg");
	audio.takenDamage.loadFromFile("resources/sounds/taken_damage.ogg");
	audio.enemyExplosion.loadFromFile("resources/sounds/enemy_explosion.ogg");

	audio.beastAttackSound.setBuffer(audio.beastAttack);
	audio.lootSound.setBuffer(audio.loot);
	audio.npcDeathSound.setBuffer(audio.npcDeath);
	audio.npcSurviveSound.setBuffer(audio.npcSurvive);
	audio.shotSound.setBuffer(audio.shot);
	audio.finishSound.setBuffer(audio.finish);
	audio.takenDamageSound.setBuffer(audio.takenDamage);
	audio.enemyExplosionSound.setBuffer(audio.enemyExplosion);
}