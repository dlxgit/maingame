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

void CheckNpcDeath(vector<Npc> & npcList, vector<Zombie>::iterator & zombie)
{
	Vector2f npcCenter;
	Vector2f zombieCenter;
	zombieCenter.x = zombie->sprite.getPosition().x + zombie->sprite.getGlobalBounds().width / 2;
	zombieCenter.y = zombie->sprite.getPosition().y + zombie->sprite.getGlobalBounds().height / 2;

	for (vector<Npc>::iterator npc = npcList.begin(); npc != npcList.end();)
	{
		if (npc->state == LIVING)  //if loot.item.center contains heroSprite  -> add new item in inventory
		{
			npcCenter.x = npc->sprite.getPosition().x + npc->sprite.getGlobalBounds().width / 2;
			npcCenter.y = npc->sprite.getPosition().y + npc->sprite.getGlobalBounds().height / 2;
			//cout << (abs(zombieCenter.x - npcCenter.x)) << " CENTER " << abs(zombieCenter.y - npcCenter.y) << endl;
			if ((abs(zombieCenter.x - npcCenter.x) < 35) && (abs(zombieCenter.y - npcCenter.y)) < 35)
			{
				npc->state = KILLED;
			}
		}
		npc++;
	}
};


Direction ComputeRandDir(vector<Zombie>::iterator & zombie)
{
	Direction dir = zombie->dir;

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

void ZombieCheckDir(vector<Zombie>::iterator & zombie, float &time)
{
	if (zombie->follow == false)
	{
		if (zombie->dirChangeTime == 0)
		{
			//cout << " 1 IS " << endl;
			zombie->dir = ComputeRandDir(zombie);
		}
		if (time - zombie->dirChangeTime > ZOMBIE_DIR_CHANGE_TIME)
		{
			//cout << " 2 IS " << endl;
			zombie->dirChangeTime = 0;
		}
	}
};

void CheckZombieCollision(vector<Zombie>::iterator & zombie) //мб объединить с функцией update? чтобы сразу здесь вычисл€ть перемещение геро€ (проблема с коллизией при движении по диагонали
{
	//исправить чтобы дл€ всех а не только преследующих
	if (zombie->follow == true)
	{
		float x = zombie->pos.x;
		float y = zombie->pos.y;

		//dunno
		float x0 = x;
		float y0 = y;

		//spritesize
		float sizeX = zombie->sprite.getGlobalBounds().width;
		float sizeY = zombie->sprite.getGlobalBounds().height;

		//проверка 4 угловых точек спрайта (верхн€€ лева€, права€, нижн€€ лева€, права€) на вхождение в блок карты
		bool q = (TILEMAP[int(y + sizeY - STEP) / STEP][int(x) / STEP] != ' ');
		bool w = (TILEMAP[int(y + sizeY - STEP) / STEP][int(x + sizeX - 1) / STEP] != ' ');
		bool e = (TILEMAP[int(y + sizeY - 1) / STEP][int(x + sizeX - 1) / STEP] != ' ');
		bool r = (TILEMAP[int(y + sizeY - 1) / STEP][int(x) / STEP] != ' ');

		if (zombie->follow == false)
		{
			if (q || w || e || r)
			{
				if ((q || w) && !(e || r))
				{
					zombie->dir = DOWN;
				}
				else if ((q || r) && !(w || e))
				{
					zombie->dir = RIGHT;
				}
				else if ((w || e) && !(q || r))
				{
					zombie->dir = LEFT;
				}
				else if ((e || r) && !(q || w))
				{
					zombie->dir = UP;
				}
			}
		}

		else if (zombie->follow)
		{
			switch (zombie->dir)
			{
			case UP:
				//up left and right
				if (q || w)
				{
					y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
				}
				break;
			case UPRIGHT:
				//upright except downleft

				if (w  && !(q || e))
				{
					y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
					x = x + (1 - DIAGONAL_STEP)* STEP_HERO;
				}
				else if (q && e)
				{
					y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
					x = (int(x + sizeX) / STEP) * STEP - sizeX;
				}
				else if (q && !e) //upleft
				{
					y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
					x = x + (1 - DIAGONAL_STEP)* STEP_HERO;
				}
				else if (e && !q) //upright
				{
					x = (int(x + sizeX) / STEP) * STEP - sizeX;
					y = y - (1 - DIAGONAL_STEP)* STEP_HERO;
				}
				break;
			case RIGHT:
				//right up and down
				if (w || e)
				{
					x = (int(x + sizeX) / STEP) * STEP - sizeX;
				}
				break;
			case DOWNRIGHT:
				//downright except upleft
				if (e && !(w || r))
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
					x = x + (1 - DIAGONAL_STEP) * STEP_HERO;
				}
				else if (w && r)  //downright
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
					x = (int(x + sizeX) / STEP) * STEP - sizeX;
				}
				else if (r && !w) //downleft
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
					x = x + (1 - DIAGONAL_STEP) * STEP_HERO;
				}
				else if (w && !r)  //upright
				{
					x = (int(x + sizeX) / STEP) * STEP - sizeX;
					y = y + (1 - DIAGONAL_STEP)* STEP_HERO;
				}

				break;
			case DOWN:
				//down left and right
				if (e || r)
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
				}
				break;
			case DOWNLEFT:
				//downleft except upright
				if (r && !(q || e))
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
					x = x - (1 - DIAGONAL_STEP)* STEP_HERO;
				}
				else if ((q && e) || (r && !(q || e)))  //downleft
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
					x = (int(x) / STEP) * STEP + STEP;
				}
				else if (e && !q) //downright
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
					x = x - (1 - DIAGONAL_STEP)* STEP_HERO;
				}
				else if (q && !e) //upleft
				{
					x = (int(x) / STEP) * STEP + STEP;
					y = y + 0.33* STEP_HERO;
				}
				break;
			case LEFT:
				//left up and down
				if (q || r)
				{
					x = (int(x) / STEP) * STEP + STEP;
				}
				break;
			case UPLEFT:
				//upleft except downright
				if (q && !(w || r))
				{
					y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
					x = x - 0.33* STEP_HERO;
				}
				else if ((w && r) || (q && !(w || r)))   //upleft
				{
					y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
					x = (int(x) / STEP) * STEP + STEP;
				}
				else if (w && !r) //upright
				{
					y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
					x = x - (1 - DIAGONAL_STEP)* STEP_HERO;
				}
				else if (r && !w)  //downleft
				{
					x = (int(x) / STEP) * STEP + STEP;
					y = y - (1 - DIAGONAL_STEP)* STEP_HERO;
				}

				break;
			case NONE:
				break;
			}
		}

		zombie->pos.x = x;
		zombie->pos.y = y;
		zombie->sprite.setPosition(zombie->pos.x, zombie->pos.y);
	}
};

