#pragma once
#include "enemy.h"


void ComputeThrownItemPosition(Sprite & sprite, Direction dir, float time, bool isHero)
{
	Vector2f startPos = sprite.getPosition();
}

Throwing CreateThrowing(Sprite & object, Sprite & target, string type,Sprite & throwingSprite, float & time)
{
	Throwing throwing;

	Vector2f distance = ComputeDistanceBetweenSprites(object, target);

	if (type == "axe")
	{
		throwing.maxTime = ENEMY_AXE_THROWING_MAX_TIME;
		throwing.cooldown = ENEMY_AXE_ACTION_COOLDOWN;
	}
	else if (type == "grenade")
	{

	}
	else if (type == "soda")
	{

	}
	else if (type == "milk")
	{
		throwing.maxTime = 3.f;
		throwing.cooldown = BOSS_SHOT_COOLDOWN;
	}

	throwing.name = type;

	float dist = sqrt(pow(distance.x, 2) + pow(distance.y, 2));

	float coeff;

	if (distance.x >= distance.y)
	{
		throwing.stepPerLoop = { 1, distance.y / distance.x };
		//coeff = sqrt(pow(dist, 2) - pow(distance.y, 2));
	}
	else
	{
		throwing.stepPerLoop = { distance.x / distance.y, 1 };
		//coeff = sqrt(pow(dist, 2) - pow(distance.x, 2));
	}
	
	throwing.stepPerLoop = { throwing.stepPerLoop.x * ENEMY_AXE_THROWING_STEP_PER_LOOP, throwing.stepPerLoop.y * ENEMY_AXE_THROWING_STEP_PER_LOOP };

	if (object.getPosition().x > target.getPosition().x)
	{
		throwing.stepPerLoop.x = -throwing.stepPerLoop.x;
	}
	if (object.getPosition().y > target.getPosition().y)
	{
		throwing.stepPerLoop.y = -throwing.stepPerLoop.y;
	}

	//throwing.dir = dir;
	throwing.sprite = throwingSprite;
	
	throwing.startPos = object.getPosition();

	throwing.sprite.setPosition(throwing.startPos);
	throwing.startTime = time;
	throwing.currentFrame = 0;
	return throwing;
}

void SpawnEnemy(vector<Enemy> & zombieList, float time, int posX, int posY, Sprites & sprites, const EnemyType & type)
{
	Enemy enemy;

	enemy.pos.x = posX;
	enemy.pos.y = posY;

	enemy.health = ZOMBIE_MAX_HP;

	//enemy.texture = texture_zombie;
	enemy.type = type;


	enemy.currentFrame = 0;
	enemy.attack_time = 0;
	enemy.dir = NONE;
	enemy.dirLast = NONE;
	enemy.follow = false;
	enemy.isNear = false;

	if (enemy.type == COMMON)
	{
		enemy.state = NOTSPAWNED;
		enemy.sprite = sprites.zombie;
		enemy.sprite.setTextureRect(IntRect(5, 5, 30, 30));
		enemy.spawnDelay = 0;

	}
	else if (enemy.type == AXE)
	{
		enemy.state = NOTSPAWNED;
		enemy.sprite = sprites.axe_enemy;
		enemy.sprite.setTextureRect(IntRect(29, 42, 22, 31));
		enemy.spawnDelay = ENEMY_AXE_SPAWN_DELAY;
		enemy.attackCooldown = ENEMY_AXE_ATTACK_COOLDOWN;
	}

	enemy.spawnTime = time;
	enemy.dirChangeTime = 0;

	enemy.isAttack = false;
	enemy.isAction = false;
	enemy.last_action_time = 0;

	enemy.sprite.setPosition(posX, posY);
	zombieList.push_back(enemy);
};

void CheckNpcDeath(vector<Npc> & npcList, Enemy & enemy)
{
	Vector2f zombieCenter = GetSpriteCenter(enemy.sprite);

	for (vector<Npc>::iterator npc = npcList.begin(); npc != npcList.end();)
	{
		if (npc->state == LIVING)  //if loot.item.center contains heroSprite  -> add new item in inventory
		{
			Vector2f npcCenter = GetSpriteCenter(npc->sprite);
			//cout << (abs(zombieCenter.x - npcCenter.x)) << " CENTER " << abs(zombieCenter.y - npcCenter.y) << endl;
			if ((abs(zombieCenter.x - npcCenter.x) < 35) && (abs(zombieCenter.y - npcCenter.y)) < 35)
			{
				npc->state = KILLED;
			}
		}
		npc++;
	}
};


