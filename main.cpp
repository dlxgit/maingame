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
delSoon
//use Lists,delete elements

((необяз.) убрать?)поменять скорость при движении по диагонали у стены на равную обычной (а не 0.66)

NEXT: доработать движение зомби при коллизии  !STOP! + автономное.

new
сделать вычисление следующего направления зомби если перед ним блок (опр. кратчайший путь)

избавиться от кучи аргументов

разбить на файлы

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
};

struct Shot
{
	Vector2f pos;
	int distance;
	Direction dir;
	Sprite sprite;
};

struct Zombie
{
	Vector2f pos;
	int health;
	Direction dir;
	bool follow;
	float attack_time;
	Texture texture;
	Sprite sprite;
	bool collision;
};

struct Npc
{
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	int health;
};


std::vector<Inventory> inventoryList;
std::vector<Loot> lootList;
std::vector<Shot> shotList;
std::vector<Zombie> zombieList;
std::vector<Npc> NpcList;

Hero hero;


void DrawHero(Hero & hero, RenderWindow & window)
{
	window.draw(hero.sprite);
}

void CheckLoot(Hero & hero, Texture & texture_items)
{
	float x = hero.sprite.getPosition().x;
	float y = hero.sprite.getPosition().y;

	bool flag = false;

	for (vector<Loot>::iterator i = lootList.begin(); i != lootList.end(); ++i)
	{
		if (i->isDrawn == true)  //если спрайт героя на середине item'a
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


						//add new inventory in List
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
						//delSoon
						for (std::vector<Inventory>::iterator iter = inventoryList.begin(); iter != inventoryList.end(); ++iter)
						{
							iter->quantity += MAX_AMMO[iter->name];
							packed += 1;
						}
				}

			}
		}
	}
}

View UpdateView(RenderWindow & window, Hero & hero, View & view)
{
	float tempX = hero.sprite.getPosition().x;
	float tempY = hero.sprite.getPosition().y;//считываем коорд игрока и проверяем их, чтобы убрать края

	if (tempX < 320) tempX = 320;//убираем из вида левую сторону
	if (tempY < 240) tempY = 240;//верхнюю сторону
	if (tempY > 554) tempY = 554;//нижнюю сторону

	view.setCenter(tempX, tempY);  //следим за игроком, передавая его координаты. 
	window.setView(view);
	return view;
}

void AddNewShot(Hero & hero, float & time)
{
	cout << "SHOT!!" << endl;

	Direction dir;
	switch (hero.dirLast)
	{
	case UP:  //если герой смотрит вверх
		dir = UP;
		break;
	case UPRIGHT: case RIGHT: case DOWNRIGHT:  //если герой смотрит вправо (при движении по диагонали вправо/влево он смотрит вправо/влево, а не наискосок)
		dir = RIGHT;
		break;
	case DOWN:   //если герой смотрит вниз
		dir = DOWN;
		break;
	case DOWNLEFT: case LEFT: case UPLEFT:  //если герой смотрит влево
		dir = LEFT;
		break;
	}

	Shot shot;
	shot.pos.x = hero.sprite.getPosition().x;
	shot.pos.y = hero.sprite.getPosition().y;
	shot.distance = 0;
	shot.dir = dir;
	shotList.push_back(shot);
}

