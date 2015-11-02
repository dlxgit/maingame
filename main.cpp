#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "map.h"

//delSoon
//убийство зомби (удаление 1 элемента в структуре)
//избавиться от обхода по структуре через len_struct...

using namespace sf;
using namespace std;

typedef enum
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UPRIGHT,
	UPLEFT,
	DOWNRIGHT,
	DOWNLEFT,
	FOLLOW
} Direction;

typedef enum {
	PISTOL,
	RIFLE,
	GRENADE,
}Item;

//need add key

const int STEPHERO = 6;
const int STEPZOMBIE = 4;
const int STEPSHOT = 12;

const int ZOMBIE_DAMAGE = 30;
const int ZOMBIE_MAX_HP = 100;

const int VISION_DISTANCE = 230;

const int len_struct_shots = 15;
const int len_struct_zombies = 3;

const float SHOT_MAX_TIME = 1;


int Health_Hero = 100;

int x, y;

Direction dir;
Direction dir_last;

int num = 1;

int N = 20, M = 15;

int w = STEP*N;
int h = STEP*M;
bool game_over = false;


//new
int IndexItem = 0;
int InventorySlots = 1;


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
	int x;
	int y;
	int HP;
	Direction dir;
	bool follow;
	float attack_time;
	Texture texture;
	Sprite sprite;
} struct_zombies[15];

struct
{
	float x;
	float y;
	Direction dir;
	float time;
} struct_shots[15];

struct
{
	Item item;
	int quantity;
	Vector2f pos;
}struct_loot[50];

struct
{
	Item item;
	int quantity;
}struct_inventory[15];


/*
void Spawn_zombie(RectangleShape & zombie)
{
int x;
int y;
bool position_is_used;
do
{
x = (rand() % 19 + 1) * STEP;
y = (rand() % 14 + 1) * STEP;

position_is_used = false;
for (int i = 0; i < num; i++)
{
if (s[i].x == x && s[i].y == y)
{
position_is_used = true;
break;
}
}
} while (position_is_used == true);

zombie.setPosition(x, y);
}
*/
//how to hr texture

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

	for (int i = 0; i < len_struct_shots; i++)
		if (struct_shots[i].x == 0)   //если нашли пустую ячейку
		{
			struct_shots[i].x = hero.getPosition().x;
			struct_shots[i].y = hero.getPosition().y;

			struct_shots[i].time = time;

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


					 /*
					 //если не заработает case
					 if (dir == 1) struct_shots[i].dir = 1;
					 else if (dir == 2 ) struct_shots[i].dir = 1;
					 else if(dir == 1) struct_shots[i].dir = 1;
					 else

					 break;
					 */
		}
}

void ProcessEvents(RenderWindow & window, Direction & dir, Sprite & hero, float & time)
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
		if (Keyboard::isKeyPressed(Keyboard::X))
		{
			if (struct_inventory[IndexItem].item)
				IndexItem += 1;
			else IndexItem = 0;
		}
		if (Keyboard::isKeyPressed(Keyboard::Z))
		{
			if (struct_inventory[IndexItem].item > 0)
				IndexItem -= 1;
			else IndexItem = InventorySlots;
		}


		if (dir != NONE) dir_last = dir;

		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			AddNewShot(hero, dir_last, time);

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