void ZombieCheckFollow(vector<Zombie>::iterator & zombie, Hero & hero)
{
	if (abs(zombie->pos.x - hero.pos.x) > ZOMBIE_VISION_DISTANCE || abs(zombie->pos.y - hero.pos.y) > ZOMBIE_VISION_DISTANCE)
	{
		zombie->follow = false;
	}
	if (zombie->follow == false)
	{
		if (abs(zombie->pos.x - hero.pos.x) < ZOMBIE_VISION_DISTANCE && abs(zombie->pos.y - hero.pos.y) < ZOMBIE_VISION_DISTANCE)
		{
			zombie->follow = true;
		}
	}
};

void ZombieUpdatePosition(vector<Zombie>::iterator & zombie)
{
	//TODO:vec2f
	float xZombie = zombie->sprite.getPosition().x;
	float yZombie = zombie->sprite.getPosition().y;

	int stepZ;
	if (zombie->follow == true)
	{
		stepZ = STEP_ZOMBIE_ACTIVE;
	}
	else
	{
		stepZ = STEP_ZOMBIE;
	}

	switch (zombie->dir)
	{
	case UP:
		yZombie -= stepZ;
		break;
	case DOWN:
		yZombie += stepZ;
		break;
	case LEFT:
		xZombie -= stepZ;
		break;
	case RIGHT:
		xZombie += stepZ;
		break;
	case UPLEFT:
		xZombie -= stepZ * DIAGONAL_STEP;
		yZombie -= stepZ * DIAGONAL_STEP;
		break;
	case UPRIGHT:
		xZombie += stepZ * DIAGONAL_STEP;
		yZombie -= stepZ * DIAGONAL_STEP;
		break;
	case DOWNLEFT:
		xZombie -= stepZ * DIAGONAL_STEP;
		yZombie += stepZ * DIAGONAL_STEP;
		break;
	case DOWNRIGHT:
		xZombie += stepZ * DIAGONAL_STEP;
		yZombie += stepZ * DIAGONAL_STEP;
		break;
	}

	//struct_zombies[z].sprite.setTextureRect(IntRect(5, 5, 30, 30));
	zombie->pos = { xZombie, yZombie };
	//struct_zombies[z].sprite.setPosition(x, y);
	//zametka
	zombie->sprite.setPosition(zombie->pos.x, zombie->pos.y);
};

