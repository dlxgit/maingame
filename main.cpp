#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cmath>
#include "game.h"
#include <vector>
#include "hero.h"
#include <cstdlib>

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


bool isPosAwayFromHouse(Game & game, Vector2f pos)
{
	bool isAway = true;
	for (vector<House>::iterator house = game.houseList.begin(); house != game.houseList.end(); house++)
	{
		Vector2f houseCenter;
		houseCenter.x = house->sprite.getGlobalBounds().width / 2;
		houseCenter.y = house->sprite.getGlobalBounds().height / 2;

		//TODO: correct numbers
		if (abs(houseCenter.x - pos.x) > 200 && abs(houseCenter.y - pos.y) > 200)
		{
			isAway = isAway * true;
		}
		else
		{
			return false;
		}
	}
	return isAway;
}

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
}

void DrawHero(Game & game, Hero & hero)
{
	game.window->draw(hero.sprite);
}

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

void AddNewShot(Hero & hero, Sprite & sprite_shot,Sprite & sprite_grenade, Game & game, ShotType  shotType) //adding new shot in list
{
	//if hero direction is UPRIGHT OR DOWNRIGHT -> he will watch(and shoot) to the left, same for RIGHT
	if (shotType == BULLET)
	{
		Shot shot;
		shot.pos = hero.sprite.getPosition();
		shot.distance = 0;
		shot.sprite = sprite_shot;
		shot.type = shotType;
		Texture texture;

		switch (hero.dirLast)
		{
		case UP:
			shot.dir = UP;
			shot.sprite.setTextureRect(IntRect(3, 11, 7, 13));
			break;
		case UPRIGHT: case RIGHT: case DOWNRIGHT:
			shot.dir = RIGHT;
			shot.sprite.setTextureRect(IntRect(0, 0, 13, 7));
			break;
		case DOWN:
			shot.dir = DOWN;
			shot.sprite.setTextureRect(IntRect(19, 11, 7, 13));
			break;
		case DOWNLEFT: case LEFT: case UPLEFT:
			shot.dir = LEFT;
			shot.sprite.setTextureRect(IntRect(16, 0, 13, 7));
			break;
		}
		game.shotList.push_back(shot);
	}
	else
	{
		Shot shot;
		shot.pos = hero.sprite.getPosition();
		shot.distance = 0;
		shot.sprite = sprite_grenade;
		shot.type = shotType;
		shot.startTime = game.time;
		Texture texture;
		//AddNewShot(hero, sprite_shot, game);
		shot.sprite.setPosition(hero.sprite.getPosition());
		shot.startTime = game.time;
		shot.currentFrame = 0;
		shot.isExploded = false;

		switch (hero.dirLast)
		{
		case UP:
			shot.dir = UP;
			break;
		case DOWN:
			shot.dir = DOWN;
			break;
		case UPLEFT: case LEFT: case DOWNLEFT:
			shot.dir = LEFT;
			break;
		case UPRIGHT: case RIGHT: case DOWNRIGHT:
			shot.dir = RIGHT;
			break;
		}
		game.shotList.push_back(shot);
	}
}

void UpdateInventory(Hero & hero, Game & game)
{
	//update items 
	if (game.inventoryList[hero.slotNo].current == 0 && game.time - hero.lastReloadTime > WEAPON_RELOAD_TIME)
	{
		hero.lastReloadTime = game.time;
		hero.isReloading = false;
		if (game.inventoryList[hero.slotNo].quantity >= MAX_AMMO[game.inventoryList[hero.slotNo].name])
		{
			game.inventoryList[hero.slotNo].quantity -= MAX_AMMO[game.inventoryList[hero.slotNo].name];
			game.inventoryList[hero.slotNo].current = MAX_AMMO[game.inventoryList[hero.slotNo].name];
		}
		else
		{
			game.inventoryList[hero.slotNo].current = game.inventoryList[hero.slotNo].quantity;
			game.inventoryList[hero.slotNo].quantity = 0;
		}
		if (game.inventoryList[hero.slotNo].quantity <= 0)
			game.inventoryList[hero.slotNo].quantity = 0;
	}
}