void ProcessEvents(Hero & hero, RenderWindow & window, float & time, bool & switch_status, float & shot_last_time)
{
	Event event;
	while (window.pollEvent(event))
	{
		// движение героя
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

		if (hero.dir != NONE) hero.dirLast = hero.dir;


		// attack
		if (Keyboard::isKeyPressed(Keyboard::A))
		{

			//should store current inventoryItem is hero struct? / just get item from list
			if (inventoryList[hero.slotNo].current > 0)
			{

				if (inventoryList[hero.slotNo].name == PISTOL)
				{
					if (time > shot_last_time + 0.35)
					{
						cout << " A!           A!" << endl;
						AddNewShot(hero, time);
						shot_last_time = time;
						inventoryList[hero.slotNo].current -= 1;
					}
				}
				else if (inventoryList[hero.slotNo].name == RIFLE)
				{
					if (time > shot_last_time + 0.15)
					{
						cout << " A!           A!" << endl;
						AddNewShot(hero, time);
						shot_last_time = time;
						inventoryList[hero.slotNo].current -= 1;
					}
				}
				else if (inventoryList[hero.slotNo].name == DRINK)
				{
					if (time > shot_last_time + 0.35)
					{
						shot_last_time = time;
						hero.health += HP_PER_DRINK;
						if (hero.health > 100) hero.health = 100;
						inventoryList[hero.slotNo].current -= 1;
					}
				}
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		// Окно закрыли
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
	bool q = (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b');
	bool w = (TILEMAP[int(y) / STEP][int(x + sizeX - 1) / STEP] == 'b');
	bool e = (TILEMAP[int(y + sizeY - 1) / STEP][int(x + sizeX - 1) / STEP] == 'b');
	bool r = (TILEMAP[int(y + sizeY - 1) / STEP][int(x) / STEP] == 'b');

	///cout << " BOOL !!  " << q << w << e << r << endl;

	switch (hero.dir)
	{
	case UP:
		//up left and right
		if (q || w)
		{
			y = (int(y) / STEP) * STEP + STEP;
		}
		break;
	case UPRIGHT:
		//upright except downleft

		if (w  && !(q || e))
		{
			y = (int(y) / STEP) * STEP + STEP;
			x = x + 0.33* STEPHERO;
		}
		else if (q && e)
		{
			y = (int(y) / STEP) * STEP + STEP;
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
		}
		else if (q && !e) //upleft
		{
			y = (int(y) / STEP) * STEP + STEP;
			x = x + 0.33* STEPHERO;
		}
		else if (e && !q) //upright
		{
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
			y = y - 0.33* STEPHERO;
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
			x = x + 0.33 * STEPHERO;
		}
		else if (w && r)  //downright
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
		}
		else if (r && !w) //downleft
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
			x = x + 0.33 * STEPHERO;
		}
		else if (w && !r)  //upright
		{
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
			y = y + 0.33* STEPHERO;
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
			x = x - 0.33* STEPHERO;
		}
		else if ((q && e) || (r && !(q || e)))  //downleft
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
			x = (int(x) / STEP) * STEP + STEP;
		}
		else if (e && !q) //downright
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
			x = x - 0.33* STEPHERO;
		}
		else if (q && !e) //upleft
		{
			x = (int(x) / STEP) * STEP + STEP;
			y = y + 0.33* STEPHERO;
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
			y = (int(y) / STEP) * STEP + STEP;
			x = x - 0.33* STEPHERO;
		}
		else if ((w && r) || (q && !(w || r)))   //upleft
		{
			y = (int(y) / STEP) * STEP + STEP;
			x = (int(x) / STEP) * STEP + STEP;
		}
		else if (w && !r) //upright
		{
			y = (int(y) / STEP) * STEP + STEP;
			x = x - 0.33* STEPHERO;
		}
		else if (r && !w)  //downleft
		{
			x = (int(x) / STEP) * STEP + STEP;
			y = y - 0.33* STEPHERO;
		}

		break;
	case NONE:
		break;
	}

	hero.sprite.setPosition(x, y);
}

void UpdateHeroPosition(Hero & hero)
{
	float x = hero.sprite.getPosition().x;
	float y = hero.sprite.getPosition().y;

	switch (hero.dir)
	{
	case UP:
		y -= STEPHERO;
		break;
	case UPRIGHT:
		x += (0.66 * STEPHERO);
		y -= (0.66 * STEPHERO);
		break;
	case RIGHT:
		x += STEPHERO;
		break;
	case DOWNRIGHT:
		x += (0.66 * STEPHERO);
		y += (0.66 * STEPHERO);
		break;
	case DOWN:
		y += STEPHERO;
		break;
	case DOWNLEFT:
		x -= (0.66 * STEPHERO);
		y += (0.66 * STEPHERO);
		break;
	case LEFT:
		x -= STEPHERO;
		break;
	case UPLEFT:
		x -= (0.66 * STEPHERO);
		y -= (0.66 * STEPHERO);
		break;
	case NONE:
		break;
	}
	hero.sprite.setPosition(x, y);
	HeroCollision(hero);
}

