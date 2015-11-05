#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cmath>

#include "map.h"

/*

delSoon
избавиться от обхода по массиву через len_struct...
//удалять элементы структуры полностью

((необяз.) убрать?)поменять скорость при движении по диагонали у стены на равную обычной (а не 0.66)
сделать 2 шага зомби: 1 для сагреного и другой для обычного.

NEXT: доработать движение зомби при коллизии  !STOP! + автономное.

new
сделать вычисление следующего направления зомби если перед ним блок (опр. кратчайший путь)


избавиться от кучи аргументов

*/

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

enum Item
{
	DRINK,
	PISTOL,
	RIFLE,
	AMMO,
	KEY,
	MIXTURE,
	GRENADE,
};

const int STEPHERO = 6;
const int STEPZOMBIE = 4;
const int STEPZOMBIE_ACTIVE = 5;
const int STEPSHOT = 12;

const int ZOMBIE_DAMAGE = 30;
const int ZOMBIE_MAX_HP = 100;

const int VISION_DISTANCE = 300;

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


int Health_Hero = 100;
//bool is_game_over = false;

//new
int IndexItem = 0;
int InventorySlots = 1;

//not using
struct
{
	int x;
	int y;
} guns[15];  //положение на карте

struct
{
	int x;
	int y;
} struct_NPC[10];


struct
{
	float x;
	float y;
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
	Vector2i pos;
	int distance;
	Direction dir;
} struct_shots[20];

struct
{
	Item item;
	int quantity;
	Vector2f pos;
	Texture texture;
	Sprite sprite;
	bool draw;
}struct_loot[50];

struct
{
	Item item;
	int quantity;
	int current;
	Sprite sprite;
}struct_inventory[15];

//not using
float getplayercoordinateX(Sprite & hero)
{

	return hero.getPosition().x;
}

float getplayercoordinateY(Sprite & hero)
{

	return hero.getPosition().y;
}

View UpdateView(RenderWindow & window, Sprite & hero, View & view)
{
	float tempX = hero.getPosition().x;
	float tempY = hero.getPosition().y;//считываем коорд игрока и проверяем их, чтобы убрать края

	if (tempX < 320) tempX = 320;//убираем из вида левую сторону
	if (tempY < 240) tempY = 240;//верхнюю сторону
	if (tempY > 554) tempY = 554;//нижнюю сторону

	view.setCenter(tempX, tempY);  //следим за игроком, передавая его координаты. 
	window.setView(view);
	return view;
}

void Update(Sprite & hero, Direction dir)
{

	float x = hero.getPosition().x;
	float y = hero.getPosition().y;

	switch (dir)
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

	hero.setPosition(x, y);
}