void ProcessEvents(Game & game, Hero & hero, bool & switch_status, float & shot_last_time, Sprite & sprite_shot, Sprite & sprite_grenade)
{
	Event event;
	while (game.window->pollEvent(event))
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

		//reset currentFrame of moving hero
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

		//item-using events
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
				if (game.inventoryList[hero.slotNo].current > 0)
				{
					if (game.inventoryList[hero.slotNo].name == PISTOL)
					{
						if (game.time > shot_last_time + 0.35)
						{
							AddNewShot(hero, sprite_shot,sprite_grenade,game,BULLET);
							shot_last_time = game.time;
							game.inventoryList[hero.slotNo].current -= 1;
						}
					}
					else if (game.inventoryList[hero.slotNo].name == RIFLE)
					{
						if (game.time > shot_last_time + 0.15)
						{
							AddNewShot(hero, sprite_shot, sprite_grenade,game,BULLET);
							shot_last_time = game.time;
							game.inventoryList[hero.slotNo].current -= 1;
						}
					}
					else if (game.inventoryList[hero.slotNo].name == DRINK)
					{
						if (game.time > shot_last_time + 0.35)
						{
							shot_last_time = game.time;
							hero.health += HP_PER_DRINK;
							if (hero.health > 100)
							{
								hero.health = 100;  //replace
							}
							game.inventoryList[hero.slotNo].current -= 1;
						}
					}
					else if (game.inventoryList[hero.slotNo].name == MIXTURE)
					{
						//if (game.time > shot_last_time + 0.35)
						if (game.inventoryList[hero.slotNo].current > 0)
						{
							shot_last_time = game.time;
							hero.health += HP_PER_DRINK;
							if (hero.health > 100)
							{
								hero.health = 100;  //replace
							}
							game.inventoryList[hero.slotNo].current -= 1;
							hero.state = TRANSFORMING;
							hero.dir = NONE;
							hero.dirLast = DOWN;
						}
					}
					else if (game.inventoryList[hero.slotNo].name == GRENADE)
					{
						if (game.time > shot_last_time + 0.15)
						{
							AddNewShot(hero, sprite_shot,sprite_grenade, game, USED_GRENADE);
							shot_last_time = game.time;
							game.inventoryList[hero.slotNo].current -= 1;
						}
					}
					else if (game.inventoryList[hero.slotNo].name == KEY)
					{
						UpdateDoors(game, hero);
					}
				}
			}
		}

		//Window close event
		if (Keyboard::isKeyPressed(Keyboard::Escape) || event.type == Event::Closed)
			game.window->close();
	}
}