Direction ComputeRandDir(Enemy & enemy)
{
	Direction dir = enemy.dir;

	switch (dir)
	{
	case UP:
		dir = DOWN;
		break;
	case RIGHT:
		dir = LEFT;
		break;
	case DOWN:
		dir = UP;
		break;
	case LEFT:
		dir = RIGHT;
		break;
	}
	return dir;
};

void CheckEnemyDir(Enemy & enemy, float &time)
{
	if (enemy.follow == false)
	{
		if (enemy.dirChangeTime == 0)
		{
			//cout << " 1 IS " << endl;
			enemy.dir = ComputeRandDir(enemy);
		}
		if (time - enemy.dirChangeTime > ZOMBIE_DIR_CHANGE_TIME)
		{
			//cout << " 2 IS " << endl;
			enemy.dirChangeTime = 0;
		}
	}
};

void CheckEnemyFollow(Enemy & enemy, Hero & hero)
{
	if (abs(enemy.pos.x - hero.pos.x) > ZOMBIE_VISION_DISTANCE || abs(enemy.pos.y - hero.pos.y) > ZOMBIE_VISION_DISTANCE)
	{
		enemy.follow = false;
	}
	if (enemy.follow == false)
	{
		if (abs(enemy.pos.x - hero.pos.x) < ZOMBIE_VISION_DISTANCE && abs(enemy.pos.y - hero.pos.y) < ZOMBIE_VISION_DISTANCE)
		{
			enemy.follow = true;
		}
	}
};

//should i store it in zombieList>? if i can just keep it here (return true)
bool IsEnemyNearHero(Hero & hero, Enemy & enemy)
{
	//comparing distance between two nearest points of hero and zombie sprites    to define is zombie near or not
	//TODO: ref

	Vector2f zombieCenter = GetSpriteCenter(enemy.sprite);
	Vector2f heroCenter = GetSpriteCenter(hero.sprite);

	int dx_max = 20;
	int dy_max = 20;
	int distanceX = 30;
	int distanceY = 30;

	//if hero and zombie sprites are very close - allow Beast to attack
	if ((abs(zombieCenter.x - heroCenter.x) < distanceX) && (abs(zombieCenter.y - heroCenter.y) < distanceY))
	{
		return true;
	}

	switch (hero.dirLast)
	{
	case UP:
		if (abs(heroCenter.x - zombieCenter.x) < dx_max && (heroCenter.y - zombieCenter.y >= 0 && heroCenter.y - zombieCenter.y < distanceY))
		{
			return true;
		}
		break;
	case RIGHT: case UPRIGHT: case DOWNRIGHT:
		if ((zombieCenter.x - heroCenter.x >= 0 && zombieCenter.x - heroCenter.x < distanceX) && (abs(heroCenter.y - zombieCenter.y) < dy_max))
		{
			return true;
		}
		break;
	case DOWN:
		if (abs(zombieCenter.x - heroCenter.x) < dx_max && (zombieCenter.y - heroCenter.y >= 0 && zombieCenter.x - heroCenter.x < distanceY))
		{
			return true;
		}
		break;
	case LEFT: case UPLEFT: case DOWNLEFT:
		if ((heroCenter.x - zombieCenter.x >= 0 && zombieCenter.x - heroCenter.x < distanceX) && (abs(heroCenter.x - zombieCenter.x) < dy_max))
		{
			return true;
		}
		break;
	}
	return false;
};

void UpdateEnemyAttack(Hero & hero, Enemy & enemy, const float & time)
{
	//add correct xyHero xyZombie for detecting meleeAttack
	//TODO: change mechanics of zombie attack(depends on side from which it attacks), same for beast attack
	if ((abs(enemy.pos.x - hero.pos.x) < 7) && (abs(enemy.pos.y - hero.pos.y) < 7))
	{
		//attack
		if (enemy.attack_time < time - 1.5)
		{
			enemy.isAttack = true;
			hero.health -= (ZOMBIE_DAMAGE * hero.damageResistance);
			if (hero.state != BEAST)
			{
				hero.state = DAMAGED;
				hero.isSoundTakeDamage = true;
				hero.currentFrame = 0;
			}
			enemy.attack_time = time;
			enemy.isNear = enemy.dir;  //for Beast melee attack
		}
	}
};

