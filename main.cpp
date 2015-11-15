#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cmath>
#include "map.h"
#include <vector>

//#include "hero.h"

using namespace sf;
using namespace std;

/*
NEWBUGS:
{
hero sprite diagonal collisions on last block (+zombie?)
}
FOUNDBUGS:
{
zombie doesnt change direction if it collides sometimes
}

NEXT: доработать движение зомби при коллизии  !STOP! + автономное.
add zombie behavour when it's attacking hero

calculate zombie's next direction if it collides
add files and structures

//question.
//struct game for time
*/

struct Inventory
{
	NameItem name;
	int quantity;
	int current;
	Sprite sprite;
};

struct Loot
{
	NameItem name;
	int quantity;
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	bool isDrawn;
};

struct Game
{
	float time;
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

struct Zombie
{
	Vector2f pos;
	int health;
	Direction dir;
	Direction dirLast;
	bool follow;
	float attack_time;
	Texture texture;
	Sprite sprite;
	bool collision;
	float currentFrame;
	ZombieState state;
	bool isNear;  //side of zombie position relatively to hero
};

struct Shot
{
	Vector2f pos;
	int distance;
	Direction dir;
	Sprite sprite;
};

struct Npc
{
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	int health;
};

//TODO: remove from global
std::vector<Inventory> inventoryList;
std::vector<Loot> lootList;
std::vector<Shot> shotList;
std::vector<Zombie> zombieList;
std::vector<Npc> NpcList;

void DrawHero(Hero & hero, RenderWindow & window)
{
	window.draw(hero.sprite);
}

void CheckLoot(Hero & hero, Texture & texture_items)
{
	//TODO: refact.
	float x = hero.sprite.getPosition().x;
	float y = hero.sprite.getPosition().y;

	bool flag = false;

	for (vector<Loot>::iterator i = lootList.begin(); i != lootList.end(); ++i)
	{
		if (i->isDrawn == true)  //if loot.item.center contains heroSprite  -> add new item in inventory
		{
			Vector2f heroCenter;
			heroCenter.x = i->pos.x + i->sprite.getGlobalBounds().width / 2;
			heroCenter.y = i->pos.y + i->sprite.getGlobalBounds().height / 2;
			if (hero.sprite.getGlobalBounds().contains(heroCenter.x, heroCenter.y))
			{
				if (i->name != AMMO)
				{
					cout << " LOOT!!! " << endl;
					//del ячейку

					for (std::vector<Inventory>::iterator j = inventoryList.begin(); j != inventoryList.end(); ++j)
					{
						if (j->name == i->name)
						{

							j->quantity += i->quantity;
							//iter->current = 0;
							j->sprite.setTexture(texture_items);
							j->sprite.setTextureRect(sf::IntRect(i->name * 32, 0, 32, 32));

							i->isDrawn = false;
							flag = flag || true;
							break;
						}
					}

					if (flag == false)
					{
						flag = true;

						//adding new inventory in List
						Inventory inventory;
						inventory.name = i->name;
						inventory.quantity = i->quantity;
						inventory.current = 0;
						inventory.sprite.setTexture(texture_items);
						inventory.sprite.setTextureRect(sf::IntRect(i->name * 32, 0, 32, 32));
						inventoryList.push_back(inventory);

						//che za 
						i->isDrawn = false;
						hero.nSlots += 1;
					}
				}
				else
				{
					i->isDrawn = false;
					int packed = 0;
					while (packed < AMMO_PACKS)
					{
						//delSoon>??
						for (std::vector<Inventory>::iterator iter = inventoryList.begin(); iter != inventoryList.end(); ++iter)
						{
							if (iter->name != MIXTURE && iter->name != KEY && iter->name != DRINK)
							{
								iter->quantity += MAX_AMMO[iter->name];
								packed += 1;
							}
						}
						break;
					}
				}
			}
		}
	}
}

View UpdateView(RenderWindow & window, Hero & hero, View & view)  //UpdateCameraPosition
{
	Vector2f temp = hero.sprite.getPosition();

	if (temp.x < 320) temp.x = 320;
	if (temp.y < 240) temp.y = 240;
	if (temp.y > 554) temp.y = 554;

	view.setCenter(temp);
	window.setView(view);
	return view;
}

void AddNewShot(Hero & hero, Sprite & sprite_shot) //adding new shot in list
{
	//if hero direction is UPRIGHT OR DOWNRIGHT -> he will watch(and shoot) to the left, same for RIGHT

	Shot shot;
	shot.pos = hero.sprite.getPosition();
	shot.distance = 0;
	Texture texture;
	shot.sprite = sprite_shot;

	Direction dir;
	switch (hero.dirLast)
	{
	case UP:
		dir = UP;
		shot.sprite.setTextureRect(IntRect(3, 11, 7, 13));
		break;
	case UPRIGHT: case RIGHT: case DOWNRIGHT:
		dir = RIGHT;
		shot.sprite.setTextureRect(IntRect(0, 0, 13, 7));
		break;
	case DOWN:
		dir = DOWN;
		shot.sprite.setTextureRect(IntRect(19, 11, 7, 13));
		break;
	case DOWNLEFT: case LEFT: case UPLEFT:
		dir = LEFT;
		shot.sprite.setTextureRect(IntRect(16, 0, 13, 7));
		break;
	}
	shot.dir = dir;
	shotList.push_back(shot);
}

void UpdateInventory(Hero & hero, Game & game)
{
	//update items 
	if (inventoryList[hero.slotNo].current == 0 && game.time - hero.lastReloadTime > WEAPON_RELOAD_TIME)
	{
		hero.lastReloadTime = game.time;
		hero.isReloading = false;
		if (inventoryList[hero.slotNo].quantity >= MAX_AMMO[inventoryList[hero.slotNo].name])
		{
			inventoryList[hero.slotNo].quantity -= MAX_AMMO[inventoryList[hero.slotNo].name];
			inventoryList[hero.slotNo].current = MAX_AMMO[inventoryList[hero.slotNo].name];
		}
		else
		{
			inventoryList[hero.slotNo].current = inventoryList[hero.slotNo].quantity;
			inventoryList[hero.slotNo].quantity = 0;
		}
		if (inventoryList[hero.slotNo].quantity <= 0)
			inventoryList[hero.slotNo].quantity = 0;
	}
}

void ProcessEvents(Hero & hero, RenderWindow & window, Game & game, bool & switch_status, float & shot_last_time, Sprite & sprite_shot)
{
	Event event;
	while (window.pollEvent(event))
	{
		//update hero direction
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))  hero.dir = UPRIGHT;
			else if (Keyboard::isKeyPressed(Keyboard::Left))  hero.dir = UPLEFT;
			else hero.dir = UP;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (Keyboard::isKeyPressed(Keyboard::Left)) hero.dir = DOWNLEFT;
			else if (Keyboard::isKeyPressed(Keyboard::Right)) hero.dir = DOWNRIGHT;
			else hero.dir = DOWN;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (Keyboard::isKeyPressed(Keyboard::Down)) hero.dir = DOWNLEFT;
			else hero.dir = LEFT;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (Keyboard::isKeyPressed(Keyboard::Down)) hero.dir = DOWNRIGHT;
			else hero.dir = RIGHT;
		}
		else hero.dir = NONE;


		//inventorySwitch
		if (switch_status == false)
		{
			if (Keyboard::isKeyPressed(Keyboard::X))
			{
				switch_status = true;
				hero.slotNo += 1;
				if (hero.slotNo >= hero.nSlots) hero.slotNo = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Z))
			{
				switch_status = true;
				hero.slotNo -= 1;
				if (hero.slotNo < 0) hero.slotNo = hero.nSlots - 1;
			}

		}
		else if ((Keyboard::isKeyPressed(Keyboard::X) || Keyboard::isKeyPressed(Keyboard::Z)) == false)
			switch_status = false;


		if (hero.dir != NONE)
		{
			hero.dirLast = hero.dir;  //update dirLast (for shooting)
			if (hero.dir != hero.dirLast)
			{
				hero.currentFrame = 0;
			}
		}
		else
		{
			hero.currentFrame = 0;
		}

		//attack
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			//question. should store current inventoryItem is hero struct? / just get item from list
			if (hero.state == BEAST)
			{
				if (game.time - hero.lastAttackTime > HERO_BEAST_ATTACK_TIME)
				{
					//hero.lastAttackTime = game.time;  (reminder)
					hero.isBeastAttack = true;
					hero.currentFrame = 0;
				}
			}
			else if (hero.state == TRANSFORMING)
			{

			}
			else if(hero.state == NORMAL)
			{
				if (inventoryList[hero.slotNo].current > 0)
				{
					if (inventoryList[hero.slotNo].name == PISTOL)
					{
						if (game.time > shot_last_time + 0.35)
						{
							AddNewShot(hero, sprite_shot);
							shot_last_time = game.time;
							inventoryList[hero.slotNo].current -= 1;
						}
					}
					else if (inventoryList[hero.slotNo].name == RIFLE)
					{
						if (game.time > shot_last_time + 0.15)
						{
							AddNewShot(hero, sprite_shot);
							shot_last_time = game.time;
							inventoryList[hero.slotNo].current -= 1;
						}
					}
					else if (inventoryList[hero.slotNo].name == DRINK)
					{
						if (game.time > shot_last_time + 0.35)
						{
							shot_last_time = game.time;
							hero.health += HP_PER_DRINK;
							if (hero.health > 100)
							{
								hero.health = 100;  //replace
							}
							inventoryList[hero.slotNo].current -= 1;
						}
					}
					else if (inventoryList[hero.slotNo].name == MIXTURE)
					{
						//if (game.time > shot_last_time + 0.35)
						if (inventoryList[hero.slotNo].current > 0)
						{
							shot_last_time = game.time;
							hero.health += HP_PER_DRINK;
							if (hero.health > 100)
							{
								hero.health = 100;  //replace
							}
							inventoryList[hero.slotNo].current -= 1;
							hero.state = TRANSFORMING;
							hero.dir = NONE;
							hero.dirLast = DOWN;
						}
					}
				}
			}
		}

		//CheckWindowClose
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		//CheckWindowClose
		if (event.type == Event::Closed)
		{
			window.close();
		}
	}
}