void CheckHeroCollision(Hero & hero)
{
	float x = hero.sprite.getPosition().x;
	float y = hero.sprite.getPosition().y;

	//herosize
	float sizeX = hero.sprite.getGlobalBounds().width;
	float sizeY = hero.sprite.getGlobalBounds().height;


	//if (TILEMAP[int(y + sizeY - STEP) / STEP][int(x) / STEP] == 'b')
	
	bool q = (TILEMAP[int(y) / STEP][int(x) / STEP] != ' ');
	bool w = (TILEMAP[int(y) / STEP][int(x + sizeX - 1) / STEP] != ' ');
	bool e = (TILEMAP[int(y + sizeY - 1) / STEP][int(x + sizeX - 1) / STEP] != ' ');
	bool r = (TILEMAP[int(y + sizeY - 1) / STEP][int(x) / STEP] != ' ');
	

	/*else
	{
		//проверка 4 угловых точек спрайта (верхн€€ лева€, права€, нижн€€ лева€, права€) на вхождение в блок карты
		q = (TILEMAP[int(y + sizeY - STEP) / STEP][int(x) / STEP] == 'b');
		w = (TILEMAP[int(y + sizeY - STEP) / STEP][int(x + sizeX - 1) / STEP] == 'b');
		e = (TILEMAP[int(y + sizeY - 1) / STEP][int(x + sizeX - 1) / STEP] == 'b');
		r = (TILEMAP[int(y + sizeY - 1) / STEP][int(x) / STEP] == 'b');
	}
	*/

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
			x = x + 0.33* STEP_HERO;
		}
		else if (q && e)
		{
			y = (int(y) / STEP) * STEP + STEP;
			x = (int(x + sizeX) / STEP) * STEP - sizeX;
		}
		else if (q && !e) //upleft
		{
			y = (int(y) / STEP) * STEP + STEP;
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
			y = (int(y) / STEP) * STEP + STEP;
			x = x - 0.33* STEP_HERO;
		}
		else if ((w && r) || (q && !(w || r)))   //upleft
		{
			y = (int(y) / STEP) * STEP + STEP;
			x = (int(x) / STEP) * STEP + STEP;
		}
		else if (w && !r) //upright
		{
			y = (int(y) / STEP) * STEP + STEP;
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

void UpdateHeroSprite(Hero & hero, Game & game)
{
	if (hero.state == TRANSFORMING)
	{
		hero.sprite.setTextureRect(IntRect(11 + 37 * int(hero.currentFrame), 484, 32, 45));
		hero.currentFrame += 0.05;

		if (hero.currentFrame > 7)
		{
			hero.state = BEAST;
			hero.beastTimer = game.time;
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
				cout << hero.currentFrame << " FRAME" << endl;
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

void UpdateHero(Game & game, Hero & hero) //position + collision + sprite
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
		if (game.time - hero.beastTimer > BEAST_MAX_TIME)
		{
			hero.state = NORMAL;
			hero.currentFrame = 0;
			hero.beastTimer = 0;
		}
	}

	if ((game.inventoryList[hero.slotNo].name == PISTOL || game.inventoryList[hero.slotNo].name == RIFLE) && game.inventoryList[hero.slotNo].current == 0 && game.inventoryList[hero.slotNo].quantity > 0)
	{
		if (hero.isReloading == false)
		{
			hero.lastReloadTime = game.time;
			hero.isReloading = true;
		}
		UpdateInventory(hero, game);
	}

	UpdateHeroSprite(hero, game);
	hero.sprite.setPosition(pos);
	//kostil
	hero.pos = hero.sprite.getPosition();
	CheckHeroCollision(hero);

	if (hero.health <= 0)
	{
		game.state = END_GAME;
	}
	if (hero.savedNeighbors >= MAX_NUMBER_OF_NEIGHBORS)
	{
		game.state = LEVEL_FINISH;
	}
}

bool IsShotCollision(Game & game, Hero & hero, vector<Shot>::iterator shot)
{
	Vector2f shotCenter;
	shotCenter.x = shot->sprite.getGlobalBounds().width / 2 + shot->pos.x;
	shotCenter.y = shot->sprite.getGlobalBounds().height / 2 + shot->pos.y;

	//checkDeleteShot
	if (shot->distance > SHOT_MAX_DISTANCE)
	{
		return true;
	}
	else if (TILEMAP[int(shotCenter.y) / STEP][int(shotCenter.x) / STEP] != ' ')
	{
		return true;
	}
	else
	{
		for (vector<Zombie>::iterator zombie = game.zombieList.begin(); zombie != game.zombieList.end(); ++zombie)
		{
			if (zombie->sprite.getGlobalBounds().contains(shotCenter))
			{
				zombie->health -= DMG_ITEM[game.inventoryList[hero.slotNo].name];
				return true;
			}
		}
	}
	return false;
}

void UpdateShots(Game & game, Hero & hero, Sprite & sprite_explosion) //shots position update and delete if need
{
	for (vector<Shot>::iterator shot = game.shotList.begin(); shot != game.shotList.end();)
	{
		if (shot->type == BULLET)
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

			if (IsShotCollision(game, hero, shot))  //shot delete
			{
				shot = game.shotList.erase(shot);
			}
			else shot++;
		}


		else if (shot->type == USED_GRENADE)
		{
			Vector2f pos = shot->sprite.getPosition();
			switch (shot->dir)
			{
			case UP:
				pos.y -= STEP_GRENADE;
				break;
			case DOWN:
				pos.y += STEP_GRENADE;
				break;
			case RIGHT:
				pos.x += STEP_GRENADE;
				if ((game.time - shot->startTime) < (GRENADE_MAX_TIME / float(2)))
				{
					pos.y -= 2;
				}
				else
				{
					pos.y += 2;
				}
				break;
			case LEFT:
				pos.x -= STEP_GRENADE;
				if (game.time - shot->startTime < GRENADE_MAX_TIME / 2)
				{
					pos.y -= 2;
				}
				else
				{
					pos.y += 2;
				}
				break;
			}
			cout << pos.x << endl;
			shot->pos = pos;
			shot->sprite.setPosition(pos);

			if (game.time - shot->startTime > GRENADE_MAX_TIME)
			{
				shot->isExploded = true;

				Explosion explosion;
				explosion.sprite = sprite_explosion;
				//explosion.sprite.setTextureRect(IntRect(0, 0, 250, 140));  250 - width  140 - height (/2)
				explosion.pos = { shot->sprite.getPosition().x - 125,shot->sprite.getPosition().y - 70};
				explosion.sprite.setPosition(explosion.pos);
				explosion.currentFrame = 0;
				game.explosionList.push_back(explosion);

				shot = game.shotList.erase(shot);
			}
			else shot++;

		}
	}
}

//zombies
void ZombieSpawn(Game & game, int posX, int posY, Sprite & sprite_zombie)
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

	zombie.spawnTime = game.time;
	zombie.dirChangeTime = 0;

	game.zombieList.push_back(zombie);
}

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
}