//should i store it in zombieList>? if i can just keep it here (return true)
bool IsZombieNearHero(Hero & hero, vector<Zombie>::iterator & zombie)
{
	//comparing distance between two nearest points of hero and zombie sprites    to define is zombie near or not
	//TODO: ref

	Vector2f zombieCenter;
	zombieCenter.x = zombie->sprite.getPosition().x + zombie->sprite.getGlobalBounds().width / 2;
	zombieCenter.y = zombie->sprite.getPosition().y + zombie->sprite.getGlobalBounds().height / 2;
	Vector2f heroCenter;
	heroCenter.x = hero.sprite.getPosition().x + hero.sprite.getGlobalBounds().width / 2;
	heroCenter.y = hero.sprite.getPosition().y + hero.sprite.getGlobalBounds().height / 2;

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

void ZombieUpdateAttack(Hero & hero, vector<Zombie>::iterator zombie, const float& time)
{
	//add correct xyHero xyZombie for detecting meleeAttack
	//TODO: change mechanics of zombie attack(depends on side from which it attacks), same for beast attack
	if ((abs(zombie->pos.x - hero.pos.x) < 7) && (abs(zombie->pos.y - hero.pos.y) < 7))
	{
		//attack
		if (zombie->attack_time < time - 1.5)
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
			zombie->attack_time = time;
			zombie->isNear = zombie->dir;  //for Beast melee attack
		}
	}
};

void ZombieUpdateSprite(vector<Zombie>::iterator & zombie)
{
	if (zombie->state == DEAD)  //if zombie is exploding
	{
		zombie->sprite.setTextureRect(IntRect(5 + 40 * int(zombie->currentFrame), 344, 34, 48));
		zombie->currentFrame += 0.05;
		if (zombie->currentFrame > 9)
		{
			zombie->state = EXPLODED;
		}
	}
	if (zombie->state == NOTSPAWNED)  //if zombie is not spawned yet
	{
		zombie->sprite.setTextureRect(IntRect(15 + 50 * int(zombie->currentFrame), 12, 33, 51));

		if (zombie->currentFrame > 5)
		{
			zombie->state = ACTIVE;
			zombie->currentFrame = 0;
		}
		zombie->currentFrame += 0.04;
	}
	else if (zombie->state == ACTIVE)
	{
		if (zombie->health < 1)
		{
			zombie->state = DEAD;
		}
		else
		{
			//sprite change for active moving zombies
			switch (zombie->dir)
			{
			case UP:
				zombie->sprite.setTextureRect(IntRect(15 + 36 * int(zombie->currentFrame), 84 + 59 + 59, 27, 49));
				if (zombie->currentFrame > 4)
				{
					zombie->currentFrame = 0;
				}
				break;
			case UPRIGHT: case RIGHT: case DOWNRIGHT:
				//21 38
				zombie->sprite.setTextureRect(IntRect(15 + 36 * int(zombie->currentFrame), 84 + 59 * 3, 27, 49));
				if (zombie->currentFrame > 2)
				{
					zombie->currentFrame = 0;
				}
				break;
			case DOWN:
				zombie->sprite.setTextureRect(IntRect(15 + 36 * int(zombie->currentFrame), 84, 27, 49));
				if (zombie->currentFrame > 2)
				{
					zombie->currentFrame = 0;
				}
				break;
			case DOWNLEFT: case LEFT: case UPLEFT:
				zombie->sprite.setTextureRect(IntRect(15 + 36 * int(zombie->currentFrame), 84 + 59, 27, 49));
				if (zombie->currentFrame > 2)
				{
					zombie->currentFrame = 0;
				}
				break;
			case NONE:
				//no need??
				zombie->sprite.setTextureRect(IntRect(190, 84, 27, 48));
				if (zombie->currentFrame > 4)
				{
					zombie->currentFrame = 0;
				}
				break;
			}
		}
	}
	zombie->currentFrame += 0.05;
};


void ZombieUpdateAttack(Hero & hero, vector<Zombie>::iterator zombie, float & time)
{
	//add correct xyHero xyZombie for detecting meleeAttack
	//TODO: change mechanics of zombie attack(depends on side from which it attacks), same for beast attack
	if ((abs(zombie->pos.x - hero.pos.x) < 7) && (abs(zombie->pos.y - hero.pos.y) < 7))
	{
		//attack
		if (zombie->attack_time < time - 1.5)
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
			zombie->attack_time = time;
			zombie->isNear = zombie->dir;  //for Beast melee attack
		}
	}
}

