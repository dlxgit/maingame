#pragma once
#include "zombie.h"


void ZombieSpawn(vector<Zombie> & zombieList, float time, int posX, int posY, Sprite & sprite_zombie)
{
	Zombie zombie;

	zombie.pos.x = posX;
	zombie.pos.y = posY;
	zombie.health = ZOMBIE_MAX_HP;

	//zombie.texture = texture_zombie;
	zombie.sprite = sprite_zombie;
	zombie.sprite.setPosition(posX, posY);

	zombie.state = NOTSPAWNED;
	zombie.currentFrame = 0;
	zombie.attack_time = 0;
	zombie.dir = NONE;
	zombie.dirLast = NONE;
	zombie.follow = false;
	zombie.isNear = false;
	zombie.sprite.setTextureRect(IntRect(5, 5, 30, 30));

	zombie.spawnTime = time;
	zombie.dirChangeTime = 0;

	zombieList.push_back(zombie);
};

void CheckNpcDeath(vector<Npc> & npcList, Zombie & zombie)
{
	Vector2f zombieCenter = GetSpriteCenter(zombie.sprite);

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


Direction ComputeRandDir(Zombie & zombie)
{
	Direction dir = zombie.dir;

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

void ZombieCheckDir(Zombie & zombie, float &time)
{
	if (zombie.follow == false)
	{
		if (zombie.dirChangeTime == 0)
		{
			//cout << " 1 IS " << endl;
			zombie.dir = ComputeRandDir(zombie);
		}
		if (time - zombie.dirChangeTime > ZOMBIE_DIR_CHANGE_TIME)
		{
			//cout << " 2 IS " << endl;
			zombie.dirChangeTime = 0;
		}
	}
};

void ZombieCheckFollow(Zombie & zombie, Hero & hero)
{
	if (abs(zombie.pos.x - hero.pos.x) > ZOMBIE_VISION_DISTANCE || abs(zombie.pos.y - hero.pos.y) > ZOMBIE_VISION_DISTANCE)
	{
		zombie.follow = false;
	}
	if (zombie.follow == false)
	{
		if (abs(zombie.pos.x - hero.pos.x) < ZOMBIE_VISION_DISTANCE && abs(zombie.pos.y - hero.pos.y) < ZOMBIE_VISION_DISTANCE)
		{
			zombie.follow = true;
		}
	}
};


//should i store it in zombieList>? if i can just keep it here (return true)
bool IsZombieNearHero(Hero & hero, Zombie & zombie)
{
	//comparing distance between two nearest points of hero and zombie sprites    to define is zombie near or not
	//TODO: ref

	Vector2f zombieCenter = GetSpriteCenter(zombie.sprite);
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

void ZombieUpdateAttack(Hero & hero, Zombie & zombie, const float & time)
{
	//add correct xyHero xyZombie for detecting meleeAttack
	//TODO: change mechanics of zombie attack(depends on side from which it attacks), same for beast attack
	if ((abs(zombie.pos.x - hero.pos.x) < 7) && (abs(zombie.pos.y - hero.pos.y) < 7))
	{
		//attack
		if (zombie.attack_time < time - 1.5)
		{
			if (hero.state == BEAST)
				hero.health -= (ZOMBIE_DAMAGE / 3);
			else
				hero.health -= (ZOMBIE_DAMAGE);
			if (hero.state != BEAST)
			{
				hero.state = DAMAGED;
				hero.currentFrame = 0;
			}
			zombie.attack_time = time;
			zombie.isNear = zombie.dir;  //for Beast melee attack
		}
	}
};

bool IsIntersectWithOtherZombie(vector<Zombie> & zombieList, int & index)
{
	int i = 0;
	for (Zombie & zomb : zombieList)
	{
		if (i != index)
		{
			if (zombieList[index].sprite.getGlobalBounds().intersects(zomb.sprite.getGlobalBounds()))
			{
				cout << "IND " << index << " i " << i << endl;
				return true;
			}
		}
		i++;
	}
	return false;
}

void UpdateZombieFrame(Zombie & zombie)
{
	if (zombie.state == DEAD)  //if zombie is exploding
	{
		zombie.sprite.setTextureRect(IntRect(5 + 40 * int(zombie.currentFrame), 344, 34, 48));
		zombie.currentFrame += 0.05;
		if (zombie.currentFrame > 9)
		{
			zombie.state = EXPLODED;
		}
	}
	if (zombie.state == NOTSPAWNED)  //if zombie is not spawned yet
	{
		zombie.sprite.setTextureRect(IntRect(15 + 50 * int(zombie.currentFrame), 12, 33, 51));

		if (zombie.currentFrame > 5)
		{
			zombie.state = ACTIVE;
			zombie.currentFrame = 0;
		}
		zombie.currentFrame += 0.04;
	}
	else if (zombie.state == ACTIVE)
	{
		if (zombie.health < 1)
		{
			zombie.state = DEAD;
		}
		else
		{
			//sprite change for active moving zombies
			switch (zombie.dir)
			{
			case UP:
				zombie.sprite.setTextureRect(IntRect(15 + 36 * int(zombie.currentFrame), 84 + 59 + 59, 27, 49));
				if (zombie.currentFrame > 4)
				{
					zombie.currentFrame = 0;
				}
				break;
			case UPRIGHT: case RIGHT: case DOWNRIGHT:
				zombie.sprite.setTextureRect(IntRect(15 + 36 * int(zombie.currentFrame), 84 + 59 * 3, 27, 49));
				if (zombie.currentFrame > 2)
				{
					zombie.currentFrame = 0;
				}
				break;
			case DOWN:
				zombie.sprite.setTextureRect(IntRect(15 + 36 * int(zombie.currentFrame), 84, 27, 49));
				if (zombie.currentFrame > 2)
				{
					zombie.currentFrame = 0;
				}
				break;
			case DOWNLEFT: case LEFT: case UPLEFT:
				zombie.sprite.setTextureRect(IntRect(15 + 36 * int(zombie.currentFrame), 84 + 59, 27, 49));
				if (zombie.currentFrame > 2)
				{
					zombie.currentFrame = 0;
				}
				break;
			case NONE:
				//no need??
				zombie.sprite.setTextureRect(IntRect(190, 84, 27, 48));
				if (zombie.currentFrame > 4)
				{
					zombie.currentFrame = 0;
				}
				break;
			}
		}
	}
	zombie.currentFrame += 0.05;
};

void ZombieMoveRandom(vector<Zombie> & zombieList)  //not using
{
	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
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


void ComputeZombieDirection(Zombie & zombie, Vector2f & heroPos)
{
	//compute distance and dir
	Vector2f distance = { abs(heroPos.x - zombie.pos.x),
						  abs(heroPos.y - zombie.pos.y) };
	if (distance.x > 5 || distance.y > 5)
	{
		//TODO: check left-right dir zombie sprite bug (almost)
		if ((distance.x > 3 && distance.y > 3) && (distance.x / distance.y > 0.9) && (distance.y / distance.x < 1.1))
		{
			if (heroPos.x >= zombie.pos.x && heroPos.y >= zombie.pos.y)
				zombie.dir = DOWNRIGHT;
			else if (heroPos.x >= zombie.pos.x && heroPos.y < zombie.pos.y)
				zombie.dir = UPRIGHT;
			else if (heroPos.x < zombie.pos.x && heroPos.y >= zombie.pos.y)
				zombie.dir = DOWNLEFT;
			else if (heroPos.x < zombie.pos.x && heroPos.y < zombie.pos.y)
				zombie.dir = UPLEFT;
		}
		else if (distance.x >= distance.y)
		{
			if (heroPos.x > zombie.pos.x)
				zombie.dir = RIGHT;
			else
				zombie.dir = LEFT;
		}
		else if (distance.x < distance.y)
		{
			if (heroPos.y > zombie.pos.y)
				zombie.dir = DOWN;
			else
				zombie.dir = UP;
		}
	}
}

void SpawnZombieRandomly(vector<Zombie>&zombieList, vector<Object> & objects, int zombiesRemaining, float & time, Sprite & sprite_zombie)
{
	//TODO: MAKE FUNCTION MORE READABLE
	do
	{
		bool needNewBlock = false;

		float x = (rand() % WIDTH_MAP) * STEP;
		float y = (rand() % HEIGHT_MAP) * STEP;

		FloatRect lootRect = { x,y,ZOMBIE_SPAWN_RADIUS_COLLISION, ZOMBIE_SPAWN_RADIUS_COLLISION };
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
			for (vector<Zombie>::iterator i = zombieList.begin(); i != zombieList.end(); ++i)
				if ((abs(i->pos.x - x) < 100 && abs(i->pos.y - y) < 100))
				{
					needNewBlock = true;
					break;
				}
			if (needNewBlock == false)
			{
				sprite_zombie.setPosition(x, y);
				ZombieSpawn(zombieList, time, x, y, sprite_zombie);
				zombieList[zombieList.size() - 1].sprite = sprite_zombie;

				zombiesRemaining -= 1;
			}
		}
	} while (zombiesRemaining > 0);
}


void ZombieCheckDir(float & time, Zombie & zombie)
{
	if (zombie.follow == false)
	{
		if (zombie.dirChangeTime == 0)
		{
			//cout << " 1 IS " << endl;
			zombie.dir = ComputeRandDir(zombie);
		}
		if (time - zombie.dirChangeTime > ZOMBIE_DIR_CHANGE_TIME)
		{
			//cout << " 2 IS " << endl;
			zombie.dirChangeTime = 0;
		}
	}
};

void CheckZombieExplosion(vector<Explosion> & explosionList, vector<Zombie> & zombieList)
{
	for (vector<Explosion>::iterator explosion = explosionList.begin(); explosion != explosionList.end();)
	{
		if (explosion->currentFrame > 12)
		{
			//checkDamageZombies;
			for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); zombie++)
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

void DrawZombies(RenderWindow & window, vector<Zombie> & zombieList)
{
	for (Zombie & zombie : zombieList)
	{
		window.draw(zombie.sprite);
	}
};

void DeleteZombieList(vector<Zombie> & zombies)
{
	zombies.clear();
}