void ZombieCheckDir(Game & game, vector<Zombie>::iterator & zombie)
{
	if (zombie->follow == false)
	{
		if (zombie->dirChangeTime == 0)
		{
			cout << " 1 IS " << endl;
			zombie->dir = ComputeRandDir(zombie);
		}
		if (game.time - zombie->dirChangeTime > ZOMBIE_DIR_CHANGE_TIME)
		{
			cout << " 2 IS " << endl;
			zombie->dirChangeTime = 0;
		}
	}
}

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
		}

		zombie->pos.x = x;
		zombie->pos.y = y;
		zombie->sprite.setPosition(zombie->pos.x, zombie->pos.y);
	}
}

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
}

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
			if (hero.state == BEAST)
				hero.health -= (ZOMBIE_DAMAGE / 3);
			else
				hero.health -= (ZOMBIE_DAMAGE);
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

void CheckNpcDeath(Game & game, vector<Zombie>::iterator & zombie)
{
	Vector2f npcCenter;
	Vector2f zombieCenter;
	zombieCenter.x = zombie->pos.x + zombie->sprite.getGlobalBounds().width / 2;
	zombieCenter.y = zombie->pos.y + zombie->sprite.getGlobalBounds().height / 2;
	bool needDeleteNpc;
	for (vector<Npc>::iterator npc = game.npcList.begin(); npc != game.npcList.end();)
	{
		needDeleteNpc = false;

		if (npc->state == NPC_LIVING)  //if loot.item.center contains heroSprite  -> add new item in inventory
		{
			npcCenter.x = npc->sprite.getPosition().x + npc->sprite.getGlobalBounds().width / 2;
			npcCenter.y = npc->sprite.getPosition().y + npc->sprite.getGlobalBounds().height / 2;
			if ((abs(zombieCenter.x - npcCenter.x) < 15) && (abs(zombieCenter.y - npcCenter.y)) < 15)
			{
				npc->state = NPC_DEAD;
			}
		}
		npc++;
	}
}

void UpdateZombies(Game & game, Hero & hero)
{
	//TODO: refact!
	float xHero = hero.sprite.getPosition().x;
	float yHero = hero.sprite.getPosition().y;

	//iterating zombieList
	for (vector<Zombie>::iterator zombie = game.zombieList.begin(); zombie != game.zombieList.end();)
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
				CheckNpcDeath(game, zombie);
				zombie->dir = dir;
			}
			else
			{
				ZombieCheckDir(game, zombie);
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
			zombie = game.zombieList.erase(zombie);
		}
		else zombie++;
	}
}

