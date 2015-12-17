#pragma once
#include <SFML/Graphics.hpp>

#include "declarations.h"

#include "map.h"
#include "shot.h"
#include "hero.h"

#include "level.h"
#include "npc.h"

#include "zombie.h"

#include "loot.h"

using namespace sf;
using namespace std;

const int W_WIDTH = 1280;
const int W_HEIGHT = 1024;








struct FinalDoor
{
	Texture texture;
	Sprite sprite;
	int currentFrame;
};



struct House
{
	Texture texture;
	Sprite sprite;
	Vector2f pos;
};

struct Door
{
	Texture texture;
	Sprite sprite;
	Vector2f pos;
	bool isOpened;
};

struct FinalDoor1
{
	Texture texture;
	Sprite sprite;
	Vector2f pos;
	int currentFrame;
	bool isOpened;
};


struct Game
{
	Hero * hero;
	float time;
	RenderWindow * window;
	std::vector<Inventory> inventoryList;
	std::vector<Loot> lootList;
	std::vector<Shot> shotList;
	std::vector<Zombie> zombieList;
	std::vector<Npc> npcList;
	std::vector<Door> doorList;
	std::vector<House> houseList;


	View view;

	Font font;
	Text text;

	GameState state;

	std::vector<Explosion> explosionList;
	Texture explosionTexture;
	Sprite explosionSprite;
};


//.CPP

void InitializeGame(Game & game)
{
	game.hero = new Hero();
	InitializeHero(*game.hero);


	game.time = 0;
	game.window = new RenderWindow(VideoMode(W_WIDTH, W_HEIGHT), "Shoot and Run");
	game.view.reset(sf::FloatRect(0, 0, 1280, 1024)); //camera

	Inventory inventory;
	inventory.name = PISTOL;
	inventory.current = 0;
	inventory.quantity = 7;
	inventory.texture.loadFromFile("images/items.png");
	inventory.sprite.setTexture(inventory.texture);
	inventory.sprite.setTextureRect(IntRect(32, 0, 32, 32));
	game.inventoryList.push_back(inventory);
	game.inventoryList[0].sprite.setTexture(inventory.texture);
	game.state = START_GAME;

};


void CheckWindowClose(Game & game)
{
	Event event;
	while (game.window->pollEvent(event))
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape) || event.type == Event::Closed)
		{
			game.window->close();
		}
	}
};

void BeginEvent(Game & game, View & view)
{
	Vector2f posView = view.getCenter();

	game.text.setString("It's time to rescue neighbors!");
	game.text.setPosition(posView.x - 140, posView.y - 100);
	game.window->draw(game.text);

	game.text.setString("Press Enter to play");
	game.text.setPosition(posView.x - 100, posView.y - 40);
	game.window->draw(game.text);

	if (Keyboard::isKeyPressed(Keyboard::Return))
	{
		game.state = PLAY;
	}
	CheckWindowClose(game);
};

void EndGameEvent(Game & game, View & view)
{
	Vector2f posView = view.getCenter();

	game.text.setString("GAME OVER");
	game.text.setPosition(posView.x - 40, posView.y - 100);
	game.window->draw(game.text);
	CheckWindowClose(game);
	/*
	std::ostringstream playerScoreString;
	playerScoreString << (snake->size - 1) * 10;
	game.text.setString("SCORE: " + playerScoreString.str());
	game.text.setPosition(240, 250);
	game.window.draw(game.text);
	

	game.text.setString("Press Enter to restart");
	game.text.setPosition(160, 350);
	game.window.draw(game.text);
	*/

	if (Keyboard::isKeyPressed(Keyboard::Return))
	{
		game.state = PLAY;
	}
};


void LevelFinishEvent(Game & game, View & view)
{
	Vector2f posView = view.getCenter();

	game.text.setString("LEVEL HAS FINISHED!");
	game.text.setPosition(posView.x - 100, posView.y - 100);
	game.window->draw(game.text);
	CheckWindowClose(game);
};


void DrawShots(RenderWindow & window, vector<Shot> & shotList, vector<Explosion> & explosionList, Hero & hero)
{
	for (vector<Shot>::iterator shot = shotList.begin(); shot != shotList.end(); ++shot)
	{
		shot->sprite.setPosition(shot->pos.x, shot->pos.y);
		window.draw(shot->sprite);
	}

	for (vector<Explosion>::iterator explosion = explosionList.begin(); explosion != explosionList.end();)
	{
		explosion->sprite.setTextureRect(IntRect(0 + 250 * int(explosion->currentFrame), 0, 250, 140));
		explosion->currentFrame += 0.7;
		window.draw(explosion->sprite);
	}
};