bool IsShotCollision(vector<Shot>::iterator shot)
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

void UpdateShots(float time) //обновление данных о пулях 
{
	for (vector<Shot>::iterator shot = shotList.begin(); shot != shotList.end();)
	{


		switch (shot->dir)  //обновление координат пуль на карте
		{
		case UP:
			shot->pos.y -= STEPSHOT;
			break;
		case RIGHT:
			shot->pos.x += STEPSHOT;
			break;
		case DOWN:
			shot->pos.y += STEPSHOT;
			break;
		case LEFT:
			shot->pos.x -= STEPSHOT;
			break;

		}
		shot->distance += STEPSHOT;

		//deleting
		if (IsShotCollision(shot))
			shot = shotList.erase(shot);
		else shot++;

	}
}
//zombies
void ZombieSpawn(int posX, int posY, Sprite & sprite_zombie)
{
	Zombie zombie;

	zombie.pos.x = posX;
	zombie.pos.y = posY;
	zombie.health = ZOMBIE_MAX_HP;
	zombie.texture.loadFromFile("images/zombie.png");
	zombie.sprite = sprite_zombie;
	//zombie.sprite.setTextureRect(IntRect(5, 5, 30, 30));

	zombieList.push_back(zombie);
}

void ZombieCollision(vector<Zombie>::iterator zombie) //мб объединить с функцией update? чтобы сразу здесь вычислять перемещение героя (проблема с коллизией при движении по диагонали
{
	//исправить чтобы для всех а не только преследующих
	if (zombie->follow == true)
	{

		float x = zombie->pos.x;
		float y = zombie->pos.y;

		//dunno
		float x0 = x;
		float y0 = y;

		Direction dir = zombie->dir;

		//spritesize
		float sizeX = zombie->sprite.getGlobalBounds().width;
		float sizeY = zombie->sprite.getGlobalBounds().height;


		//проверка 4 угловых точек спрайта (верхняя левая, правая, нижняя левая, правая) на вхождение в блок карты
		bool q = (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b');
		bool w = (TILEMAP[int(y) / STEP][int(x + sizeX - 1) / STEP] == 'b');
		bool e = (TILEMAP[int(y + sizeY - 1) / STEP][int(x + sizeX - 1) / STEP] == 'b');
		bool r = (TILEMAP[int(y + sizeY - 1) / STEP][int(x) / STEP] == 'b');

		//cout << "  ZOMB !!  " << q << w << e << r << endl;

		switch (dir)
		{
		case UP:
			//up left and right
			if (q || w)
			{
				y = (int(y) / STEP) * STEP + STEP;
			}
			break;
		case UPRIGHT:
			//upright except downleft

			if (w  && !(q || e))
			{
				y = (int(y) / STEP) * STEP + STEP;
				x = x + 0.33* STEPHERO;
			}
			else if (q && e)
			{
				y = (int(y) / STEP) * STEP + STEP;
				x = (int(x + sizeX) / STEP) * STEP - sizeX;
			}
			else if (q && !e) //upleft
			{
				y = (int(y) / STEP) * STEP + STEP;
				x = x + 0.33* STEPHERO;
			}
			else if (e && !q) //upright
			{
				x = (int(x + sizeX) / STEP) * STEP - sizeX;
				y = y - 0.33* STEPHERO;
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
				x = x + 0.33 * STEPHERO;
			}
			else if (w && r)  //downright
			{
				y = (int(y + sizeY) / STEP) * STEP - sizeY;
				x = (int(x + sizeX) / STEP) * STEP - sizeX;
			}
			else if (r && !w) //downleft
			{
				y = (int(y + sizeY) / STEP) * STEP - sizeY;
				x = x + 0.33 * STEPHERO;
			}
			else if (w && !r)  //upright
			{
				x = (int(x + sizeX) / STEP) * STEP - sizeX;
				y = y + 0.33* STEPHERO;
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
				x = x - 0.33* STEPHERO;
			}
			else if ((q && e) || (r && !(q || e)))  //downleft
			{
				y = (int(y + sizeY) / STEP) * STEP - sizeY;
				x = (int(x) / STEP) * STEP + STEP;
			}
			else if (e && !q) //downright
			{
				y = (int(y + sizeY) / STEP) * STEP - sizeY;
				x = x - 0.33* STEPHERO;
			}
			else if (q && !e) //upleft
			{
				x = (int(x) / STEP) * STEP + STEP;
				y = y + 0.33* STEPHERO;
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
				y = (int(y) / STEP) * STEP + STEP;
				x = x - 0.33* STEPHERO;
			}
			else if ((w && r) || (q && !(w || r)))   //upleft
			{
				y = (int(y) / STEP) * STEP + STEP;
				x = (int(x) / STEP) * STEP + STEP;
			}
			else if (w && !r) //upright
			{
				y = (int(y) / STEP) * STEP + STEP;
				x = x - 0.33* STEPHERO;
			}
			else if (r && !w)  //downleft
			{
				x = (int(x) / STEP) * STEP + STEP;
				y = y - 0.33* STEPHERO;
			}

			break;
		case NONE:
			break;
		}

		zombie->pos.x = x;
		zombie->pos.y = y;
		zombie->dir = dir;
		zombie->sprite.setPosition(zombie->pos.x, zombie->pos.y);
	}
}

void ZombieCheckFollow(Hero & hero, float xHero, float yHero)
{
	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
	{
		float xZombie = zombie->pos.x;
		float yZombie = zombie->pos.y;

		if (abs(xZombie - xHero) > ZOMBIE_VISION_DISTANCE || abs(yZombie - yHero) > ZOMBIE_VISION_DISTANCE)
			zombie->follow = false;

		if (zombie->follow == false)
		{
			if (abs(xZombie - xHero) < ZOMBIE_VISION_DISTANCE && abs(yZombie - yHero) < ZOMBIE_VISION_DISTANCE)
				zombie->follow = true;
		}
	}
}

void ZombieUpdatePosition(vector<Zombie>::iterator zombie)
{
	float xZombie = zombie->pos.x;
	float yZombie = zombie->pos.y;

	int stepZ;
	if (zombie->follow == true)
		stepZ = STEPZOMBIE_ACTIVE;
	else stepZ = STEPZOMBIE;
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

void ZombieUpdateAttack(Hero & hero, vector<Zombie>::iterator zombie, float & time)
{
	//add correct xyHero xyZombie for detecting meleeAttack
	if ((zombie->pos.x < hero.pos.x + 32) && (zombie->pos.x) >(hero.pos.x))
	{
		//attack
		if (zombie->attack_time < time - 1.5)
		{
			hero.health -= ZOMBIE_DAMAGE;
			zombie->attack_time = time;
		}
	}
}

void ZombieUpdate(Hero & hero, float & time)
{
	float xHero = hero.sprite.getPosition().x;
	float yHero = hero.sprite.getPosition().y;

	ZombieCheckFollow(hero, xHero, yHero);

	std::cout << "  " << endl;

	//iterating zombieList
	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end();)
	{
		Direction dir = zombie->dir;

		int xZombie = zombie->pos.x;
		int yZombie = zombie->pos.y;

		//compute distance and dir
		float dx = abs(xHero - xZombie);
		float dy = abs(yHero - yZombie);
		//float di = sqrt(pow(dx,2) + pow(dy,2));

		if (zombie->follow)
		{
			if ((dx / dy) > 0.85 && (dy / dx) < 1.15)
			{
				if (xHero >= xZombie && yHero >= yZombie)
					dir = DOWNRIGHT;
				else if (xHero >= xZombie && yHero < yZombie)
					dir = UPRIGHT;
				else if (xHero < xZombie && yHero >= yZombie)
					dir = DOWNLEFT;
				else if (xHero < xZombie && yHero < yZombie)
					dir = UPLEFT;
			}
			else if (dx >= dy)
			{
				if (xHero > xZombie)
					dir = RIGHT;
				else
					dir = LEFT;
			}
			else if (dx < dy)
			{
				if (yHero > yZombie)
					dir = DOWN;
				else
					dir = UP;
			}
		}

		if (xHero != xZombie || yHero != yZombie)
		{
			ZombieUpdatePosition(zombie);
		}

		ZombieCollision(zombie);
		ZombieUpdateAttack(hero, zombie, time);
		//cout << dir << "  dir" << endl;

		//UpdateZombiePosition(i);  make it for all zombies, not jsut for following ones
		zombie->dir = dir;

		//remove soon
		if (zombie->follow == false)
			zombie->dir = NONE;

		if (zombie->health < 1)
		{
			zombie = zombieList.erase(zombie);
		}
		//std::cout << zombie->health << ' ';
		else zombie++;
	}
	std::cout << "  " << endl;
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
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if (TILEMAP[i][j] == 'b')  mapSprite.setTextureRect(IntRect(0, 0, 82, 82)); //стена
			if (TILEMAP[i][j] == ' ')  mapSprite.setTextureRect(IntRect(84, 0, 82, 82));//трава

			mapSprite.setPosition(j * STEP, i * STEP);  //по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат
			window.draw(mapSprite);//рисуем квадратики на экран
		}
}