bool IsIntersectWithOtherEnemy(vector<Enemy> & zombieList, int & index)
{
	int i = 0;
	for (Enemy & enemy : zombieList)
	{
		if (i != index)
		{
			if (zombieList[index].sprite.getGlobalBounds().intersects(enemy.sprite.getGlobalBounds()))
			{
				//cout << "IND " << index << " i " << i << endl;
				return true;
			}
		}
		i++;
	}
	return false;
}

bool IsIntersectWithHero(Sprite & enemy, Sprite & hero)
{
	if (enemy.getGlobalBounds().intersects(hero.getGlobalBounds()))
		return true;
	return false;
}


void UpdateEnemyFrame(Enemy & enemy, float & time)
{
	if (enemy.type == COMMON)
	{
		if (enemy.state == DEAD)  //if zombie is exploding
		{
			enemy.sprite.setTextureRect(IntRect(5 + 40 * int(enemy.currentFrame), 344, 34, 48));
			enemy.currentFrame += 0.05;
			if (enemy.currentFrame > 9)
			{
				enemy.state = EXPLODED;
			}
		}
		if (enemy.state == NOTSPAWNED)  //if zombie is not spawned yet
		{
			enemy.sprite.setTextureRect(IntRect(15 + 50 * int(enemy.currentFrame), 12, 33, 51));

			if (enemy.currentFrame > 5)
			{
				enemy.state = ACTIVE;
				enemy.currentFrame = 0;
			}
			enemy.currentFrame += 0.04;
		}
		else if (enemy.state == ACTIVE)
		{
			if (enemy.health < 1)
			{
				enemy.state = DEAD;
			}
			else
			{
				//sprite change for active moving zombies
				switch (enemy.dir)
				{
				case UP:
					enemy.sprite.setTextureRect(IntRect(15 + 36 * int(enemy.currentFrame), 84 + 59 + 59, 27, 49));
					if (enemy.currentFrame > 4)
					{
						enemy.currentFrame = 0;
					}
					break;
				case UPRIGHT: case RIGHT: case DOWNRIGHT:
					enemy.sprite.setTextureRect(IntRect(15 + 36 * int(enemy.currentFrame), 84 + 59 * 3, 27, 49));
					if (enemy.currentFrame > 2)
					{
						enemy.currentFrame = 0;
					}
					break;
				case DOWN:
					enemy.sprite.setTextureRect(IntRect(15 + 36 * int(enemy.currentFrame), 84, 27, 49));
					if (enemy.currentFrame > 2)
					{
						enemy.currentFrame = 0;
					}
					break;
				case DOWNLEFT: case LEFT: case UPLEFT:
					enemy.sprite.setTextureRect(IntRect(15 + 36 * int(enemy.currentFrame), 84 + 59, 27, 49));
					if (enemy.currentFrame > 2)
					{
						enemy.currentFrame = 0;
					}
					break;
				case NONE:
					//no need??
					enemy.sprite.setTextureRect(IntRect(190, 84, 27, 48));
					if (enemy.currentFrame > 4)
					{
						enemy.currentFrame = 0;
					}
					break;
				}
			}
		}
		enemy.currentFrame += 0.05;
	}

	
	else if (enemy.type == AXE)
	{	
		if (enemy.isAttack)
		{
			ComputeEnemyAttackFrame(enemy);
		}
		else
		{
			if (enemy.state == ACTIVE)
			{
				if (enemy.health < 1)
				{
					enemy.state = DEAD;
				}
				else
				{
					//sprite change for active moving zombies


					/*
					10, 7, 22, 31
					10, 43, 22, 31
					10, 80, 22, 31
					10, 118, 22, 31
					*/

					/*
					5, 173, 24, 33
					5, 201, 24, 33
					5, 229, 24, 33
					5, 256, 24, 33
					*/


					switch (enemy.dir)
					{
					case UP:
						enemy.sprite.setTextureRect(IntRect(10 + 22 * int(enemy.currentFrame), 7, 21, 31));
						if (enemy.currentFrame > 4)
						{
							enemy.currentFrame = 0;
						}
						break;
					case UPRIGHT: case RIGHT: case DOWNRIGHT:
						enemy.sprite.setTextureRect(IntRect(10 + 22 * int(enemy.currentFrame), 118, 21, 31));
						if (enemy.currentFrame > 2)
						{
							enemy.currentFrame = 0;
						}
						break;
					case DOWN:
						enemy.sprite.setTextureRect(IntRect(10 + 22 * int(enemy.currentFrame), 43, 21, 31));
						if (enemy.currentFrame > 2)
						{
							enemy.currentFrame = 0;
						}
						break;
					case DOWNLEFT: case LEFT: case UPLEFT:
						enemy.sprite.setTextureRect(IntRect(10 + 22 * int(enemy.currentFrame), 80, 21, 31));
						if (enemy.currentFrame > 2)
						{
							enemy.currentFrame = 0;
						}
						break;
					case NONE:
						//no need??
						enemy.sprite.setTextureRect(IntRect(190, 84, 27, 48));
						if (enemy.currentFrame > 4)
						{
							enemy.currentFrame = 0;
						}
						break;
					}
				}
			}
			else if (enemy.state == DEAD)
			{
				enemy.sprite.setTextureRect(IntRect(5 + 29 * int(enemy.currentFrame),232, 30, 38));
				enemy.currentFrame += 0.03;
				if (enemy.currentFrame > 9)
				{
					enemy.state = EXPLODED;
				}
			}
			else if (enemy.state == NOTSPAWNED)
			{
				//cout << "TIMES " << time << " " << enemy.spawnTime << " " << enemy.spawnDelay << " " << endl;
				if (enemy.spawnTime + enemy.spawnDelay > time)
				{
					enemy.state = ACTIVE;
				}
			}
			enemy.currentFrame += 0.05;
		}
	}
};