void HeroCollision(Hero & hero)
{
	float x = hero.sprite.getPosition().x;
	float y = hero.sprite.getPosition().y;

	//herosize
	float sizeX = hero.sprite.getGlobalBounds().width;
	float sizeY = hero.sprite.getGlobalBounds().height;


	//проверка 4 угловых точек спрайта (верхняя левая, правая, нижняя левая, правая) на вхождение в блок карты
	bool q = (TILEMAP[int(y + sizeY  - STEP) / STEP][int(x) / STEP] == 'b');
	bool w = (TILEMAP[int(y + sizeY  - STEP) / STEP][int(x + sizeX - 1) / STEP] == 'b');
	bool e = (TILEMAP[int(y + sizeY - 1) / STEP][int(x + sizeX - 1) / STEP] == 'b');
	bool r = (TILEMAP[int(y + sizeY - 1) / STEP][int(x) / STEP] == 'b');

	switch (hero.dir)
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
			x = x + 0.33* STEP_HERO;
		}
		else if (q && e)
		{
			y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
		}
		else if (q && !e) //upleft
		{
			y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
			x = x + 0.33* STEP_HERO;
		}
		else if (e && !q) //upright
		{
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
			y = y - 0.33* STEP_HERO;
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
			x = x + 0.33 * STEP_HERO;
		}
		else if (w && r)  //downright
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
		}
		else if (r && !w) //downleft
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
			x = x + 0.33 * STEP_HERO;
		}
		else if (w && !r)  //upright
		{
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
			y = y + 0.33* STEP_HERO;
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
			x = x - 0.33* STEP_HERO;
		}
		else if ((q && e) || (r && !(q || e)))  //downleft
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
			x = (int(x) / STEP) * STEP + STEP;
		}
		else if (e && !q) //downright
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
			x = x - 0.33* STEP_HERO;
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
			x = x - 0.33* STEP_HERO;
		}
		else if (r && !w)  //downleft
		{
			x = (int(x) / STEP) * STEP + STEP;
			y = y - 0.33* STEP_HERO;
		}

		break;
	case NONE:
		break;
	}

	hero.sprite.setPosition(x, y);
}