void DrawShots(Sprite & sprite_shot, RenderWindow & window)
{
	for (vector<Shot>::iterator shot = shotList.begin(); shot != shotList.end(); ++shot)
		if (shot->dir > 0)
		{
			sprite_shot.setPosition(shot->pos.x, shot->pos.y);
			window.draw(sprite_shot);
		}
}

void DrawZombies(RenderWindow & window)
{
	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
	{
		window.draw(zombie->sprite);
	}
}

void DrawText(RenderWindow & window, Text & text, float x, float y)
{
	text.setPosition(x + 40, y + 40);

	String str;
	switch (inventoryList[hero.slotNo].name)
	{
	case PISTOL:
		str = "pistol";
		break;
	case RIFLE:
		str = "rifle";
		break;
	case GRENADE:
		str = "grenade";
		break;
	case KEY:
		str = "key";
		break;
	case DRINK:
		str = "drink";
		break;
	case MIXTURE:
		str = "mixture";
		break;
	}

	//currentItem << (hero.iCurrent.item);
	std::ostringstream toStringCurrent;
	toStringCurrent << inventoryList[hero.slotNo].current;

	std::ostringstream toStringQuantity;
	toStringQuantity << inventoryList[hero.slotNo].quantity;
	text.setString(toStringCurrent.str() + "/" + toStringQuantity.str() + " " + str);

	window.draw(text);//рисую этот текст
	window.draw(text);//рисую этот текст
}