void ZombieMoveRandom(vector<Zombie> & zombieList)
{
	int rand_dir;
	Direction dir_zombie;

	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
	{
		if (zombie->dir == NONE)
		{
			rand_dir = 1 + rand() % 4;

			switch (rand_dir)
			{
			case 1:
				dir_zombie = UP;
				break;
			case 2:
				dir_zombie = RIGHT;
				break;
			case 3:
				dir_zombie = DOWN;
				break;
			case 4:
				dir_zombie = LEFT;
				break;
			}
		}
	}
};


void SpawnZombieRandomly(vector<Zombie>&zombieList, int zombiesRemaining, float & time, Sprite & sprite_zombie)
{
	int x;
	int y;
	bool needNewBlock = false;

	do
	{
		x = (rand() % WIDTH_MAP) * STEP;
		y = (rand() % HEIGHT_MAP) * STEP;
		needNewBlock = false;
		Vector2f posRand = { float(x),float(y) };

		if (TILEMAP[y / STEP][x / STEP] == 'b') needNewBlock = true;
		else
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


void ZombieCheckDir(float & time, vector<Zombie>::iterator & zombie)
{
	if (zombie->follow == false)
	{
		if (zombie->dirChangeTime == 0)
		{
			//cout << " 1 IS " << endl;
			zombie->dir = ComputeRandDir(zombie);
		}
		if (time - zombie->dirChangeTime > ZOMBIE_DIR_CHANGE_TIME)
		{
			//cout << " 2 IS " << endl;
			zombie->dirChangeTime = 0;
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


/*
void UpdateZombies(vector<Zombie> & zombieList, Hero & hero, vector<Npc> & npcList,const float & time)
{
//TODO: refact!
float xHero = hero.sprite.getPosition().x;
float yHero = hero.sprite.getPosition().y;

//iterating zombieList
for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end();)
{
Direction dir = zombie->dir;

//compute distance and dir
float dx = abs(xHero - zombie->pos.x);  //distance x
float dy = abs(yHero - zombie->pos.y);  //distance y
//float di = sqrt(pow(dx,2) + pow(dy,2));  //distance
if (zombie->state == ACTIVE)
{
ZombieCheckFollow(zombie, hero);
if (zombie->follow)
{
//TODO: ref
if (dx > 5 || dy > 5)
{
//TODO: check left-right dir zombie sprite bug (almost)
if ((dx > 3 && dy > 3) && (dx / dy > 0.9) && (dy / dx < 1.1))
{
if (xHero >= zombie->pos.x && yHero >= zombie->pos.y)
dir = DOWNRIGHT;
else if (xHero >= zombie->pos.x && yHero < zombie->pos.y)
dir = UPRIGHT;
else if (xHero < zombie->pos.x && yHero >= zombie->pos.y)
dir = DOWNLEFT;
else if (xHero < zombie->pos.x && yHero < zombie->pos.y)
dir = UPLEFT;
}
else if (dx >= dy)
{
if (xHero > zombie->pos.x)
dir = RIGHT;
else
dir = LEFT;
}
else if (dx < dy)
{
if (yHero > zombie->pos.y)
dir = DOWN;
else
dir = UP;
}
}

CheckNpcDeath(npcList, zombie);
zombie->dir = dir;
}
else
{
ZombieCheckDir(time, zombie);
}

CheckZombieCollision(zombie);
ZombieUpdateAttack(hero, zombie, game);

if (hero.state == BEAST)
{
if (IsZombieNearHero(hero, zombie))
{
CheckHeroBeastDamage(hero, zombie, game);
}
}
if (xHero != zombie->pos.x || yHero != zombie->pos.y)
{
if (zombie->follow)
{
ZombieUpdatePosition(zombie);
}
}
}
ZombieUpdateSprite(zombie);
//UpdateZombiePosition(i);  TODO: make it for all zombies, not jsut for following ones

//TOCHECK: why dir is here? is it needed below?
if (dir != zombie->dirLast)
{
//zombie->currentFrame = 0;
zombie->dirLast = dir;
}

//zombie->dir = dir


if (zombie->state == EXPLODED)  //deleting
{
zombie = zombieList.erase(zombie);
}
else zombie++;
}
};
*/