void UpdateHeroSprite(Hero & hero)
{
	if (hero.state == TRANSFORMING)
	{
		hero.sprite.setTextureRect(IntRect(11 + 37 * int(hero.currentFrame), 484, 32, 45));
		hero.currentFrame += 0.05;

		if (hero.currentFrame > 7)
		{
			hero.state = BEAST;
			hero.beastTimer = 0;
			hero.currentFrame = 0;
		}
	}
	else if (hero.state == BEAST)
	{
		if (hero.isBeastAttack)  //attacking beast animation
		{
			switch (hero.dirLast)
			{
			case UP:
				hero.sprite.setTextureRect(IntRect(355 + 40 * int(hero.currentFrame), 597, 34, 47));
				break;
			case UPRIGHT: case RIGHT: case DOWNRIGHT:
				//21 38
				hero.sprite.setTextureRect(IntRect(182 + 57 * int(hero.currentFrame), 598, 48, 52));
				break;
			case DOWN:
				hero.sprite.setTextureRect(IntRect(42 + 35 * int(hero.currentFrame), 595, 35, 54));
				break;
			case DOWNLEFT: case LEFT: case UPLEFT:
				hero.sprite.setTextureRect(IntRect(492 + 56 * int(hero.currentFrame), 601, 54, 52));
				break;
			case NONE:
				break;
			}

			hero.currentFrame += 0.2;
			if (hero.currentFrame > 2)
			{
				//TODO: deal damage after this
				hero.isBeastAttack = false;
				hero.currentFrame = 0;
			}
		}
		else  //moving beast animation
		{
			switch (hero.dir)
			{
			case UP:
				hero.sprite.setTextureRect(IntRect(326 + 40 * int(hero.currentFrame), 537, 37, 47));
				break;
			case UPRIGHT: case RIGHT: case DOWNRIGHT:
				//21 38
				hero.sprite.setTextureRect(IntRect(163 + 40 * int(hero.currentFrame), 537, 37, 47));
				break;
			case DOWN:
				hero.sprite.setTextureRect(IntRect(9 + 38 * int(hero.currentFrame), 537, 37, 47));
				break;
			case DOWNLEFT: case LEFT: case UPLEFT:
				hero.sprite.setTextureRect(IntRect(480 + 40 * int(hero.currentFrame), 537, 37, 47));
				break;
			case NONE:
				if (hero.dirLast == UP)
				{
					hero.sprite.setTextureRect(IntRect(270 + 76, 485, 36, 46));
				}
				else if (hero.dirLast == RIGHT)
				{
					hero.sprite.setTextureRect(IntRect(270 + 39, 485, 36, 46));
				}
				else if (hero.dirLast == DOWN)
				{
					hero.sprite.setTextureRect(IntRect(270, 485, 36, 46));
				}
				else if (hero.dirLast == LEFT)
				{
					hero.sprite.setTextureRect(IntRect(270 + 114, 485, 36, 46));
				}
				break;
			}

			hero.currentFrame += 0.2;

			if (hero.currentFrame > 3)
			{
				hero.currentFrame = 0;

			}
		}
	}
	else if (hero.state == NORMAL)  //normal moving animation
	{
		switch (hero.dir)
		{
		case UP:
			hero.sprite.setTextureRect(IntRect(105 + 22 * int(hero.currentFrame), 84, 21, 37));
			break;
		case UPRIGHT: case RIGHT: case DOWNRIGHT:
			//21 38
			hero.sprite.setTextureRect(IntRect(105 + 22 * int(hero.currentFrame), 44, 21, 37));
			break;
		case DOWN:
			hero.sprite.setTextureRect(IntRect(105 + 22 * int(hero.currentFrame), 4, 21, 37));
			break;
		case DOWNLEFT: case LEFT: case UPLEFT:
			hero.sprite.setTextureRect(IntRect(105 + 22 * int(hero.currentFrame), 124, 21, 37));
			break;
		case NONE:
			if (hero.dirLast == UP)
			{
				hero.sprite.setTextureRect(IntRect(86, 85, 21, 36));
			}
			else if (hero.dirLast == RIGHT)
			{
				hero.sprite.setTextureRect(IntRect(86, 45, 21, 36));
			}
			else if (hero.dirLast == DOWN)
			{
				hero.sprite.setTextureRect(IntRect(86, 5, 21, 36));
			}
			else if (hero.dirLast == LEFT)
			{
				hero.sprite.setTextureRect(IntRect(86, 125, 21, 36));
			}
			break;
		}

		hero.currentFrame += 0.2;
		if (hero.currentFrame > 4)
		{
			hero.currentFrame = 0;
		}
	}
	else if (hero.state == DAMAGED)
	{
		hero.sprite.setTextureRect(IntRect(10 + 32 * int(hero.currentFrame), 179, 32, 45));
		hero.currentFrame += 0.06;
		if (hero.currentFrame > 2)
		{
			hero.currentFrame = 0;
			hero.state = NORMAL;
		}
	}
}