void ComputeEnemyAttackFrame(Enemy & enemy)
{
	switch (enemy.dirLast)
	{
	case UP:
		enemy.sprite.setTextureRect(IntRect(34, 174, 28, 27));
		if (enemy.currentFrame > 4)
		{
			enemy.currentFrame = 0;
		}
		break;
	case UPRIGHT: case RIGHT: case DOWNRIGHT:
		enemy.sprite.setTextureRect(IntRect(62, 173, 28, 27));
		if (enemy.currentFrame > 2)
		{
			enemy.currentFrame = 0;
		}
		break;
	case DOWN:
		enemy.sprite.setTextureRect(IntRect(89, 173, 28, 27));
		if (enemy.currentFrame > 2)
		{
			enemy.currentFrame = 0;
		}
		break;
	case DOWNLEFT: case LEFT: case UPLEFT:
		enemy.sprite.setTextureRect(IntRect(5, 173, 28, 27));
		if (enemy.currentFrame > 2)
		{
			enemy.currentFrame = 0;
		}
		break;
	default:
		break;
	}
	enemy.currentFrame += 0.05;

	if (enemy.currentFrame > 2)
	{
		enemy.isAttack = false;
	}
}

void ResetEnemySpawnTime(vector<Enemy> & zombieList, float & time)
{
	for (Enemy & enemy : zombieList)
	{
		enemy.spawnTime = time;
	}
}