void AddNewShot(Sprite hero, Direction & dir_last, float & time)
{
	bool found = false;
	for (int i = 0; found == false; i++)
		if (struct_shots[i].pos.x == 0)   //если нашли пустую ячейку
		{
			found = true;
			struct_shots[i].pos.x = hero.getPosition().x;
			struct_shots[i].pos.y = hero.getPosition().y;

			cout << "SHOT!!" << endl;

			struct_shots[i].distance = 0;

			switch (dir_last)
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

void ProcessEvents(RenderWindow & window, Direction & dir, Direction & dir_last, Sprite & hero, float & time, bool & switch_status, float & shot_last_time)
{
	Event event;
	while (window.pollEvent(event))
	{
		// движение героя
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (Keyboard::isKeyPressed(Keyboard::Right))  dir = UPRIGHT;
			else if (Keyboard::isKeyPressed(Keyboard::Left))  dir = UPLEFT;
			else dir = UP;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (Keyboard::isKeyPressed(Keyboard::Left)) dir = DOWNLEFT;
			else if (Keyboard::isKeyPressed(Keyboard::Right)) dir = DOWNRIGHT;
			else dir = DOWN;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (Keyboard::isKeyPressed(Keyboard::Down)) dir = DOWNLEFT;
			else dir = LEFT;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (Keyboard::isKeyPressed(Keyboard::Down)) dir = DOWNRIGHT;
			else dir = RIGHT;
		}
		else dir = NONE;


		//inventorySwitch
		if (switch_status == false)
		{
			if (Keyboard::isKeyPressed(Keyboard::X))
			{
				switch_status = true;
				IndexItem += 1;
				if (IndexItem >= InventorySlots) IndexItem = 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Z))
			{
				switch_status = true;
				IndexItem -= 1;
				if (IndexItem < 0) IndexItem = InventorySlots - 1;
			}

		}
		else if ((Keyboard::isKeyPressed(Keyboard::X) || Keyboard::isKeyPressed(Keyboard::Z)) == false)
			switch_status = false;

		if (dir != NONE) dir_last = dir;


		// attack
		if (Keyboard::isKeyPressed(Keyboard::A))
		{

			if (struct_inventory[IndexItem].current > 0)
			{
				//if (struct_inventory[IndexItem].current = 0)

				if (struct_inventory[IndexItem].item == PISTOL)
				{
					if (time > shot_last_time + 0.35)
					{
						cout << " A!           A!" << endl;
						AddNewShot(hero, dir_last, time);
						shot_last_time = time;
						struct_inventory[IndexItem].current -= 1;
					}
				}
				else if (struct_inventory[IndexItem].item == DRINK)
				{
					if (time > shot_last_time + 0.35)
					{
						shot_last_time = time;
						Health_Hero += HP_PER_DRINK;
						if (Health_Hero > 100) Health_Hero = 100;
						struct_inventory[IndexItem].current -= 1;
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

void HeroCollision(Sprite & hero, Direction & dir) //ф-ция взаимодействия с картой      мб объединить с функцией update? чтобы сразу здесь вычислять перемещение героя (проблема с коллизией при движении по диагонали
{
	float x = hero.getPosition().x;
	float y = hero.getPosition().y;

	//herosize
	float sizeX = hero.getGlobalBounds().width;
	float sizeY = hero.getGlobalBounds().height;


	//проверка 4 угловых точек спрайта (верхняя левая, правая, нижняя левая, правая) на вхождение в блок карты
	bool q = (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b');
	bool w = (TILEMAP[int(y) / STEP][int(x + sizeX - 1) / STEP] == 'b');
	bool e = (TILEMAP[int(y + sizeY - 1) / STEP][int(x + sizeX - 1) / STEP] == 'b');
	bool r = (TILEMAP[int(y + sizeY - 1) / STEP][int(x) / STEP] == 'b');

	///cout << " BOOL !!  " << q << w << e << r << endl;

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

	hero.setPosition(x, y);
}
//shots

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

			if (struct_shots[i].distance > SHOT_MAX_DISTANCE)
			{
				cout << "CLEAR" << endl;
				struct_shots[i].dir = NONE;
				struct_shots[i].pos.x = 0;
				struct_shots[i].pos.y = 0;
				struct_shots[i].distance = 0;
			}
		}
	}
}

//zombies

void ZombieSpawn(int posX, int posY)
{
	int i = 0;
	while (struct_zombies[i].health > 0)
		i += 1;

	struct_zombies[i].x = posX;
	struct_zombies[i].y = posY;
	struct_zombies[i].health = ZOMBIE_MAX_HP;
	struct_zombies[i].texture.loadFromFile("images/zombie.png");
	struct_zombies[i].sprite.setTexture(struct_zombies[i].texture);
}

void ZombieCollision(int n, Sprite & sprite_zombie) //ф-ция взаимодействия с картой      мб объединить с функцией update? чтобы сразу здесь вычислять перемещение героя (проблема с коллизией при движении по диагонали
{
	//исправить чтобы для всех а не только преследующих
	if (struct_zombies[n].follow == true)
	{

		float x = struct_zombies[n].x;
		float y = struct_zombies[n].y;

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

		struct_zombies[n].x = x;
		struct_zombies[n].y = y;
		struct_zombies[n].dir = dir;
		struct_zombies[n].sprite.setPosition(struct_zombies[n].x, struct_zombies[n].y);
	}
}

void ZombieCheckFollow(Sprite & hero, float xHero, float yHero)
{
	for (int i = 0; i < len_struct_zombies; i++)
	{
		float xZombie = struct_zombies[i].x;
		float yZombie = struct_zombies[i].y;


		if (abs(xZombie - xHero) > VISION_DISTANCE || abs(yZombie - yHero) > VISION_DISTANCE)
			struct_zombies[i].follow = false;


		if (struct_zombies[i].follow == false)
		{
			if (abs(xZombie - xHero) < VISION_DISTANCE && abs(yZombie - yHero) < VISION_DISTANCE)
				struct_zombies[i].follow = true;
		}
	}
}

void ZombieUpdatePosition(int z)
{
	float xZombie = struct_zombies[z].x;
	float yZombie = struct_zombies[z].y;

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
	struct_zombies[z].x = xZombie;
	struct_zombies[z].y = yZombie;
	//struct_zombies[z].sprite.setPosition(x, y);

	//poka chto
	struct_zombies[z].sprite.setPosition(struct_zombies[z].x, struct_zombies[z].y);
}

//сделать чтобы лишь в одной функции перебирались в цикле зомби, и применить туда остальные функции (внутри цикла)
void ZombieUpdateAttack(int n, float xZombie, float yZombie, float xHero, float yHero, float & time)
{
	//add correct xyHero xyZombie for detecting meleeAttack
	if ((xZombie < xHero + 32) && (xZombie) >(xHero))
	{
		//attack
		if (struct_zombies[n].attack_time < time - 1.5)
		{
			Health_Hero -= ZOMBIE_DAMAGE;
			struct_zombies[n].attack_time = time;
		}
	}
}

void ZombieUpdate(Sprite & hero, Sprite & sprite_zombie, float & time)
{
	float xHero = hero.getPosition().x;
	float yHero = hero.getPosition().y;

	ZombieCheckFollow(hero, xHero, yHero);

	for (int i = 0; i < len_struct_zombies - 2; i++)
	{
		Direction dir = struct_zombies[i].dir;



		int xZombie = struct_zombies[i].x;
		int yZombie = struct_zombies[i].y;

		//geometry
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
			ZombieUpdateAttack(i, xZombie, yZombie, xHero, yHero, time);
		//cout << dir << "  dir" << endl;

		//UpdateZombiePosition(i);  make it for all zombies, not jsut for following ones
		struct_zombies[i].dir = dir;

		//remove soon
		if (struct_zombies[i].follow == false)
			struct_zombies[i].dir = NONE;
	}
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
void DrawHero(Sprite & hero, RenderWindow & window)
{
	window.draw(hero);
}

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

			for (int z = 0; struct_zombies[z].health >0; z++)
				if (sprite_shot.getGlobalBounds().contains(struct_zombies[z].sprite.getPosition()))
					struct_zombies[z].health -=  DMG_ITEM[struct_inventory[IndexItem].item];
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
	switch (struct_inventory[IndexItem].item)
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
		str = "soda";
		break;
	case MIXTURE:
		str = "mixture";
		break;
	}

	//currentItem << (struct_inventory[IndexItem].item);
	std::ostringstream toStringCurrent;
	toStringCurrent << struct_inventory[IndexItem].current;

	std::ostringstream toStringQuantity;
	toStringQuantity << struct_inventory[IndexItem].quantity;
	text.setString(toStringCurrent.str() + "/" + toStringQuantity.str() + " " + str);

	window.draw(text);//рисую этот текст
	window.draw(text);//рисую этот текст
}

//избавиться от многих аргументов

void DrawBar(RenderWindow & window, View & view, Text & text, Sprite & sprite_bar, Sprite & sprite_health, Sprite & sprite_items)
{
	float x = view.getCenter().x - 600;
	float y = view.getCenter().y - 450;

	//center 319 238

	sprite_bar.setTextureRect(IntRect(0, 0, 170, 35));
	sprite_bar.setPosition(x, y);


	//delSoon

	if (Health_Hero < 0) Health_Hero = 0;
	sprite_health.setTextureRect(IntRect(1, 0, 146 * (float(Health_Hero) / 100), 29));
	
	sprite_health.setPosition(x + 10, y + 1);

	//Item item = struct_inventory[IndexItem].item;
	struct_inventory[IndexItem].sprite.setPosition(x + 5, y + 40);

	window.draw(struct_inventory[IndexItem].sprite);

	window.draw(sprite_bar);
	window.draw(sprite_health);
	DrawText(window, text, x, y);
}

void CheckLoot(Sprite & hero, Texture & texture_items)
{
	float x = hero.getPosition().x;
	float y = hero.getPosition().y;

	bool flag = false;

	for (int i = 0; struct_loot[i].pos.x > 0; i++)
	{
		if (struct_loot[i].draw == true)
			//если спрайт героя на середине item'a


				if (hero.getGlobalBounds().contains(struct_loot[i].pos.x + struct_loot[i].sprite.getGlobalBounds().width / 2, struct_loot[i].pos.y + struct_loot[i].sprite.getGlobalBounds().height / 2))
				{
					if (struct_loot[i].item != AMMO)
					{

						cout << " LOOT!!! " << endl;
						//del ячейку
						//доработать подбор патронов, чтобы плюсовались, или же если уже такая пушка есть то прибавлять ее патроны
						for (int n = 0; n < InventorySlots; n++)
						{
							if (struct_inventory[n].item == struct_loot[i].item)
							{

								//struct_inventory[n].item = struct_loot[i].item;
								struct_inventory[n].quantity += struct_loot[i].quantity;
								//struct_inventory[n].current = 0;
								struct_inventory[n].sprite.setTexture(texture_items);
								struct_inventory[n].sprite.setTextureRect(sf::IntRect(struct_loot[i].item * 32, 0, 32, 32));

								struct_loot[i].draw = false;
								flag = flag || true;
								break;
							}
						}

						if (flag == false)
						{
							flag = true;
							struct_inventory[InventorySlots].item = struct_loot[i].item;
							struct_inventory[InventorySlots].quantity = struct_loot[i].quantity;
							struct_inventory[InventorySlots].current = 0;
							struct_inventory[InventorySlots].sprite.setTexture(texture_items);
							struct_inventory[InventorySlots].sprite.setTextureRect(sf::IntRect(struct_loot[i].item * 32, 0, 32, 32));

							struct_loot[i].draw = false;
							InventorySlots += 1;
						}
					}
					else
					{
						struct_loot[i].draw = false;
						int packed = 0;
						while (packed < AMMO_PACKS)
							//delSoon
							for (int m = 0; m < InventorySlots; m++)
							{
								struct_inventory[m].quantity += MAX_AMMO[struct_inventory[m].item];
								packed += 1;
							}
					}

				}
			}
		
	}

void GenerateLoot(int  num, Item  item, Texture & texture_items)
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
			for (int i = 0; struct_loot[i].pos.x > 0; i++)
				if (abs(struct_loot[i].pos.x - x) < 100 && abs(struct_loot[i].pos.y - y) < 100)
				{
					need_new_block = true;
					break;
				}
			if (need_new_block == false)
			{
				struct_loot[len_struct_loot].item = item;

				int quantity;
				switch (struct_loot[len_struct_loot].item)
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
				struct_loot[len_struct_loot].quantity = quantity;
				//lishnee
				struct_loot[len_struct_loot].pos.x = x;
				struct_loot[len_struct_loot].pos.y = y;

				struct_loot[len_struct_loot].sprite.setTexture(texture_items);
				struct_loot[len_struct_loot].sprite.setTextureRect(sf::IntRect(item * 32, 0, 32, 32));
				struct_loot[len_struct_loot].sprite.setPosition(x, y);
				struct_loot[len_struct_loot].draw = true;

				len_struct_loot += 1;
				num -= 1;
			}
		}

	} while (num > 0);
}