void UpdateHero(Hero & hero, Game & game) //position + collision + sprite
{
	Vector2f pos = hero.sprite.getPosition();

	if (hero.state == NORMAL || hero.state == BEAST)
	{
		switch (hero.dir)
		{
		case UP:
			pos.y -= STEP_HERO;
			break;
		case UPRIGHT:
			pos.x += (0.66 * STEP_HERO);
			pos.y -= (0.66 * STEP_HERO);
			break;
		case RIGHT:
			pos.x += STEP_HERO;
			break;
		case DOWNRIGHT:
			pos.x += (0.66 * STEP_HERO);
			pos.y += (0.66 * STEP_HERO);
			break;
		case DOWN:
			pos.y += STEP_HERO;
			break;
		case DOWNLEFT:
			pos.x -= (0.66 * STEP_HERO);
			pos.y += (0.66 * STEP_HERO);
			break;
		case LEFT:
			pos.x -= STEP_HERO;
			break;
		case UPLEFT:
			pos.x -= (0.66 * STEP_HERO);
			pos.y -= (0.66 * STEP_HERO);
			break;
		case NONE:
			break;
		}
	}
	if (hero.state == BEAST)
	{
		hero.beastTimer += (game.time - hero.beastTimer);

		if (hero.beastTimer > BEAST_MAX_TIME)
		{
			hero.state = NORMAL;
			hero.currentFrame = 0;
		}
	}

	if (inventoryList[hero.slotNo].current == 0 && inventoryList[hero.slotNo].quantity > 0)
	{
		if (hero.isReloading == false)
		{
			hero.lastReloadTime = game.time;
			hero.isReloading = true;
		}
		UpdateInventory(hero, game);
	}

	UpdateHeroSprite(hero);
	hero.sprite.setPosition(pos);
	//kostil
	hero.pos = hero.sprite.getPosition();
	HeroCollision(hero);
}

bool IsShotCollision(vector<Shot>::iterator shot, Hero & hero)
{
	Vector2f shotCenter;
	shotCenter.x = shot->sprite.getGlobalBounds().width / 2 + shot->pos.x;
	shotCenter.y = shot->sprite.getGlobalBounds().height / 2 + shot->pos.y;

	//checkDeleteShot
	if (shot->distance > SHOT_MAX_DISTANCE)
	{
		return true;
	}
	else if (TILEMAP[int(shotCenter.y) / STEP][int(shotCenter.x) / STEP] == 'b')
	{
		return true;
	}
	else
	{
		for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
		{
			if (zombie->sprite.getGlobalBounds().contains(shotCenter))
			{
				zombie->health -= DMG_ITEM[inventoryList[hero.slotNo].name];
				return true;
			}
		}
	}
	return false;
}