//переписать на структуры

void DrawBar(RenderWindow & window, View & view, Text & text, Sprite & sprite_bar, Sprite & sprite_health, Sprite & sprite_items)
{
	float x = view.getCenter().x - 600;
	float y = view.getCenter().y - 450;

	//center 319 238

	sprite_bar.setTextureRect(IntRect(0, 0, 170, 35));
	sprite_bar.setPosition(x, y);


	//delSoon

	if (hero.health < 0) hero.health = 0;
	sprite_health.setTextureRect(IntRect(1, 0, 146 * (float(hero.health) / 100), 29));

	sprite_health.setPosition(x + 10, y + 1);

	//Item item = hero.iCurrent.item;
	inventoryList[hero.slotNo].sprite.setPosition(x + 5, y + 40);

	window.draw(inventoryList[hero.slotNo].sprite);

	window.draw(sprite_bar);
	window.draw(sprite_health);
	DrawText(window, text, x, y);
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

				//быдлокод
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
				case KEY:
					quantity = 1;
					break;
				case MIXTURE:
					quantity = 1;
					break;
				case GRENADE:
					quantity = 1;
				case AMMO:
					quantity = 1;
					break;

				}


				loot.quantity = quantity;
				//nado li
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

void UpdateInventory()
{
	//update items 
	for (std::vector<Inventory>::iterator iter = inventoryList.begin(); iter != inventoryList.end(); ++iter)
	{
		if (iter->current == 0)
		{
			if (iter->quantity >= MAX_AMMO[iter->name])
			{
				iter->quantity -= MAX_AMMO[iter->name];
				iter->current = MAX_AMMO[iter->name];
			}
			else
			{
				iter->current = iter->quantity;
				iter->quantity = 0;
			}
			if (iter->quantity <= 0)
				iter->quantity = 0;
		}
	}
}