void EnemyMoveRandom(vector<Enemy> & zombieList)  //not using
{
	for (vector<Enemy>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
	{
		if (zombie->dir == NONE)
		{
			int rand_dir = 1 + rand() % 4;

			switch (rand_dir)
			{
			case 1:
				zombie->dir = UP;
				break;
			case 2:
				zombie->dir = RIGHT;
				break;
			case 3:
				zombie->dir = DOWN;
				break;
			case 4:
				zombie->dir = LEFT;
				break;
			}
		}
	}
};


void ComputeEnemyDirection(Enemy & enemy, Vector2f & heroPos)
{
	//compute distance and dir
	Vector2f distance = { abs(heroPos.x - enemy.pos.x),
						  abs(heroPos.y - enemy.pos.y) };
	if (distance.x > 5 || distance.y > 5)
	{
		//TODO: check left-right dir zombie sprite bug (almost)
		if ((distance.x > 3 && distance.y > 3) && (distance.x / distance.y > 0.9) && (distance.y / distance.x < 1.1))
		{
			if (heroPos.x >= enemy.pos.x && heroPos.y >= enemy.pos.y)
				enemy.dir = DOWNRIGHT;
			else if (heroPos.x >= enemy.pos.x && heroPos.y < enemy.pos.y)
				enemy.dir = UPRIGHT;
			else if (heroPos.x < enemy.pos.x && heroPos.y >= enemy.pos.y)
				enemy.dir = DOWNLEFT;
			else if (heroPos.x < enemy.pos.x && heroPos.y < enemy.pos.y)
				enemy.dir = UPLEFT;
		}
		else if (distance.x >= distance.y)
		{
			if (heroPos.x > enemy.pos.x)
				enemy.dir = RIGHT;
			else
				enemy.dir = LEFT;
		}
		else if (distance.x < distance.y)
		{
			if (heroPos.y > enemy.pos.y)
				enemy.dir = DOWN;
			else
				enemy.dir = UP;
		}
	}
}

void SpawnEnemyRandomly(vector<Enemy>&zombieList, vector<Object> & objects, int zombiesRemaining, float & time, Sprites & sprites)
{
	//TODO: MAKE FUNCTION MORE READABLE
	do
	{
		bool needNewBlock = false;

		Vector2f newPos = { float((rand() % WIDTH_MAP) * STEP), float((rand() % HEIGHT_MAP) * STEP) };

		FloatRect lootRect = { newPos.x,newPos.y,ZOMBIE_SPAWN_RADIUS_COLLISION, ZOMBIE_SPAWN_RADIUS_COLLISION };
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (lootRect.intersects(objects[i].rect))
			{
				needNewBlock = true;
				break;
			}
		}
		if (!needNewBlock)
		{
			for (Enemy & enemy : zombieList)
				if ((abs(enemy.pos.x - newPos.x) < 100 && abs(enemy.pos.y - newPos.y) < 100))
				{
					needNewBlock = true;
					break;
				}
			if (needNewBlock == false)
			{
				//sprites.enemy.setPosition(newPos);
				//ZombieSpawn(zombieList, time, newPos.x,newPos.y, sprite_zombie, COMMON);
				if (time < 10)
				{
					SpawnEnemy(zombieList, time, newPos.x, newPos.y, sprites, COMMON);
				}
				else
				{
					EnemyType type = COMMON;
					if (zombieList.size() > 0)
					{
						if (zombieList[zombieList.size() - 1].type == AXE)
							type = COMMON;
						else type = AXE;
					}
					SpawnEnemy(zombieList, time, newPos.x, newPos.y, sprites, type);
				}
				//zombieList[zombieList.size() - 1].sprite = sprites.zombie;

				zombiesRemaining -= 1;
			}
		}
	} while (zombiesRemaining > 0);
}

void CheckEnemyDir(float & time, Enemy & enemy)
{
	if (enemy.follow == false)
	{
		if (enemy.dirChangeTime == 0)
		{
			//cout << " 1 IS " << endl;
			enemy.dir = ComputeRandDir(enemy);
		}
		if (time - enemy.dirChangeTime > ZOMBIE_DIR_CHANGE_TIME)
		{
			//cout << " 2 IS " << endl;
			enemy.dirChangeTime = 0;
		}
	}
};

void CheckEnemyExplosion(vector<Explosion> & explosionList, vector<Enemy> & zombieList)
{
	for (vector<Explosion>::iterator explosion = explosionList.begin(); explosion != explosionList.end();)
	{
		if (explosion->currentFrame > 12)
		{
			for (vector<Enemy>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); zombie++)
			{
				if (abs(zombie->pos.x - (explosion->pos.x + 120)) < 120 && (abs(zombie->pos.y - (explosion->pos.y + 70)) < 120))
				{
					zombie->health = 0;
				}
			}
			explosion = explosionList.erase(explosion);

		}
		else
		{
			explosion++;
		}
	}
}

void DrawEnemies(RenderWindow & window, vector<Enemy> & zombieList)
{
	for (Enemy & enemy : zombieList)
	{
		window.draw(enemy.sprite);
	}
};

void DeleteEnemyList(vector<Enemy> & zombies)
{
	zombies.clear();
}