void UpdateShots(Hero & hero) //shots position update and delete if need
{
	for (vector<Shot>::iterator shot = shotList.begin(); shot != shotList.end();)
	{


		switch (shot->dir)  //shot position update
		{
		case UP:
			shot->pos.y -= STEP_SHOT;
			break;
		case RIGHT:
			shot->pos.x += STEP_SHOT;
			break;
		case DOWN:
			shot->pos.y += STEP_SHOT;
			break;
		case LEFT:
			shot->pos.x -= STEP_SHOT;
			break;

		}
		shot->distance += STEP_SHOT;

		if (IsShotCollision(shot, hero))  //shot delete
			shot = shotList.erase(shot);
		else shot++;

	}
}
//zombies
void ZombieSpawn(int posX, int posY, Texture & texture_zombie)
{
	Zombie zombie;

	zombie.pos.x = posX;
	zombie.pos.y = posY;
	zombie.health = ZOMBIE_MAX_HP;

	zombie.texture = texture_zombie;
	zombie.sprite.setTexture(texture_zombie);
	zombie.sprite.setPosition(posX, posY);

	zombie.state = NOTSPAWNED;
	zombie.currentFrame = 0;
	zombie.attack_time = 0;
	zombie.dir = NONE;
	zombie.dirLast = NONE;
	zombie.follow = false;
	zombie.isNear = false;
	//zombie.sprite.setTextureRect(IntRect(5, 5, 30, 30));

	zombieList.push_back(zombie);
}

void ZombieCollision(vector<Zombie>::iterator & zombie) //мб объединить с функцией update? чтобы сразу здесь вычислять перемещение героя (проблема с коллизией при движении по диагонали
{
	//исправить чтобы для всех а не только преследующих
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

		//проверка 4 угловых точек спрайта (верхняя левая, правая, нижняя левая, правая) на вхождение в блок карты
		bool q = (TILEMAP[int(y + sizeY - STEP) / STEP][int(x) / STEP] == 'b');
		bool w = (TILEMAP[int(y + sizeY - STEP) / STEP][int(x + sizeX - 1) / STEP] == 'b');
		bool e = (TILEMAP[int(y + sizeY - 1) / STEP][int(x + sizeX - 1) / STEP] == 'b');
		bool r = (TILEMAP[int(y + sizeY - 1) / STEP][int(x) / STEP] == 'b');

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
				x = x + 0.33* STEP_HERO;
			}
			else if (q && e)
			{
				y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
				x = (int(x + sizeX) / STEP) * STEP - sizeX;
			}
			else if (q && !e) //upleft
			{
				y = (int(y) / STEP) * STEP + STEP - (sizeY - STEP);
				x = x + 0.33* STEP_HERO;
			}
			else if (e && !q) //upright
			{
				x = (int(x + sizeX) / STEP) * STEP - sizeX;
				y = y - 0.33* STEP_HERO;
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
				x = x + 0.33 * STEP_HERO;
			}
			else if (w && r)  //downright
			{
				y = (int(y + sizeY) / STEP) * STEP - sizeY;
				x = (int(x + sizeX) / STEP) * STEP - sizeX;
			}
			else if (r && !w) //downleft
			{
				y = (int(y + sizeY) / STEP) * STEP - sizeY;
				x = x + 0.33 * STEP_HERO;
			}
			else if (w && !r)  //upright
			{
				x = (int(x + sizeX) / STEP) * STEP - sizeX;
				y = y + 0.33* STEP_HERO;
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
				x = x - 0.33* STEP_HERO;
			}
			else if ((q && e) || (r && !(q || e)))  //downleft
			{
				y = (int(y + sizeY) / STEP) * STEP - sizeY;
				x = (int(x) / STEP) * STEP + STEP;
			}
			else if (e && !q) //downright
			{
				y = (int(y + sizeY) / STEP) * STEP - sizeY;
				x = x - 0.33* STEP_HERO;
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
				x = x - 0.33* STEP_HERO;
			}
			else if (r && !w)  //downleft
			{
				x = (int(x) / STEP) * STEP + STEP;
				y = y - 0.33* STEP_HERO;
			}

			break;
		case NONE:
			break;
		}

		zombie->pos.x = x;
		zombie->pos.y = y;
		zombie->sprite.setPosition(zombie->pos.x, zombie->pos.y);
	}
}
//TODO: SOON  - убрать аргументы ниже
void ZombieCheckFollow(vector<Zombie>::iterator zombie, Hero & hero)
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
}

