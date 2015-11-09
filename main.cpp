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
//(almost)сделать чтобы лишь в одной функции перебирались в цикле зомби, и применить туда остальные функции (внутри цикла)
*/

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

Hero hero;


//movespeed
const int STEPHERO = 6;
const int STEPZOMBIE = 2;
const int STEPZOMBIE_ACTIVE = 2;
const int STEPSHOT = 12;

const int ZOMBIE_DAMAGE = 30;
const int ZOMBIE_MAX_HP = 100;
const int ZOMBIE_VISION_DISTANCE = 300;

const int HP_PER_DRINK = 30;

const int SHOT_MAX_DISTANCE = 400;

const int DMG_ITEM[7] = { 0, 35, 50,0, 0,0,100 };
/*
const int MAX_AMMO_PISTOL = 12;
const int MAX_AMMO_RIFLE = 30;
const int MAX_AMMO_DRINK = 1;
const int MAX_AMMO_KEY = 1;
const int MAX_AMMO_MIXTURE = 1;
const int MAX_AMMO_GRENADE = 1;
*/

const int AMMO_PACKS = 4;
const int MAX_AMMO[7] = { 1,12,30,1,1,1,1 };

//deleteSoon
int len_struct_shots = 15;
int len_struct_zombies = 3;
int len_struct_loot = 0;


struct
{
	int x;
	int y;
} struct_NPC[10];

struct Zombies
{
	Vector2f pos;
	int health;
	Direction dir;
	bool follow;
	float attack_time;
	Texture texture;
	Sprite sprite;
	bool collision;
} struct_zombies[15];

struct
{
	Vector2f pos;
	int distance;
	Direction dir;
	Sprite sprite;
} struct_shots[20];