void ZombieMoveRandom(Game & game)
{
	int rand_dir;
	Direction dir_zombie;

	for (vector<Zombie>::iterator zombie = game.zombieList.begin(); zombie != game.zombieList.end(); ++zombie)
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

void DrawShots(Game & game, Hero & hero)
{
	for (vector<Shot>::iterator shot = game.shotList.begin(); shot != game.shotList.end(); ++shot)
	{	
		shot->sprite.setPosition(shot->pos.x, shot->pos.y);
		game.window->draw(shot->sprite);
	}

	for (vector<Explosion>::iterator explosion = game.explosionList.begin(); explosion != game.explosionList.end();)
	{
		explosion->sprite.setTextureRect(IntRect(0 + 250 * int(explosion->currentFrame), 0, 250, 140));
		explosion->currentFrame += 0.7;
		game.window->draw(explosion->sprite);

		if (explosion->currentFrame > 12)
		{
			//checkDamageZombies;
			for (vector<Zombie>::iterator zombie = game.zombieList.begin(); zombie != game.zombieList.end(); zombie++)
			{
				if (abs(zombie->pos.x - (explosion->pos.x + 120)) < 120 && (abs(zombie->pos.y - (explosion->pos.y + 70)) < 120))
				{
					zombie->health = 0;
				}
			}
			explosion = game.explosionList.erase(explosion);
		}
		else
		{
			explosion++;
		}
	}
}

void DrawZombies(Game & game)
{
	for (vector<Zombie>::iterator zombie = game.zombieList.begin(); zombie != game.zombieList.end(); ++zombie)
	{
		game.window->draw(zombie->sprite);
	}
}

void DrawInventoryText(Game & game, Hero & hero, Text & text, const Vector2f & posView)
{
	text.setPosition(posView.x + 40, posView.y + 40);

	//currentItem << (hero.iCurrent.item);
	std::ostringstream toStringCurrent;
	toStringCurrent << game.inventoryList[hero.slotNo].current;

	std::ostringstream toStringQuantity;
	toStringQuantity << game.inventoryList[hero.slotNo].quantity;
	text.setString(toStringCurrent.str() + "/" + toStringQuantity.str() + " " + ITEM_NAMES[game.inventoryList[hero.slotNo].name]);
	game.window->draw(text);
	if (hero.isReloading)
	{
		text.setPosition(posView.x + 40, posView.y + 70);
		if (game.inventoryList[hero.slotNo].name == PISTOL || game.inventoryList[hero.slotNo].name == RIFLE)
		{
			text.setString("reloading");
		}
		else if (game.inventoryList[hero.slotNo].name == GRENADE)
		{
			//text.setString("pulling");
		}
		else
		{
			//text.setString("opening");
		}
		game.window->draw(text);
	}
	std::ostringstream toStringNeighbors;
	toStringNeighbors << hero.savedNeighbors;
	text.setString("rescued: " + toStringNeighbors.str());
	text.setPosition(posView.x + 5, posView.y + 100);
	game.window->draw(text);

	std::ostringstream toStringRemaining;
	int remaining = MAX_NUMBER_OF_NEIGHBORS - hero.savedNeighbors;
	toStringRemaining << remaining;
	text.setString("remaining: " + toStringRemaining.str());
	text.setPosition(posView.x + 5, posView.y + 120);
	game.window->draw(text);
}

//TODO: change to structs
void DrawBar(Game & game, Hero & hero,View & view, Text & text, Sprite & sprite_bar, Sprite & sprite_health, Sprite & sprite_items)
{
	Vector2f posView;
	posView.x = view.getCenter().x - 600;
	posView.y = view.getCenter().y - 450;

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

	game.inventoryList[hero.slotNo].sprite.setPosition(posView.x + 5, posView.y + 40);

	game.window->draw(game.inventoryList[hero.slotNo].sprite);
	game.window->draw(sprite_bar);
	game.window->draw(sprite_health);
	DrawInventoryText(game, hero, text, posView);
}

void GenerateLoot(Game & game, int  num, NameItem  item, Texture & texture_items)
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
			for (vector<Loot>::iterator i = game.lootList.begin(); i != game.lootList.end(); ++i)
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

				game.lootList.push_back(loot);
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

void DrawLoot(Game & game)
{
	for (vector<Loot>::iterator i = game.lootList.begin(); i != game.lootList.end(); ++i)
	{
		if (i->isDrawn == true)
			game.window->draw(i->sprite);
	}
}

void SpawnZombieRandomly(Game & game, int num,Sprite & sprite_zombie)
{
	int x;
	int y;
	bool need_new_block = false;

	do
	{
		x = (rand() % WIDTH_MAP) * STEP;
		y = (rand() % HEIGHT_MAP) * STEP;
		need_new_block = false;
		Vector2f posRand = { float(x),float(y) };

		if (TILEMAP[y / STEP][x / STEP] == 'b') need_new_block = true;
		else
		{
			for (vector<Zombie>::iterator i = game.zombieList.begin(); i != game.zombieList.end(); ++i)
				if ((abs(i->pos.x - x) < 100 && abs(i->pos.y - y) < 100) || isPosAwayFromHouse(game,posRand) == false)
				{
					need_new_block = true;
					break;
				}
			if (need_new_block == false)
			{
				sprite_zombie.setPosition(x, y);
				ZombieSpawn(game, x, y, sprite_zombie);
				game.zombieList[game.zombieList.size() - 1].sprite = sprite_zombie;
						
				num -= 1;
			}
		}
	} while (num > 0);
}

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
		GenerateLoot(game, 1, item, texture_items);
	}
	if (game.zombieList.size() < 10)
	{
		SpawnZombieRandomly(game, 1, sprite_zombie);
	}
}