void ZombieUpdatePosition(vector<Zombie>::iterator zombie)
{
	//TODO: разобраться с хранением Vector2f pos помимо спрайтов. пока исправил на спрайт
	float xZombie = zombie->sprite.getPosition().x;
	float yZombie = zombie->sprite.getPosition().y;

	int stepZ;
	if (zombie->follow == true)
		stepZ = STEP_ZOMBIE_ACTIVE;
	else stepZ = STEP_ZOMBIE;
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
		xZombie -= stepZ * 0.66;
		yZombie -= stepZ * 0.66;
		break;
	case UPRIGHT:
		xZombie += stepZ * 0.66;
		yZombie -= stepZ * 0.66;
		break;
	case DOWNLEFT:
		xZombie -= stepZ * 0.66;
		yZombie += stepZ * 0.66;
		break;
	case DOWNRIGHT:
		xZombie += stepZ * 0.66;
		yZombie += stepZ * 0.66;
		break;
	}

	//struct_zombies[z].sprite.setTextureRect(IntRect(5, 5, 30, 30));
	zombie->pos.x = xZombie;
	zombie->pos.y = yZombie;
	//struct_zombies[z].sprite.setPosition(x, y);

	//poka chto
	zombie->sprite.setPosition(zombie->pos.x, zombie->pos.y);
}

//should i store it in zombieList>? if i can just keep it here (return true)
bool IsZombieNearHero(Hero & hero, vector<Zombie>::iterator & zombie)
{
	//comparing distance between two nearest points of hero and zombie sprites    to define is zombie near or not
	//TODO: refact

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

	//cout << "ZC " << zombieCenter.y  << " HC " << heroCenter.y << endl;
	//cout << hero.dirLast << " DIRLAST" << endl;

	//if hero and zombie sprites are very close - allow Beast to attack independeltly from its direction
	if ((abs(zombieCenter.x - heroCenter.x) < distanceX) && (abs(zombieCenter.y - heroCenter.y) < distanceY))
	{
		//cout << " INDEP " << abs(zombieCenter.x - heroCenter.x) << "  XY  " << abs(zombieCenter.y - heroCenter.y) << endl;
		return true;
	}

	switch (hero.dirLast)
	{
	case UP:
		if (abs(heroCenter.x - zombieCenter.x) < dx_max && (heroCenter.y - zombieCenter.y >= 0 && heroCenter.y - zombieCenter.y < distanceY))
		{
			//cout << "UP" << endl;
			return true;
		}
		break;
	case RIGHT: case UPRIGHT: case DOWNRIGHT:
		if ((zombieCenter.x - heroCenter.x >= 0 && zombieCenter.x - heroCenter.x < distanceX) && (abs(heroCenter.y - zombieCenter.y) < dy_max))
		{
			//cout << "RIGHT" << endl;
			return true;
		}
		break;
	case DOWN:
		if (abs(zombieCenter.x - heroCenter.x) < dx_max && (zombieCenter.y - heroCenter.y >= 0 && zombieCenter.x - heroCenter.x < distanceY))
		{
			//cout << "DOWN" << endl;
			return true;
		}
		break;
	case LEFT: case UPLEFT: case DOWNLEFT:
		if ((heroCenter.x - zombieCenter.x >= 0 && zombieCenter.x - heroCenter.x < distanceX) && (abs(heroCenter.x - zombieCenter.x) < dy_max))
		{
			//cout << "LEFT" << endl;
			return true;
		}
		break;
	case NONE:
		return false;  //TOCHECK: NEED?
		break;
	}
	return false;
}

void ZombieUpdateAttack(Hero & hero, vector<Zombie>::iterator zombie, Game & game)
{
	//add correct xyHero xyZombie for detecting meleeAttack
	//TODO: change mechanics of zombie attack(depends on side from which it attacks), same for beast attack
	if ((abs(zombie->pos.x - hero.pos.x) < 7) && (abs(zombie->pos.y - hero.pos.y) < 7))
	{
		//attack
		if (zombie->attack_time < game.time - 1.5)
		{
			hero.health -= ZOMBIE_DAMAGE;
			if (hero.state != BEAST)
			{
				hero.state = DAMAGED;
				hero.currentFrame = 0;
			}
			zombie->attack_time = game.time;
			zombie->isNear = zombie->dir;  //for Beast melee attack
		}
	}
}

//TODO: dir autonomic + dir NONE for zombies
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
}

void CheckHeroBeastDamage(Hero & hero, vector<Zombie>::iterator & zombie, Game & game)  //killing zombie with melee attack
{
	if (hero.isBeastAttack && ((game.time - hero.lastAttackTime) > HERO_BEAST_ATTACK_TIME))
	{
		zombie->health -= HERO_BEAST_DAMAGE;
		hero.lastAttackTime = game.time;
	}
}

void ZombieUpdate(Hero & hero, Game & game)
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
					if ((dx > 3 && dy > 3) &&  (dx / dy > 0.9) && (dy / dx < 1.1))
					{
						cout << "MAIN" << endl;
						//cout << zombie->dir << " DIR" <<  endl;
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
						cout << "DX >+ DY " << endl;
						if (xHero > zombie->pos.x)
							dir = RIGHT;
						else
							dir = LEFT;
					}
					else if (dx < dy)
					{
						cout << " NO!!" << endl;
						if (yHero > zombie->pos.y)
							dir = DOWN;
						else
							dir = UP;
					}
				}
			}

			zombie->dir = dir;

			ZombieCollision(zombie);
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
				ZombieUpdatePosition(zombie);
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

		//zombie->dir = dir;

		//delSoon soon ???
		if (zombie->follow == false)
		{
			zombie->dir = NONE;
		}

		//zombieDelete
		if (zombie->state == EXPLODED)
		{
			zombie = zombieList.erase(zombie);
		}
		else zombie++;
	}
}