std::vector<Inventory> inventoryList;
std::vector<Loot> lootList;


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

					for (std::vector<Inventory>::iterator j = inventoryList.begin(); j != inventoryList.end(); ++iter)
					{
						if (j->name == i->name)
						{

							//iter->item = lootList[i].item;
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
						inventory.name = lootList[i].name;
						inventory.quantity = lootList[i].quantity;
						inventory.current = 0;
						inventory.sprite.setTexture(texture_items);
						inventory.sprite.setTextureRect(sf::IntRect(lootList[i].name * 32, 0, 32, 32));
						inventoryList.push_back(inventory);


						//che za 
						lootList[i].isDrawn = false;
						hero.nSlots += 1;
					}
				}
				else
				{
					lootList[i].isDrawn = false;
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
	bool found = false;
	for (int i = 0; found == false; i++)
		if (struct_shots[i].pos.x == 0)   //если нашли пустую ячейку
		{
			found = true;
			struct_shots[i].pos.x = hero.sprite.getPosition().x;
			struct_shots[i].pos.y = hero.sprite.getPosition().y;

			cout << "SHOT!!" << endl;

			struct_shots[i].distance = 0;

			switch (hero.dirLast)
			{
			case UP:  //если герой смотрит вверх
				struct_shots[i].dir = UP;
				break;
			case UPRIGHT: case RIGHT: case DOWNRIGHT:  //если герой смотрит вправо (при движении по диагонали вправо/влево он смотрит вправо/влево, а не наискосок)
				struct_shots[i].dir = RIGHT;
				break;
			case DOWN:   //если герой смотрит вниз
				struct_shots[i].dir = DOWN;
				break;
			case DOWNLEFT: case LEFT: case UPLEFT:  //если герой смотрит влево
				struct_shots[i].dir = LEFT;
				break;
			}

			break;   //выйти из цикла как только нашли пустую ячейку
		}
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
//shots
void DeleteShot(int i)
{
	struct_shots[i].dir = NONE;
	struct_shots[i].pos.x = 0;
	struct_shots[i].pos.y = 0;
	struct_shots[i].distance = 0;
}

void ShotCollision(int i)
{
	Vector2f shotCenter;
	shotCenter.x = struct_shots[i].sprite.getGlobalBounds().width / 2 + struct_shots[i].pos.x;
	shotCenter.y = struct_shots[i].sprite.getGlobalBounds().height / 2 + struct_shots[i].pos.y;


	//checkDeleteShot
	if (struct_shots[i].distance > SHOT_MAX_DISTANCE)
	{
		cout << "CLEAR" << endl;
		DeleteShot(i);
	}
	else if (TILEMAP[int(shotCenter.y) / STEP][int(shotCenter.x) / STEP] == 'b')
	{
		DeleteShot(i);
	}
	else
	{
		for (int n = 0; n < len_struct_zombies; n++)
		{
			if (struct_zombies[n].sprite.getGlobalBounds().contains(shotCenter))
			{
				DeleteShot(i);
				struct_zombies[n].health -= DMG_ITEM[inventoryList[hero.slotNo].name];
			}
		}
	}
}

void UpdateShots(float time) //обновление данных о пулях 
{
	for (int i = 0; i < len_struct_shots; i++)
	{
		if (struct_shots[i].dir != NONE)
		{
			switch (struct_shots[i].dir)  //обновление координат пуль на карте
			{
			case UP:
				struct_shots[i].pos.y -= STEPSHOT;
				break;
			case RIGHT:
				struct_shots[i].pos.x += STEPSHOT;
				break;
			case DOWN:
				struct_shots[i].pos.y += STEPSHOT;
				break;
			case LEFT:
				struct_shots[i].pos.x -= STEPSHOT;
				break;

			}
			struct_shots[i].distance += STEPSHOT;

			ShotCollision(i);
		}
	}
}
//zombies
void ZombieSpawn(int posX, int posY)
{
	int i = 0;
	while (struct_zombies[i].health > 0)
		i += 1;

	struct_zombies[i].pos.x = posX;
	struct_zombies[i].pos.y = posY;
	struct_zombies[i].health = ZOMBIE_MAX_HP;
	struct_zombies[i].texture.loadFromFile("images/zombie.png");
	struct_zombies[i].sprite.setTexture(struct_zombies[i].texture);
}

void ZombieCollision(int n, Sprite & sprite_zombie) //ф-ция взаимодействия с картой      мб объединить с функцией update? чтобы сразу здесь вычислять перемещение героя (проблема с коллизией при движении по диагонали
{
	//исправить чтобы для всех а не только преследующих
	if (struct_zombies[n].follow == true)
	{

		float x = struct_zombies[n].pos.x;
		float y = struct_zombies[n].pos.y;

		//dunno
		float x0 = x;
		float y0 = y;

		Direction dir = struct_zombies[n].dir;

		//spritesize
		float sizeX = sprite_zombie.getGlobalBounds().width;
		float sizeY = sprite_zombie.getGlobalBounds().height;


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

		struct_zombies[n].pos.x = x;
		struct_zombies[n].pos.y = y;
		struct_zombies[n].dir = dir;
		struct_zombies[n].sprite.setPosition(struct_zombies[n].pos.x, struct_zombies[n].pos.y);
	}
}

void ZombieCheckFollow(Hero & hero, float xHero, float yHero)
{
	for (int i = 0; i < len_struct_zombies; i++)
	{
		float xZombie = struct_zombies[i].pos.x;
		float yZombie = struct_zombies[i].pos.y;


		if (abs(xZombie - xHero) > ZOMBIE_VISION_DISTANCE || abs(yZombie - yHero) > ZOMBIE_VISION_DISTANCE)
			struct_zombies[i].follow = false;


		if (struct_zombies[i].follow == false)
		{
			if (abs(xZombie - xHero) < ZOMBIE_VISION_DISTANCE && abs(yZombie - yHero) < ZOMBIE_VISION_DISTANCE)
				struct_zombies[i].follow = true;
		}
	}
}

void ZombieUpdatePosition(int z)
{
	float xZombie = struct_zombies[z].pos.x;
	float yZombie = struct_zombies[z].pos.y;

	int stepZ;
	if (struct_zombies[z].follow == true)
		stepZ = STEPZOMBIE_ACTIVE;
	else stepZ = STEPZOMBIE;
	switch (struct_zombies[z].dir)
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
	struct_zombies[z].pos.x = xZombie;
	struct_zombies[z].pos.y = yZombie;
	//struct_zombies[z].sprite.setPosition(x, y);

	//poka chto
	struct_zombies[z].sprite.setPosition(struct_zombies[z].pos.x, struct_zombies[z].pos.y);
}

void ZombieUpdateAttack(int n, float xZombie, float yZombie, Hero & hero, float & xHero, float & yHero, float & time)
{
	//add correct xyHero xyZombie for detecting meleeAttack
	if ((xZombie < xHero + 32) && (xZombie) >(xHero))
	{
		//attack
		if (struct_zombies[n].attack_time < time - 1.5)
		{
			hero.health -= ZOMBIE_DAMAGE;
			struct_zombies[n].attack_time = time;
		}
	}
}

void ZombieUpdate(Hero & hero, Sprite & sprite_zombie, float & time)
{
	float xHero = hero.sprite.getPosition().x;
	float yHero = hero.sprite.getPosition().y;

	ZombieCheckFollow(hero, xHero, yHero);

	std::cout << "  " << endl;

	for (int i = 0; i < len_struct_zombies - 2; i++)
	{
		Direction dir = struct_zombies[i].dir;

		int xZombie = struct_zombies[i].pos.x;
		int yZombie = struct_zombies[i].pos.y;

		//compute distance and dir
		float dx = abs(xHero - xZombie);
		float dy = abs(yHero - yZombie);
		//float di = sqrt(pow(dx,2) + pow(dy,2));

		if (struct_zombies[i].follow)
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

			ZombieUpdatePosition(i);

		ZombieCollision(i, sprite_zombie);
		if (struct_zombies[i].health > 0)
			ZombieUpdateAttack(i, xZombie, yZombie, hero, xHero, yHero, time);
		//cout << dir << "  dir" << endl;

		//UpdateZombiePosition(i);  make it for all zombies, not jsut for following ones
		struct_zombies[i].dir = dir;

		//remove soon
		if (struct_zombies[i].follow == false)
			struct_zombies[i].dir = NONE;

		std::cout << struct_zombies[i].health << ' ';
	}
	std::cout << "  " << endl;
}

void ZombieMoveRandom()
{
	int rand_dir;
	Direction dir_zombie;

	for (int i = 0; i < len_struct_zombies; i++)
	{
		if (struct_zombies[i].dir == NONE)
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
	for (int i = 0; i < len_struct_shots; i++)
		if (struct_shots[i].dir > 0)
		{
			sprite_shot.setPosition(struct_shots[i].pos.x, struct_shots[i].pos.y);
			window.draw(sprite_shot);

			//реализовано в функции UpdateShots
			/*
			for (int z = 0; struct_zombies[z].health >0; z++)
				if (sprite_shot.getGlobalBounds().contains(struct_zombies[z].sprite.getPosition()))
					struct_zombies[z].health -=  DMG_ITEM[hero.iCurrent.item];
					*/ 
			//last.   исправить везде считывание элементов структур (не по len_struct, и не по HP.) 
			//хп падает, но элемент не удаляется, (просто не рисуется), и не происходит самого процесса убийства зомби.
		}
}

void DrawZombies(Sprite & sprite_zombie, RenderWindow & window)
{
	for (int i = 0; struct_zombies[i].health > 0; i++)
		//if (struct_zombies[i].dir > 0)
	{
		//sprite_zombie.setPosition(struct_zombies[i].x, struct_zombies[i].y);
		window.draw(struct_zombies[i].sprite);
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
			{
				if (abs(i->pos.x - x) < 100 && abs(i->pos.y - y) < 100)
				{
					need_new_block = true;
					break;
				}
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
				//
				len_struct_loot += 1;
				num -= 1;
			}
		}

	} while (num > 0);
}

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
	//zombie.setTextureRect(IntRect(5, 5, 30, 30))
	sprite_zombie.setPosition(320, 320);

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
	ZombieSpawn(500, 200);
	ZombieSpawn(300, 700);

	//initializeHero(hero);
	hero.slotNo = 0;
	hero.nSlots = 1;
	hero.health = 100;
	hero.slotNo = 0;
	hero.nSlots = 1;
	hero.texture.loadFromFile("images/hero.png");
	hero.sprite.setTexture(hero.texture);
	hero.sprite.setTextureRect(IntRect(4, 4, 32, 32));
	hero.dir = NONE;
	hero.dirLast;
	hero.sprite.setPosition(6 * 32, 9 * 32);  //start position


	//bool is_game_over = false;
	
	//initializeInventory
	Inventory inventory;
	inventory.name = PISTOL;
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
		window.clear();

		float time = clock.getElapsedTime().asSeconds();
		float timeZ  = clock2.getElapsedTime().asSeconds();
		//float timeMili = clock.getElapsedTime().asMilliseconds();

		if (timeZ > TIME_GAME_STEP)
		{
			
			
		}

		ProcessEvents(hero, window, time, switch_status, shot_last_time);    //main
		UpdateHeroPosition(hero);

		UpdateView(window, hero, view);

		UpdateShots(time);
		ZombieUpdate(hero, sprite_zombie, time);

		CheckLoot(hero, texture_items);
		UpdateInventory();

		cout << struct_zombies[0].health << endl;

		//Drawing
		DrawMap(mapSprite, window);
		DrawLoot(window);
		DrawHero(hero, window);
		DrawShots(sprite_shot, window);
		DrawZombies(sprite_zombie, window);
		DrawBar(window, view, text, sprite_bar, sprite_health, sprite_items);
		
		//cout << hero.slotNo << hero.nSlots << "  " << hero.iCurrent.current << endl;
		//cout << struct_zombies[0].x << "  " << struct_zombies[0].y << endl;
		//cout << "HERO  " << hero.getPosition().x  << "  " << hero.getPosition().y << endl;
		window.display();
	}
}