void DrawZombies(RenderWindow & window, vector<Zombie> & zombieList)
{
	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
	{
		window.draw(zombie->sprite);
	}
};


void DrawDoors(RenderWindow & window, vector<Door> & doorList)
{
	for (vector<Door>::iterator door = doorList.begin(); door != doorList.end(); door++)
	{
		if (door->isOpened)
		{
			door->sprite.setTextureRect(sf::IntRect(78, 0, 78, 72));
		}
		else
			door->sprite.setTextureRect(sf::IntRect(0, 0, 78, 72));
		window.draw(door->sprite);
	}
};


void DrawNpc(RenderWindow & window, vector<Npc> & npcList)
{
	for (vector<Npc>::iterator npc = npcList.begin(); npc != npcList.end(); npc++)
	{
		window.draw(npc->sprite);
	}
};


void CheckHeroBeastDamage(Hero & hero, vector<Zombie>::iterator & zombie, float & time)  //killing zombie with melee attack
{
	if (hero.isBeastAttack && ((time - hero.lastAttackTime) > HERO_BEAST_ATTACK_TIME))
	{
		zombie->health -= HERO_BEAST_DAMAGE;
		hero.lastAttackTime = time;
	}
}

bool IsShotCollision(vector<Zombie> & zombieList, Hero & hero, vector<Shot>::iterator shot)
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
		for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
		{
			if (zombie->sprite.getGlobalBounds().contains(shotCenter))
			{
				zombie->health -= DMG_ITEM[hero.item.name];
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

			if (IsShotCollision(game.zombieList, hero, shot))  //shot delete
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
				explosion.pos = { shot->sprite.getPosition().x - 125,shot->sprite.getPosition().y - 70 };
				explosion.sprite.setPosition(explosion.pos);
				explosion.currentFrame = 0;
				game.explosionList.push_back(explosion);

				shot = game.shotList.erase(shot);
			}
			else shot++;

		}
	}
}


void UpdateZombies(vector<Zombie> & zombieList, Hero & hero, vector<Npc> & npcList, float & time)
{
	//TODO: ref
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
				ZombieCheckDir(zombie, time);
			}

			CheckZombieCollision(zombie);
			ZombieUpdateAttack(hero, zombie, time);

			if (hero.state == BEAST)
			{
				if (IsZombieNearHero(hero, zombie))
				{
					CheckHeroBeastDamage(hero, zombie, time);
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

		if (zombie->state == EXPLODED)  //deleting
		{
			zombie = zombieList.erase(zombie);
		}
		else zombie++;
	}
}


void UpdateHero(Game & game) //position + collision + sprite
{
	Vector2f pos = game.hero->sprite.getPosition();

	if (game.hero->state == NORMAL || game.hero->state == BEAST)
	{
		switch (game.hero->dir)
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
	if (game.hero->state == BEAST)
	{
		if (game.time - game.hero->beastTimer > BEAST_MAX_TIME)
		{
			game.hero->state = NORMAL;
			game.hero->currentFrame = 0;
			game.hero->beastTimer = 0;
		}
	}

	if ((game.inventoryList[game.hero->slotNo].name == PISTOL || game.inventoryList[game.hero->slotNo].name == RIFLE) && game.inventoryList[game.hero->slotNo].current == 0 && game.inventoryList[game.hero->slotNo].quantity > 0)
	{
		if (game.hero->isReloading == false)
		{
			game.hero->lastReloadTime = game.time;
			game.hero->isReloading = true;
		}
	}

	UpdateHeroSprite(*game.hero, game.time);
	game.hero->sprite.setPosition(pos);
	game.hero->pos = game.hero->sprite.getPosition();  //#for
	CheckHeroCollision(*game.hero);
}

void ProcessEvents(Game & game, Sprite & sprite_shot, Sprite & sprite_grenade)
{
	Event event;
	while (game.window->pollEvent(event))
	{
		UpdateDirection(*game.hero);


		//inventorySwitch
		if (IsInventorySwitch(*game.hero))
		{
			game.hero->item = game.inventoryList[game.hero->slotNo];
		}
		CheckUsingItems(*game.hero, game.inventoryList, game.shotList, game.time, sprite_shot, sprite_grenade);

		//Window close event
		if (Keyboard::isKeyPressed(Keyboard::Escape) || event.type == Event::Closed)
			game.window->close();
	}
}


void Render(Game & game, Hero & hero)
{
	DrawLoot(*game.window, game.lootList);
	DrawShots(*game.window, game.shotList, game.explosionList,hero);
	//DrawHouses(*game);
	DrawDoors(*game.window, game.doorList);
	DrawNpc(*game.window, game.npcList);
	DrawZombies(*game.window, game.zombieList);
	DrawHero(*game.window, hero);
};