void ZombieMoveRandom()
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
}
//draw
void DrawMap(Sprite & mapSprite, RenderWindow & window)
{
	for (int i = 0; i < HEIGHT_MAP; i++)
	{
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TILEMAP[i][j] == 'b')
			{
				mapSprite.setTextureRect(IntRect(0, 0, 82, 82));    //block
			}
			if (TILEMAP[i][j] == ' ')
			{
				mapSprite.setTextureRect(IntRect(84, 0, 82, 82));   //grass
			}

			mapSprite.setPosition(j * STEP, i * STEP);
			window.draw(mapSprite);
		}
	}
}

void DrawShots(RenderWindow & window)
{
	for (vector<Shot>::iterator shot = shotList.begin(); shot != shotList.end(); ++shot)
	{
		shot->sprite.setPosition(shot->pos.x, shot->pos.y);
		window.draw(shot->sprite);
	}
}

void DrawZombies(RenderWindow & window)
{
	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
	{
		window.draw(zombie->sprite);
	}
}

void DrawInventoryText(RenderWindow & window, Text & text, const Vector2f & posView, Hero & hero)
{
	text.setPosition(posView.x + 40, posView.y + 40);

	//currentItem << (hero.iCurrent.item);
	std::ostringstream toStringCurrent;
	toStringCurrent << inventoryList[hero.slotNo].current;

	std::ostringstream toStringQuantity;
	toStringQuantity << inventoryList[hero.slotNo].quantity;
	text.setString(toStringCurrent.str() + "/" + toStringQuantity.str() + " " + ITEM_NAMES[inventoryList[hero.slotNo].name]);
	window.draw(text);
	if (hero.isReloading)
	{
		text.setPosition(posView.x + 40, posView.y + 70);
		if (inventoryList[hero.slotNo].name == PISTOL || inventoryList[hero.slotNo].name == RIFLE)
		{
			text.setString("reloading");
		}
		else if (inventoryList[hero.slotNo].name == GRENADE)
		{
			text.setString("pulling");
		}
		else
		{
			text.setString("opening");
		}
		window.draw(text);
	}
}

//TODO: change to structs
void DrawBar(RenderWindow & window, View & view, Text & text, Sprite & sprite_bar, Sprite & sprite_health, Sprite & sprite_items, Hero & hero)
{
	Vector2f posView;
	posView.x = view.getCenter().x - 600;
	posView.y = view.getCenter().y - 450;

	//center 319 238

	sprite_bar.setTextureRect(IntRect(0, 0, 170, 35));
	sprite_bar.setPosition(posView);


	//delSoon
	//REPLACE (HERO_DEATH)
	if (hero.health < 0) hero.health = 0;
	sprite_health.setTextureRect(IntRect(1, 0, 146 * (float(hero.health) / 100), 29));

	//changeSoon
	sprite_health.setPosition(posView.x + 10, posView.y + 1);

	//Item item = hero.iCurrent.item;
	inventoryList[hero.slotNo].sprite.setPosition(posView.x + 5, posView.y + 40);

	window.draw(inventoryList[hero.slotNo].sprite);

	window.draw(sprite_bar);
	window.draw(sprite_health);
	DrawInventoryText(window, text, posView, hero);
}

void GenerateLoot(int  num, NameItem  item, Texture & texture_items)
{
	int x;
	int y;
	bool need_new_block = false;

	do
	{
		x = (rand() % WIDTH_MAP) * STEP;
		y = (rand() % HEIGHT_MAP) * STEP;
		need_new_block = false;


		if (TILEMAP[y / STEP][x / STEP] == 'b') need_new_block = true;
		else
		{
			for (vector<Loot>::iterator i = lootList.begin(); i != lootList.end(); ++i)
				if (abs(i->pos.x - x) < 100 && abs(i->pos.y - y) < 100)
				{
					need_new_block = true;
					break;
				}
			if (need_new_block == false)
			{
				Loot loot;

				loot.name = item;

				//TODO: REFACT
				int quantity;
				switch (loot.name)
				{
				case DRINK:
					quantity = 1;
					break;
				case PISTOL:
					quantity = 12;
					break;
				case RIFLE:
					quantity = 30;
					break;
				case AMMO:
					quantity = 1;
					break;
				case KEY:
					quantity = 1;
					break;
				case MIXTURE:
					quantity = 1;
					break;
				case GRENADE:
					quantity = 1;
				}


				loot.quantity = quantity;
				//checkifNeedIt??
				loot.pos.x = x;
				loot.pos.y = y;
				loot.sprite.setPosition(x, y);

				loot.sprite.setTexture(texture_items);
				loot.sprite.setTextureRect(sf::IntRect(item * 32, 0, 32, 32));
				loot.isDrawn = true;

				lootList.push_back(loot);
				//che
				num -= 1;
			}
		}

	} while (num > 0);
}