void main()
{

	//map
	Texture texture_map;
	texture_map.loadFromFile("images/map.png");
	sf::Sprite mapSprite;
	mapSprite.setTexture(texture_map);

	//hero
	Texture texture_hero;
	texture_hero.loadFromFile("images/hero.png");
	sf::Sprite heroSprite;
	heroSprite.setTexture(texture_hero);
	heroSprite.setTextureRect(IntRect(4, 4, 32, 32));

	//zombie
	Texture texture_zombie;
	texture_zombie.loadFromFile("images/zombie.png");
	sf::Sprite sprite_zombie;
	sprite_zombie.setTexture(texture_zombie);
	//sprite_zombie.setTextureRect(IntRect(5, 5, 30, 30));
	//sprite_zombie.setPosition(320, 320);

	//shot
	Texture texture_shot;
	texture_shot.loadFromFile("images/shot.png");
	Sprite sprite_shot;
	sprite_shot.setTexture(texture_shot);
	//shot.setPosition(position_hero)

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
	ZombieSpawn(100, 100,sprite_zombie);
	ZombieSpawn(300, 700,sprite_zombie);
	ZombieSpawn(500, 500, sprite_zombie);

	//initializeHero(hero);
	hero.slotNo = 0;
	hero.nSlots = 1;
	hero.health = 100;
	hero.texture.loadFromFile("images/hero.png");
	hero.sprite.setTexture(hero.texture);
	hero.sprite.setTextureRect(IntRect(4, 4, 32, 32));
	hero.dir = NONE;
	hero.dirLast = NONE;
	hero.sprite.setPosition(6 * 32, 9 * 32);  //start position


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

	bool switch_status = false; //для ProcessEvents (корректное переключение оружия)
	//bool fire_status = false; //для стрельбы 
	float shot_last_time = 0;
	while (window.isOpen())
	{

		cout << inventoryList[0].current << " " << inventoryList[0].quantity << endl;

		window.clear();

		float time = clock.getElapsedTime().asSeconds();
		float timeZ = clock2.getElapsedTime().asSeconds();
		//float timeMili = clock.getElapsedTime().asMilliseconds();

		if (timeZ > TIME_GAME_STEP)
		{


		}

		ProcessEvents(hero, window, time, switch_status, shot_last_time);    //main
		UpdateHeroPosition(hero);

		UpdateView(window, hero, view);

		UpdateShots(time);
		ZombieUpdate(hero, time);

		CheckLoot(hero, texture_items);
		UpdateInventory();

		//cout << zombieList[0].health << endl;

		//Drawing
		DrawMap(mapSprite, window);
		DrawLoot(window);
		DrawHero(hero, window);
		DrawShots(sprite_shot, window);
		DrawZombies(window);
		DrawBar(window, view, text, sprite_bar, sprite_health, sprite_items);

		//cout << hero.slotNo << hero.nSlots << "  " << hero.iCurrent.current << endl;
		//cout << struct_zombies[0].x << "  " << struct_zombies[0].y << endl;
		//cout << "HERO  " << hero.getPosition().x  << "  " << hero.getPosition().y << endl;
		window.display();
	}
}