#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <cstdlib>

#include "declarations.h"
#include "game.h"
#include <vector>
#include "hero.h"



using namespace sf;
using namespace std;

/*
NEWBUGS:
{

}
FOUNDBUGS:
{
zombie doesnt change direction if it collides sometimes
doesnt display shots sometimes
}

NEXT: доработать движение зомби при коллизии  !STOP! + автономное.
add zombie behavour when it's attacking hero

calculate zombie's next direction if it collides
add files and structures

потом почистить код св€заный со спрайтами и их Vec2f pos
//question.

*/


/*
void UpdateDoors(Game & game, Hero & hero)
{
	//TODONOW: CORRECT NUMBERS
	for (vector<Door>::iterator door = game.doorList.begin(); door != game.doorList.end(); door++)
	{
		if (door->isOpened == false)
		{
			if (abs(door->pos.x - hero.pos.x) < 50 && (hero.pos.y - door->pos.y) < 70)
			{
				door->isOpened = true;
				game.inventoryList[hero.slotNo].current -= 1;
			}
		}
	}
}*/

void CheckLoot(Hero & hero, Texture & texture_items, Game & game)
{
	//TODO: refact.
	float x = hero.sprite.getPosition().x;
	float y = hero.sprite.getPosition().y;

	bool flag = false;

	for (vector<Loot>::iterator i = game.lootList.begin(); i != game.lootList.end();)
	{
		if (i->isDrawn == true)  //if loot.item.center contains heroSprite  -> add new item in inventory
		{
			Vector2f itemCenter;
			itemCenter.x = i->pos.x + i->sprite.getGlobalBounds().width / 2;
			itemCenter.y = i->pos.y + i->sprite.getGlobalBounds().height / 2;
			if (hero.sprite.getGlobalBounds().contains(itemCenter))
			{
				if (i->name != AMMO)
				{
					cout << " LOOT!!! " << endl;
					//del €чейку
					for (std::vector<Inventory>::iterator j = game.inventoryList.begin(); j != game.inventoryList.end(); ++j)
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
						game.inventoryList.push_back(inventory);

						//che za 
						i->isDrawn = false;
						hero.nSlots += 1;
					}
				}
				else
				{
					i->isDrawn = false;
					int nWeaponAmmoAdded = 0;
					while (nWeaponAmmoAdded < AMMO_PACKS)
					{
						//delSoon>??
						for (std::vector<Inventory>::iterator iter = game.inventoryList.begin(); iter != game.inventoryList.end(); ++iter)
						{
							if (iter->name != MIXTURE && iter->name != KEY && iter->name != DRINK)
							{
								iter->quantity += MAX_AMMO[iter->name];
								nWeaponAmmoAdded += 1;
							}
						}
						break;
					}
				}
			}
			i++;
		}
		else
		{
			i = game.lootList.erase(i);
		}
	}
}

View UpdateView(Game & game, Hero & hero, View & view)  //UpdateCameraPosition
{
	Vector2f temp = hero.sprite.getPosition();

	if (temp.x < 640) temp.x = 640;
	else if (temp.x > 3008) temp.x = 3008;

	if (temp.y < 512) temp.y = 512;
	else if (temp.y > 928) temp.y = 928;
	
	view.setCenter(temp);
	game.window->setView(view);
	return view;
}


//zombies