void CheckEventNpc(Game & game, Hero & hero)
{
	Vector2f itemCenter;
	Vector2f heroCenter;
	heroCenter.x = hero.pos.x + hero.sprite.getGlobalBounds().width / 2;
	heroCenter.y = hero.pos.y + hero.sprite.getGlobalBounds().height / 2;
	bool needDeleteNpc;
	for (vector<Npc>::iterator npc = game.npcList.begin(); npc != game.npcList.end();)
	{
		needDeleteNpc = false;

		if (npc->state == NPC_LIVING)  //if loot.item.center contains heroSprite  -> add new item in inventory
		{
			itemCenter.x = npc->sprite.getPosition().x + npc->sprite.getGlobalBounds().width / 2;
			itemCenter.y = npc->sprite.getPosition().y + npc->sprite.getGlobalBounds().height / 2;
			if ((abs(heroCenter.x - itemCenter.x) < 15) && (abs(heroCenter.y - itemCenter.y)) < 15)
			{
				//cout << "NEAR" << endl;
				npc->state = NPC_SURVIVED;
				hero.savedNeighbors += 1;
				npc->currentFrame = 0;
			}
			if (npc->health <= 0)
			{
				npc->state = NPC_DEAD;
			}
		}
		if (npc->state == NPC_DEAD)
		{
			if (npc->currentFrame > 8)
			{
				needDeleteNpc = true;
			}
		}
		if (npc->state == NPC_SURVIVED)
		{
			//if (npc->currentFrame > 5)
			{
				needDeleteNpc = true;
			}
		}

		//deleting Npc from List
		if (needDeleteNpc)
		{
			npc = game.npcList.erase(npc);
		}
		else
		{
			npc++;
		}
	}
}

void DoorEvent()
{
	//rnd spot near hero (not in block)

}

void DrawHouses(Game & game)
{
	for (vector<House>::iterator house = game.houseList.begin(); house != game.houseList.end(); house++)
	{
		game.window->draw(house->sprite);
	}
}

void DrawDoors(Game & game)
{
	for (vector<Door>::iterator door = game.doorList.begin(); door != game.doorList.end(); door++)
	{
		if (door->isOpened)
		{
			door->sprite.setTextureRect(sf::IntRect(78, 0, 78, 72));
		}
		else
			door->sprite.setTextureRect(sf::IntRect(0, 0, 78, 72));
		game.window->draw(door->sprite);
	}
}

void InitializeHouse()
{

}


void ComputeNpcFrame(Game & game)
{
	for (vector<Npc>::iterator npc = game.npcList.begin(); npc != game.npcList.end(); npc++)
	{
		if (npc->state == NPC_LIVING)
		{
			switch (npc->type)
			{
			case PHOTOGRAPHS:
				if (npc->currentFrame > 12)
				{
					npc->currentFrame = 0;
				}
				npc->sprite.setTextureRect(IntRect(2 + 41 * int(npc->currentFrame), 350, 41, 50));
				break;
			case BABY:
				if (npc->currentFrame > 7)
				{
					npc->currentFrame = 0;
				}
				npc->sprite.setTextureRect(IntRect(4 + 28 * int(npc->currentFrame), 213, 23, 32));
				break;
			case TEACHER:
				if (npc->currentFrame > 2)
				{
					npc->currentFrame = 0;
				}
				npc->sprite.setTextureRect(IntRect(3 + 37 * int(npc->currentFrame), 2, 37, 41));
				break;
			case GIRL:
				if (npc->currentFrame > 7)
				{
					npc->currentFrame = 0;
				}
				npc->sprite.setTextureRect(IntRect(0 + 50 * int(npc->currentFrame), 92, 50, 67));
				npc->currentFrame += 0.08;
				break;
			case DOG:
				if (npc->currentFrame > 4)
				{
					npc->currentFrame = 0;
				}
				npc->sprite.setTextureRect(IntRect(2 + 34 * int(npc->currentFrame), 49, 31, 34));
				break;
			case SOLDIER:
				if (npc->currentFrame > 3)
				{
					npc->currentFrame = 0;
				}
				npc->sprite.setTextureRect(IntRect(4 + 36 * int(npc->currentFrame), 252, 35, 46));
				break;
			case SEARCHER:
				if (npc->currentFrame > 3)
				{
					npc->currentFrame = 0;
				}
				npc->sprite.setTextureRect(IntRect(4 + 31 * int(npc->currentFrame), 306, 29, 39));
				break;
			case COOK:
				if (npc->currentFrame > 9)
				{
					npc->currentFrame = 0;
				}
				npc->sprite.setTextureRect(IntRect(3 + 54 * int(npc->currentFrame), 454, 52, 66));
				break;
			}
		}
		else if (npc->state == NPC_DEAD)
		{
			npc->sprite.setTextureRect(IntRect(4 + 45 * int(npc->currentFrame), 593, 53, 45));
			npc->currentFrame += 0.06;
		}
		npc->currentFrame += 0.02;
	}
}