void DrawLoot(RenderWindow & window)
{
	for (int i = 0; struct_loot[i].pos.x > 0; i++)
	{
		if (struct_loot[i].draw == true)
			window.draw(struct_loot[i].sprite);
	}
}

void UpdateInventory()
{
	//update items 
	for (int i = 0; i < InventorySlots; i++)
	{
		if (struct_inventory[i].current == 0)
		{
			if (struct_inventory[i].quantity >= MAX_AMMO[struct_inventory[i].item])
			{
				struct_inventory[i].quantity -= MAX_AMMO[struct_inventory[i].item];
				struct_inventory[i].current = MAX_AMMO[struct_inventory[i].item];
			}
			else
			{
				struct_inventory[i].current = struct_inventory[i].quantity;
				struct_inventory[i].quantity = 0;
			}
			if (struct_inventory[i].quantity <= 0)
				struct_inventory[i].quantity = 0;
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
	sf::Sprite hero;
	hero.setTexture(texture_hero);
	hero.setTextureRect(IntRect(4, 4, 32, 32));

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

	/*
	Texture texture_pistol;
	texture_pistol.loadFromFile("images/pistol.png");
	Sprite pistol;
	pistol.setTexture(texture_pistol);

	Texture texture_drink;
	texture_drink.loadFromFile("images/drink.png");
	Sprite drink;
	pistol.setTexture(texture_drink);
	*/

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

	Direction dir = NONE;
	Direction dir_last;

	View view;
	Clock clock;

	RenderWindow window(sf::VideoMode(1280, 1024), "dota 3");

	//camera
	view.reset(sf::FloatRect(0, 0, 1280, 1024));


	hero.setPosition(6 * STEP, 9 * STEP);  //start position

	ZombieSpawn(500, 200);


	struct_inventory[0].item = PISTOL;
	struct_inventory[0].quantity = 7;
	struct_inventory[0].sprite = sprite_items;
	struct_inventory[0].sprite.setTextureRect(IntRect(32, 0, 32, 32));

	GenerateLoot(5, DRINK, texture_items);
	GenerateLoot(3, PISTOL, texture_items);
	GenerateLoot(2, AMMO, texture_items);
	bool switch_status = false; //для ProcessEvents (корректное переключение оружия)
								//bool fire_status = false; //для стрельбы 
	float shot_last_time = 0;
	while (window.isOpen())
	{
		window.clear();

		float time = clock.getElapsedTime().asSeconds();
		float timeMili = clock.getElapsedTime().asMilliseconds();
		ProcessEvents(window, dir, dir_last, hero, time, switch_status, shot_last_time);    //main
		Update(hero, dir);

		HeroCollision(hero, dir);
		UpdateView(window, hero, view);

		UpdateShots(time);
		ZombieUpdate(hero, sprite_zombie, time);

		CheckLoot(hero, texture_items);
		UpdateInventory();

		//draw
		DrawMap(mapSprite, window);
		DrawLoot(window);
		DrawHero(hero, window);
		DrawShots(sprite_shot, window);
		DrawZombies(sprite_zombie, window);
		DrawBar(window, view, text, sprite_bar, sprite_health, sprite_items);

		cout << struct_zombies[0].health << endl;

		//cout << IndexItem << InventorySlots << "  " << struct_inventory[IndexItem].current << endl;
		//cout << Health_Hero << endl;
		//cout << struct_zombies[0].x << "  " << struct_zombies[0].y << endl;
		//cout << "HERO  " << hero.getPosition().x  << "  " << hero.getPosition().y << endl;
		window.display();
	}
}