//draw
/*
void DrawMap(Game & game, Sprite & mapSprite)
{
	for (int i = HEIGHT_MAP - 1; i >= 0; i--)
	{
		for (int j = WIDTH_MAP - 1; j >= 0; j--)
		{
			if (TILEMAP[i][j] == ' ')
			{
				mapSprite.setTextureRect(IntRect(48, 0, 48, 48));    //block (horizontal)
				mapSprite.setPosition(j * STEP, i * STEP);
			}
			else if (TILEMAP[i][j] == 'b')
			{
				mapSprite.setTextureRect(IntRect(0, 0, 48, 48));    //block (horizontal)
				mapSprite.setPosition(j * STEP, i * STEP);
			}
			else if (TILEMAP[i][j] == 'v')                           //block (vertical)
			{
				mapSprite.setTextureRect(IntRect(48, 0, 48, 48));   //grass
				mapSprite.setPosition(j * STEP, i * STEP);
				game.window->draw(mapSprite);

				mapSprite.setTextureRect(IntRect(96, 0, 16, 48));
				mapSprite.setPosition(j * STEP, i * STEP);
			}
			else if (TILEMAP[i][j] == 'L')                           //block (horisontal left)
			{
				mapSprite.setTextureRect(IntRect(192, 96, 48, 48));
				mapSprite.setPosition(j * STEP, i * STEP);
			}
			else if (TILEMAP[i][j] == 'R')                           //block(horisontal right)
			{
				mapSprite.setTextureRect(IntRect(192, 48, 64, 48));
				mapSprite.setPosition(j * STEP, i * STEP);
			}
			else if (TILEMAP[i][j] == 'r')                           //block(horisontal right)
			{
				mapSprite.setTextureRect(IntRect(192, 0, 64, 48));
				mapSprite.setPosition(j * STEP, i * STEP);
			}
			game.window->draw(mapSprite);
		}
	}
}
*/


void CheckZombieExplosion(vector<Explosion> & explosionList,vector<Zombie> & zombieList)
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


//TODO: change to structs
void DrawBar(RenderWindow & window,vector<Inventory> & inventoryList, Hero & hero,View & view, Text & text, Sprite & sprite_bar, Sprite & sprite_health, Sprite & sprite_items)
{
	Vector2f posView = GetInterfacePosition(view);

	sprite_bar.setTextureRect(IntRect(0, 0, 170, 35));
	sprite_bar.setPosition(posView);

	//TODO NOT NOW: hero.death
	if (hero.health < 0)
	{
		hero.health = 0;
	}
	sprite_health.setTextureRect(IntRect(1, 0, 146 * (float(hero.health) / 100), 29));

	//changeSoon(забыл чЄ хотел)
	sprite_health.setPosition(posView.x + 10, posView.y + 1);

	inventoryList[hero.slotNo].sprite.setPosition(posView.x + 5, posView.y + 40);

	window.draw(inventoryList[hero.slotNo].sprite);
	window.draw(sprite_bar);
	window.draw(sprite_health);
}

void GenerateLoot(vector<Loot> & lootList,int  num, NameItem  item, Texture & texture_items)
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