void DrawNpc(Game & game)
{
	for (vector<Npc>::iterator npc = game.npcList.begin(); npc != game.npcList.end(); npc++)
	{
		game.window->draw(npc->sprite);
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
	//text
	Font font;
	font.loadFromFile("Font/Arialbd.ttf");
	Text text("", font, 20);
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


	View view;
	Clock clock;
	Clock gameSpeedClock;
	//camera
	view.reset(sf::FloatRect(0, 0, 1280, 1024));

	//initializeGame
	Game * game = new Game();
	Hero * hero = new Hero();
	InitializeGame(*game);
	InitializeHero(*hero);

	game->inventoryList[0].texture = texture_items;
	game->inventoryList[0].sprite.setTexture(texture_items);
	game->inventoryList[0].sprite.setTextureRect(IntRect(32, 0, 32, 32));
	game->explosionTexture = texture_explosion;
	game->explosionSprite = sprite_explosion;
	game->font = font;
	game->text = text;

	House house;
	house.pos = { 47 * STEP, 5 * STEP };
	house.texture = texture_house;
	house.sprite = sprite_house;
	house.sprite.setPosition(house.pos);
	game->houseList.push_back(house);

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
	ZombieSpawn(*game,100, 100, sprite_zombie);

	//bool is_game_over = false;

	//initializeLoot
	GenerateLoot(*game,5, DRINK, texture_items);
	GenerateLoot(*game,3, PISTOL, texture_items);
	GenerateLoot(*game,2, AMMO, texture_items);
	GenerateLoot(*game,1, RIFLE, texture_items);
	GenerateLoot(*game,1, KEY, texture_items);
	GenerateLoot(*game,1, MIXTURE, texture_items);
	GenerateLoot(*game, 2, GRENADE, texture_items);

	InitializeNpc(*game, texture_npc);

	bool switch_status = false; //for ProcessEvents(correct inventory-switch)
								//bool fire_status = false; //дл€ стрельбы 
	float shot_last_time = 0;
	float gameSpeedTime = 0;


	while (game->window->isOpen())
	{
		gameSpeedTime = gameSpeedClock.getElapsedTime().asMilliseconds();
		if (gameSpeedTime > SCREEN_UPDATE_TIME)
		{
			gameSpeedClock.restart();
			game->window->clear();
			game->time = clock.getElapsedTime().asSeconds();

			cout << hero->savedNeighbors << endl;

			if (game->state == START_GAME)
			{
				BeginEvent(*game, view);
			}
			else if (game->state == END_GAME)
			{
				EndGameEvent(*game, view);
			}
			else if (game->state == LEVEL_FINISH)
			{
				LevelFinishEvent(*game, view);
			}
			else if (game->state == PLAY)
			{
				//TODO: spawn zombie at definite time (and change SpawnZombie func (spawn only near hero))

				CheckSpawnZombiesAndLoot(*game, texture_items, sprite_zombie);
				ProcessEvents(*game, *hero, switch_status, shot_last_time, sprite_shot, sprite_grenade);
				UpdateHero(*game, *hero);
				CheckEventNpc(*game, *hero);
				UpdateView(*game, *hero, view);
				UpdateShots(*game, *hero, sprite_explosion);

				
				UpdateZombies(*game, *hero);
				CheckLoot(*hero, texture_items, *game);
				UpdateInventory(*hero, *game);

				//Drawing
				DrawMap(*game, mapSprite);
				DrawLoot(*game);
				DrawShots(*game, *hero);
				DrawHouses(*game);
				DrawDoors(*game);
				DrawNpc(*game);
				DrawZombies(*game);
				DrawHero(*game, *hero);

				ComputeNpcFrame(*game);
				DrawNpc(*game);
				DrawBar(*game, *hero, view, text, sprite_bar, sprite_health, sprite_items);
			}
			game->window->display();
		}
	}
}