void InteractWithMap(Sprite & hero, Direction & dir) //ф-ция взаимодействия с картой      мб объединить с функцией update? чтобы сразу здесь вычислять перемещение героя (проблема с коллизией при движении по диагонали
{
	float x = hero.getPosition().x;
	float y = hero.getPosition().y;

	//herosize
	float sizeX = hero.getGlobalBounds().width;
	float sizeY = hero.getGlobalBounds().height;


	/*
	if (TILEMAP[int(x) / STEP][int(y) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y) / STEP] == 'b')

	//right up and down
	if (TILEMAP[int(x + sizeX) / STEP][int(y) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y + sizeY) / STEP] == 'b')

	//down left and right
	if (TILEMAP[int(x) / STEP][int(y + sizeY) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y + sizeY) / STEP] == 'b')

	//left up and down
	if (TILEMAP[int(x) / STEP][int(y) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y) / STEP] == 'b')

	//upright except downleft
	if (TILEMAP[int(x) / STEP][int(y) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y) / STEP] == 'b' || TILEMAP[int(x) / STEP][int(y) / STEP] == 'b')
	//upleft except downright
	if (TILEMAP[int(x) / STEP][int(y) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y + sizeY) / STEP] == 'b')
	//downright except upleft
	if (TILEMAP[int(x) / STEP][int(y + sizeY) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y + sizeY) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y) / STEP] == 'b')
	//downleft except upright
	if (TILEMAP[int(x) / STEP][int(y + sizeY) / STEP] == 'b' || TILEMAP[int(x + sizeX) / STEP][int(y + sizeY) / STEP] == 'b' || TILEMAP[int(x) / STEP][int(y) / STEP] == 'b')

	*/

	switch (dir)
	{
	case UP:
		//up left and right
		if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b' || TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b')
		{
			y = (int(y) / STEP) * STEP + STEP;
		}
		break;
	case UPRIGHT:
		//upright except downleft

		if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b') //upleft
		{
			y = (int(y) / STEP) * STEP + STEP;
		}
		if (TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b') //upright
		{
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
		}
		else if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b')
		{

		}
		break;
	case RIGHT:
		//right up and down
		if (TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b' || TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b')
		{
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
		}
		break;
	case DOWNRIGHT:
		//downright except upleft
		if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b')  //downleft
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
		}
		if (TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b')  //upright
		{
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
		}
		else if (TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b')  //downright
		{

		}

		break;
	case DOWN:
		//down left and right
		if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b' || TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b')
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
		}
		break;
	case DOWNLEFT:
		//downleft except upright
		if (TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b') //downright
		{
			y = (int(y + sizeY) / STEP) * STEP - sizeY;
		}
		if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b')  //upleft
		{
			x = (int(x) / STEP) * STEP + STEP;
		}
		else if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b')  //downleft
		{

		}
		break;
	case LEFT:
		//left up and down
		if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b' || TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b')
		{
			x = (int(x) / STEP) * STEP + STEP;
		}
		break;
	case UPLEFT:
		//upleft except downright
		if (TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b') //upright
		{
			y = (int(y) / STEP) * STEP + STEP;
		}
		if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b')  //downleft
		{
			x = (int(x) / STEP) * STEP + STEP;
		}
		else if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b')   //upleft
		{

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


	for (int index_shot = 0; index_shot < len_struct_shots; index_shot++)
	{

		if (struct_shots[index_shot].dir != NONE)
		{
			switch (struct_shots[index_shot].dir)  //обновление координат пуль на карте
			{
			case UP:
				struct_shots[index_shot].y -= STEPSHOT;
				break;
			case RIGHT:
				struct_shots[index_shot].x += STEPSHOT;
				break;
			case DOWN:
				struct_shots[index_shot].y += STEPSHOT;
				break;
			case LEFT:
				struct_shots[index_shot].x -= STEPSHOT;
				break;

			}

			if (time - struct_shots[index_shot].time > SHOT_MAX_TIME)
			{
				cout << "CLEAR" << endl;
				struct_shots[index_shot].x = 0;
				struct_shots[index_shot].y = 0;
				struct_shots[index_shot].dir = NONE;
				struct_shots[index_shot].time = 0;

			}
		}
	}
}


//zombies

void ZombieSpawn(int posX, int posY)
{
	int i = 0;
	while (struct_zombies[i].HP > 0)
		i += 1;

	struct_zombies[i].x = posX;
	struct_zombies[i].y = posY;
	struct_zombies[i].HP = ZOMBIE_MAX_HP;
	struct_zombies[i].texture.loadFromFile("images/zombie.png");
	struct_zombies[i].sprite.setTexture(struct_zombies[i].texture);
}

void ZombieCollision(Sprite & sprite_zombie) //ф-ция взаимодействия с картой      мб объединить с функцией update? чтобы сразу здесь вычислять перемещение героя (проблема с коллизией при движении по диагонали
{
	for (int i = 0; i < len_struct_zombies; i++)
	{

		//исправить чтобы для всех а не только преследующих
		if (struct_zombies[i].follow == true)
		{

			float x = struct_zombies[i].x;
			float y = struct_zombies[i].x;
			Direction dir = struct_zombies[i].dir;

			//herosize
			float sizeX = sprite_zombie.getGlobalBounds().width;
			float sizeY = sprite_zombie.getGlobalBounds().height;


			cout << struct_zombies[i].follow << endl;

			switch (dir)
			{
			case UP:
				//up left and right
				if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b' || TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b')
				{
					y = (int(y) / STEP) * STEP + STEP;
				}
				cout << " UP " << endl;
				break;
			case UPRIGHT:
				//upright except downleft

				if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b') //upleft
				{
					y = (int(y) / STEP) * STEP + STEP;
					dir = RIGHT;
				}
				if (TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b') //upright
				{
					x = (int(x + sizeX) / STEP) * STEP - sizeX;
					dir = UP;
				}
				else if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b')
				{

				}

				cout << " UPRIGHT " << endl;
				break;
			case RIGHT:
				//right up and down
				if (TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b' || TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b')
				{
					x = (int(x + sizeX) / STEP) * STEP - sizeX;
				}
				cout << "RIGHT " << endl;
				break;
			case DOWNRIGHT:
				//downright except upleft
				if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b')  //downleft
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
					dir = RIGHT;
				}
				if (TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b')  //upright
				{
					x = (int(x + sizeX) / STEP) * STEP - sizeX;
					dir = DOWN;
				}
				else if (TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b')  //downright
				{

				}
				cout << " DOWNRIGHT " << endl;
				break;
			case DOWN:
				//down left and right
				if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b' || TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b')
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
				}
				cout << "DOWN " << endl;
				break;
			case DOWNLEFT:
				//downleft except upright
				if (TILEMAP[int(y + sizeY) / STEP][int(x + sizeX) / STEP] == 'b') //downright
				{
					y = (int(y + sizeY) / STEP) * STEP - sizeY;
					dir = LEFT;
				}
				if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b')  //upleft
				{
					x = (int(x) / STEP) * STEP + STEP;
					dir = DOWN;
				}
				else if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b')  //downleft
				{

				}
				cout << " DOWNLEFT" << endl;
				break;
			case LEFT:
				//left up and down
				if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b' || TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b')
				{
					x = (int(x) / STEP) * STEP + STEP;
				}
				cout << "LEFT" << endl;
				break;
			case UPLEFT:
				//upleft except downright
				if (TILEMAP[int(y) / STEP][int(x + sizeX) / STEP] == 'b') //upright
				{
					y = (int(y) / STEP) * STEP + STEP;
					dir = LEFT;
				}
				if (TILEMAP[int(y + sizeY) / STEP][int(x) / STEP] == 'b')  //downleft
				{
					x = (int(x) / STEP) * STEP + STEP;
					dir = UP;
				}
				else if (TILEMAP[int(y) / STEP][int(x) / STEP] == 'b')   //upleft
				{

				}
				cout << "UPLEFT " << endl;
				break;
			case NONE:
				cout << "NONE " << endl;
				break;
			}
			
			struct_zombies[i].x = x;
			struct_zombies[i].y = y;
			struct_zombies[i].dir = dir;
		}
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
	float x = struct_zombies[z].x;
	float y = struct_zombies[z].y;

	switch (struct_zombies[z].dir)
	{
	case UP:
		y -= STEPZOMBIE;
		break;
	case DOWN:
		y += STEPZOMBIE;
		break;
	case LEFT:
		x -= STEPZOMBIE;
		break;
	case RIGHT:
		x += STEPZOMBIE;
		break;
	case UPLEFT:
		x -= STEPZOMBIE * 0.66;
		y -= STEPZOMBIE * 0.66;
		break;
	case UPRIGHT:
		x += STEPZOMBIE * 0.66;
		y -= STEPZOMBIE * 0.66;
		break;
	case DOWNLEFT:
		x -= STEPZOMBIE * 0.66;
		y += STEPZOMBIE * 0.66;
		break;
	case DOWNRIGHT:
		x += STEPZOMBIE * 0.66;
		y += STEPZOMBIE * 0.66;
		break;
	}

	//struct_zombies[z].sprite.setTextureRect(IntRect(5, 5, 30, 30));
	struct_zombies[z].x = x;
	struct_zombies[z].y = y;
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

void ZombieUpdate(Sprite & hero, float & time)
{
	float xHero = hero.getPosition().x;
	float yHero = hero.getPosition().y;

	ZombieCheckFollow(hero, xHero, yHero);

	for (int i = 0; i < len_struct_zombies; i++)
	{
		Direction dir = struct_zombies[i].dir;

		if (struct_zombies[i].follow == true)
		{

			int xZombie = struct_zombies[i].x;
			int yZombie = struct_zombies[i].y;

			//geometry
			float dx = abs(xHero - xZombie);
			float dy = abs(yHero - yZombie);
			//float di = sqrt(pow(dx,2) + pow(dy,2));


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
			if (xHero != xZombie || yHero != yZombie)
				
				ZombieUpdatePosition(i);
			
			ZombieUpdateAttack(i, xZombie, yZombie, xHero, yHero, time);
		}
		//UpdateZombiePosition(i);  make it for all zombies, not jsut for following ones
		struct_zombies[i].dir = dir;
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

/*
void UpdateZombies(Sprite & sprite_zombie, Sprite & hero) //обновляет элементы структуры зомби
{
int x, y;
x = hero.getPosition().x;
y = hero.getPosition().y;

for (int i = 0; i < len_struct_zombies; i++)
{

if (abs(struct_zombies[i].x - x) > vision_distance || abs(struct_zombies[i].y - y) > vision_distance)
struct_zombies[i].dir = NONE;


if (struct_zombies[i].follow = false)
{
if (abs(struct_zombies[i].x - x) < vision_distance && abs(struct_zombies[i].y - y) < vision_distance)
struct_zombies[i].follow = true;
}

}

ZombieFollowHero(hero);
}
*/

//draw
void DrawHero(Sprite & hero, RenderWindow & window)
{
	hero.setTextureRect(IntRect(5, 5, 30, 30));
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
			sprite_shot.setPosition(struct_shots[i].x, struct_shots[i].y);
			window.draw(sprite_shot);

			for (int z = 0; struct_zombies[z].HP >0; z++)
				if (sprite_shot.getGlobalBounds().contains(struct_zombies[z].sprite.getPosition()))
					struct_zombies[z].HP = 0;
			//last.   исправить везде считывание элементов структур (не по len_struct, и не по HP.) 
			//хп падает, но элемент не удаляется, (просто не рисуется), и не происходит самого процесса убийства зомби.
		}
}

void DrawZombies(Sprite & sprite_zombie, RenderWindow & window)
{
	for (int i = 0; struct_zombies[i].HP > 0; i++)
		//if (struct_zombies[i].dir > 0)
	{
		//sprite_zombie.setPosition(struct_zombies[i].x, struct_zombies[i].y);
		window.draw(struct_zombies[i].sprite);
	}
}

void DrawBar(RenderWindow & window, View & view)
{

	float x = view.getCenter().x - 600;
	float y = view.getCenter().y - 450;

	//center 319 238

	//healthbar
	Texture texture_bar;
	texture_bar.loadFromFile("images/bar.png");
	sf::Sprite sprite_bar;
	sprite_bar.setTexture(texture_bar);
	sprite_bar.setTextureRect(IntRect(0, 0, 300, 100));
	sprite_bar.setPosition(x, y);


	Texture texture_health;
	texture_health.loadFromFile("images/health.png");
	sf::Sprite sprite_health;
	sprite_health.setTexture(texture_health);

	//delSoon
	if (Health_Hero > 0)
		sprite_health.setTextureRect(IntRect(1, 0, 146 * (float(Health_Hero) / 100), 29));
	sprite_health.setPosition(x + 91, y + 4);

	window.draw(sprite_bar);
	window.draw(sprite_health);
}

void CheckLoot(Sprite & hero)
{
	float x = hero.getPosition().x;
	float y = hero.getPosition().y;

	for (int i = 0; struct_loot[i].item > 0; i++)
	{
		//changeSoon прикосновение к спрайту, а не к его левой верхней позиции чтобы было
		if (hero.getGlobalBounds().contains(struct_loot[i].pos))
		{
			//del ячейку
			//доработать подбор патронов, чтобы плюсовались, или же если уже такая пушка есть то прибавлять ее патроны
			struct_inventory[InventorySlots].item = struct_loot[i].item;
			struct_inventory[InventorySlots].quantity = struct_loot[i].quantity;
		}
	}
}

void main()
{

	//заполнить структуру выстрелов нулями (для определения пустой ячейки(пока что))

	/*
	for (int i = 0; i < len_struct_shots; i++)
	{
	struct_shots[i].x = 0;
	struct_shots[i].y = 0;
	struct_shots[i].dir = NONE;
	struct_shots[i].time = 0;
	}

	*/



	//ZombieSpawn(3* STEP, 4* STEP);
	//ZombieSpawn(150, 320);
	ZombieSpawn(500, 200);


	View view;

	Direction dir = NONE;


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



	sf::RenderWindow window(sf::VideoMode(1280, 1024), "dota 3");

	//camera
	view.reset(sf::FloatRect(0, 0, 1280, 1024));


	hero.setPosition(12 * STEP, 3 * STEP);  //start position
	Clock clock;


	while (window.isOpen() && game_over == false)
	{

		window.clear();

		float time = clock.getElapsedTime().asSeconds();

		ProcessEvents(window, dir, hero, time);    //main
		Update(hero, dir);
		UpdateView(window, hero, view);

		InteractWithMap(hero, dir);

		UpdateShots(time);
		//ZombieCollision(sprite_zombie);
		ZombieUpdate(hero, time);
		//zombie_follow_hero(zombie, hero);

		ZombieCollision(sprite_zombie);
		//getplayerpositionforWiew(hero, view);

		DrawMap(mapSprite, window);    //draw
		DrawHero(hero, window);
		DrawShots(sprite_shot, window);
		DrawZombies(sprite_zombie, window);
		DrawBar(window, view);

		cout << Health_Hero << endl;

		window.display();
	}

}