void CheckSpawnZombiesAndLoot(Game & game, Texture & texture_items, Sprite & sprite_zombie)
{
	if (game.lootList.size() < 5)
	{
		int itemNo = rand() % 4;
		NameItem item;
		switch (itemNo)
		{
		case 0:
			item = DRINK;
			break;
		case 1:
			item = PISTOL;
			break;
		case 2:
			item = RIFLE;
			break;
		case 3:
			item = MIXTURE;
			break;
		}
		GenerateLoot(game.lootList, 1, item, texture_items);
	}
	if (game.zombieList.size() < 10)
	{
		SpawnZombieRandomly(game.zombieList, 1, game.time,sprite_zombie);
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
	//Npc
	Texture texture_npc;
	texture_npc.loadFromFile("images/npcs.png");
	sf::Sprite sprite_npc;
	sprite_npc.setTexture(texture_npc);
	//Grenade
	Texture texture_grenade;
	texture_grenade.loadFromFile("images/grenade.png");
	sf::Sprite sprite_grenade;
	sprite_grenade.setTexture(texture_grenade);
	//House
	Texture texture_house;
	texture_house.loadFromFile("images/house1.png");
	sf::Sprite sprite_house;
	sprite_house.setTexture(texture_house);
	//Door
	Texture texture_door;
	texture_door.loadFromFile("images/door1.png");
	sf::Sprite sprite_door;
	sprite_door.setTexture(texture_door);
	//Explosion
	Texture texture_explosion;
	texture_explosion.loadFromFile("images/explosion.png");
	sf::Sprite sprite_explosion;
	sprite_explosion.setTexture(texture_explosion);



	//initializeGame
	Game * game = new Game();
	InitializeGame(*game);

	Clock clock;
	Clock gameSpeedClock;

	game->view.reset(sf::FloatRect(0, 0, 1280, 1024)); //camera

	game->inventoryList[0].texture = texture_items;
	game->inventoryList[0].sprite.setTexture(texture_items);
	game->inventoryList[0].sprite.setTextureRect(IntRect(32, 0, 32, 32));
	game->explosionTexture = texture_explosion;
	game->explosionSprite = sprite_explosion;

	//text
	game->font.loadFromFile("Font/Arialbd.ttf");
	game->text.setString("");
	game->text.setFont(game->font);
	game->text.setCharacterSize(20);
	game->text.setColor(Color::White);


	/*
	House house;
	house.pos = { 47 * STEP, 5 * STEP };
	house.texture = texture_house;
	house.sprite = sprite_house;
	house.sprite.setPosition(house.pos);
	game->houseList.push_back(house);
	*/

	Door door;
	door.pos = { 47 * STEP + 119, 5 * STEP + 107 };
	door.texture = texture_door;
	door.sprite = sprite_door;
	door.sprite.setPosition(door.pos);
	door.isOpened = false;
	game->doorList.push_back(door);

	//srand(time(NULL));
	/*
	hero->texture.loadFromFile("images/hero.png");
	hero->sprite.setTexture(hero->texture);
	hero->sprite.setTextureRect(IntRect(4, 4, 32, 32));
	hero->sprite.setPosition(6 * 32, 9 * 32);  //start position
	*/

	//initializeZombie
	ZombieSpawn(game->zombieList,game->time,100, 100, sprite_zombie);

	//bool is_game_over = false;

	//initializeLoot
	GenerateLoot(game->lootList,5, DRINK, texture_items);
	GenerateLoot(game->lootList,3, PISTOL, texture_items);
	GenerateLoot(game->lootList,2, AMMO, texture_items);
	GenerateLoot(game->lootList,1, RIFLE, texture_items);
	GenerateLoot(game->lootList,1, KEY, texture_items);
	GenerateLoot(game->lootList,1, MIXTURE, texture_items);
	GenerateLoot(game->lootList, 2, GRENADE, texture_items);

	InitializeNpc(game->npcList, texture_npc);

	Level lvl;//создали экземпл€р класса уровень
	lvl.LoadFromFile("level0.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.


								//bool fire_status = false; //дл€ стрельбы 

	float gameSpeedTime = 0;

	while (game->window->isOpen())
	{
		gameSpeedTime = gameSpeedClock.getElapsedTime().asMilliseconds();
		if (gameSpeedTime > SCREEN_UPDATE_TIME)
		{
			gameSpeedClock.restart();
			game->time = clock.getElapsedTime().asSeconds();

			//cout << game.herosavedNeighbors << endl;

			game->window->clear();

			switch (game->state)
			{
			case START_GAME:
				BeginEvent(*game, game->view);
				break;
			case END_GAME:
				EndGameEvent(*game, game->view);
				break;
			case LEVEL_FINISH:
				LevelFinishEvent(*game, game->view);
				break;
			case PLAY:

				//TODO: spawn zombie at definite time (and change SpawnZombie func (spawn only near hero))


				CheckSpawnZombiesAndLoot(*game, texture_items, sprite_zombie);
				ProcessEvents(*game, sprite_shot, sprite_grenade);
				UpdateHero(*game);
				UpdateInventory(*game->hero, game->inventoryList, game->time);
				CheckEventNpc(game->npcList, *game->hero);
				UpdateView(*game, *game->hero, game->view);
				UpdateShots(*game, *game->hero, sprite_explosion);


				UpdateZombies(game->zombieList,*game->hero,game->npcList,game->time);

				CheckZombieExplosion(game->explosionList, game->zombieList);

				CheckLoot(*game->hero, texture_items, *game);
				UpdateInventory(*game->hero, game->inventoryList,game->time);

				ComputeNpcFrame(game->npcList);
				CheckGameOver(game->state, *game->hero);


				//Drawing
				//DrawMap(*game, mapSprite);
				lvl.Draw(*game->window);//рисуем новую карту

				DrawInventoryText(*game->window, game->inventoryList, *game->hero, game->view,game->text);

				Render(*game, *game->hero);
				//DrawNpc(*game);
				DrawBar(*game->window, game->inventoryList, *game->hero, game->view, game->text, sprite_bar, sprite_health, sprite_items);

				break;
			}
			game->window->display();
		}
	}
}