/*
void SpawnZombieRandom(int num)
{

int x;
int y;
bool need_new_block = false;

do
{
x = (rand() % WIDTH_MAP) * STEP;
y = (rand() % HEIGHT_MAP) * STEP;
need_new_block = false;


if (TILEMAP[y / STEP][x / STEP] == 'b') need_new_block = true;
else
{
for (vector<Loot>::iterator i = lootList.begin(); i != lootList.end(); ++i)
if (abs(i->pos.x - x) < 100 && abs(i->pos.y - y) < 100)
{
need_new_block = true;
break;
}
if (need_new_block == false)
{
int i = 0;
//addelementZombie
//убрать
while (!(struct_zombies[i].pos.x > 0))
{

}
}
}

} while (num > 0);

}
*/

void DrawLoot(RenderWindow & window)
{
	for (vector<Loot>::iterator i = lootList.begin(); i != lootList.end(); ++i)
	{
		if (i->isDrawn == true)
			window.draw(i->sprite);
	}
}

void main()
{
	//map
	Texture texture_map;
	texture_map.loadFromFile("images/map.png");
	sf::Sprite mapSprite;
	mapSprite.setTexture(texture_map);

	//zombie
	Texture texture_zombie;
	texture_zombie.loadFromFile("images/zombie.png");
	sf::Sprite sprite_zombie;
	sprite_zombie.setTexture(texture_zombie);

	//shot
	Texture texture_shot;
	texture_shot.loadFromFile("images/shots.png");
	Sprite sprite_shot;
	sprite_shot.setTexture(texture_shot);

	//текст
	Font font;
	font.loadFromFile("Font/Arialbd.ttf");
	Text text("", font, 30);
	text.setColor(Color::White);

	//items
	Texture texture_items;
	texture_items.loadFromFile("images/items.png");
	Sprite sprite_items;
	sprite_items.setTexture(texture_items);

	//healthbar
	Texture texture_bar;
	texture_bar.loadFromFile("images/bar.png");
	sf::Sprite sprite_bar;
	sprite_bar.setTexture(texture_bar);
	//health
	Texture texture_health;
	texture_health.loadFromFile("images/health.png");
	sf::Sprite sprite_health;
	sprite_health.setTexture(texture_health);

	View view;
	Clock clock;
	Clock clock2;

	RenderWindow window(sf::VideoMode(1280, 1024), "dota 3");

	//camera
	view.reset(sf::FloatRect(0, 0, 1280, 1024));

	//initializeZombie
	ZombieSpawn(100, 100, texture_zombie);
	//ZombieSpawn(300, 700,texture_zombie);
	//ZombieSpawn(500, 500,texture_zombie);

	//initializeGame
	Game game;
	game.time = 0;
	//initializeHero(hero);
	Hero hero;
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

	//bool is_game_over = false;

	//initializeInventory
	Inventory inventory;
	inventory.name = PISTOL;
	inventory.current = 0;
	inventory.quantity = 7;
	inventory.sprite = sprite_items;
	inventory.sprite.setTextureRect(IntRect(32, 0, 32, 32));
	inventoryList.push_back(inventory);

	//initializeLoot
	GenerateLoot(5, DRINK, texture_items);
	GenerateLoot(3, PISTOL, texture_items);
	GenerateLoot(2, AMMO, texture_items);
	GenerateLoot(1, RIFLE, texture_items);
	GenerateLoot(1, KEY, texture_items);
	GenerateLoot(1, MIXTURE, texture_items);

	bool switch_status = false; //for ProcessEvents(correct inventory-switch)
								//bool fire_status = false; //для стрельбы 
	float shot_last_time = 0;

	while (window.isOpen())
	{
		window.clear();


		game.time = clock.getElapsedTime().asSeconds();
		float timeZ = clock2.getElapsedTime().asSeconds();

		cout << hero.isReloading << endl;
		//game.time = time;
		//float timeMili = clock.getElapsedTime().asMilliseconds();

		//TODO: spawn zombie at definite time (and change SpawnZombie func (spawn only near hero))
		if (timeZ > TIME_GAME_STEP)
		{

		}

		ProcessEvents(hero, window, game, switch_status, shot_last_time, sprite_shot);
		UpdateHero(hero, game);

		UpdateView(window, hero, view);

		UpdateShots(hero);
		ZombieUpdate(hero, game);

		CheckLoot(hero, texture_items);
		UpdateInventory(hero,game);


		//Drawing
		DrawMap(mapSprite, window);
		DrawLoot(window);
		DrawShots(window);

		DrawZombies(window);
		DrawHero(hero, window);
		DrawBar(window, view, text, sprite_bar, sprite_health, sprite_items, hero);

		window.display();
	}
}