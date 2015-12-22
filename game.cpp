#include "game.h"


void InitializeLoot(Game & game)
{
	GenerateLoot(game.lootList, game.lvl.GetAllObjects(), 5, DRINK, game.sprites.items);
	GenerateLoot(game.lootList, game.lvl.GetAllObjects(), 3, PISTOL, game.sprites.items);
	GenerateLoot(game.lootList, game.lvl.GetAllObjects(), 2, AMMO, game.sprites.items);
	GenerateLoot(game.lootList, game.lvl.GetAllObjects(), 1, RIFLE, game.sprites.items);
	GenerateLoot(game.lootList, game.lvl.GetAllObjects(), 1, KEY, game.sprites.items);
	GenerateLoot(game.lootList, game.lvl.GetAllObjects(), 1, MIXTURE, game.sprites.items);
	GenerateLoot(game.lootList, game.lvl.GetAllObjects(), 2, GRENADE, game.sprites.items);
}

void InitializeGame(Game & game)
{
	game.hero = new Hero();
	InitializeInventory(game.inventoryList, game.sprites);
	InitializeHero(*game.hero);
	game.hero->item = game.inventoryList[0];
	game.window = new RenderWindow(VideoMode(W_WIDTH, W_HEIGHT), "Shoot and Run");
	InitializeView(game.view);
	InitiazlizeSprites(game.sprites);
	InitializeText(game.font,game.text);
	game.lvl.LoadFromFile("level0.tmx");
	game.solidObjects = game.lvl.GetObjects("solid");

	ZombieSpawn(game.zombieList, game.time, 100, 100, game.sprites.zombie);
	InitializeLoot(game);
	InitializeNpc(game.npcList, game.sprites.npc);
	game.time = 0;
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
		explosion++;
	}
};


void DrawZombies(RenderWindow & window, vector<Zombie> & zombieList)
{
	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
	{
		window.draw(zombie->sprite);
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


bool IsShotCollision(vector<Zombie> & zombieList, NameItem & weapon, vector<Object> & objects, vector<Shot>::iterator  shot)
{
	Vector2f shotCenter = GetSpriteCenter(shot->sprite);
	//checkDeleteShot
	if (shot->distance > SHOT_MAX_DISTANCE)
	{
		return true;
	}
	else
	{
		FloatRect spriteRect = GetSpriteRect(shot->sprite);
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (spriteRect.intersects(objects[i].rect))
			{
				return true;
			}
		}
	}

	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end(); ++zombie)
	{
		if (zombie->sprite.getGlobalBounds().contains(shotCenter))
		{
			zombie->health -= DMG_ITEM[weapon];
			return true;
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

			if (IsShotCollision(game.zombieList,hero.item.name,game.solidObjects, shot))  //shot delete
			{
				shot = game.shotList.erase(shot);
			}
			else shot++;
		}
		else if (shot->type == USED_GRENADE)
		{
			switch (shot->dir)
			{
			case UP:
				shot->pos.y -= STEP_GRENADE;
				break;
			case DOWN:
				shot->pos.y += STEP_GRENADE;
				break;
			case RIGHT:
				shot->pos.x += STEP_GRENADE;
				if ((game.time - shot->startTime) < (GRENADE_MAX_TIME / float(2)))
				{
					shot->pos.y -= 2;
				}
				else
				{
					shot->pos.y += 2;
				}
				break;
			case LEFT:
				shot->pos.x -= STEP_GRENADE;
				if (game.time - shot->startTime < GRENADE_MAX_TIME / 2)
				{
					shot->pos.y -= 2;
				}
				else
				{
					shot->pos.y += 2;
				}
				break;
			}

			shot->sprite.setPosition(shot->pos);

			if (game.time - shot->startTime > GRENADE_MAX_TIME)
			{
				shot->isExploded = true;

				Explosion explosion;
				explosion.sprite = sprite_explosion;
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


void CheckCollisionWithMap(Sprite & sprite, Direction & dir, const float & speed, vector<Object> &objects)
{
	FloatRect spriteRect = GetSpriteRect(sprite);

	bool q = false; //is TopLeft position collide
	bool w = false; //is TopRight position collide
	bool e = false; //is BottomRight position collide
	bool r = false; //is BottomLeft position collide

	for (size_t i = 0; i < objects.size(); ++i)
	{
		if (spriteRect.intersects(objects[i].rect))
		{
			if (objects[i].name == "solid")
			{
				//cout << "ASDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD" << endl;
				//cout << "SPRITE " << obj[i].sprite.getPosition().x << " " << obj[i].sprite.getPosition().y<< endl;
				switch (dir)
				{
				case UP:
					//cout << "WE DID IT" << endl;
					sprite.setPosition(spriteRect.left, spriteRect.top + speed);
					break;
				case RIGHT:
					//cout << "WE DID IT" << endl;
					sprite.setPosition(spriteRect.left - speed, spriteRect.top);
					break;
				case DOWN:
					//cout << "WE DID IT" << endl;
					sprite.setPosition(spriteRect.left, spriteRect.top - speed);
					break;
				case LEFT:
					//cout << "WE DID IT" << endl;
					sprite.setPosition(spriteRect.left + speed, spriteRect.top);
					break;
				default:
					break;
				}
			}
		}

	}
};

void UpdateZombies(vector<Zombie> & zombieList, Hero & hero, vector<Npc> & npcList, vector<Object> & objects, float & time)
{
	Vector2f heroPos = hero.sprite.getPosition();
	for (vector<Zombie>::iterator zombie = zombieList.begin(); zombie != zombieList.end();)
	{
		//float di = sqrt(pow(dx,2) + pow(dy,2));  //distance
		if (zombie->state == ACTIVE)
		{
			ZombieCheckFollow(zombie, hero);
			if (zombie->follow)
			{
				ComputeZombieDirection(zombie,heroPos);  //direction relatively to hero
				CheckNpcDeath(npcList, zombie);
			}
			else
			{
				ZombieCheckDir(zombie, time);
			}

			CheckCollisionWithMap(zombie->sprite,zombie->dirLast,STEP_ZOMBIE_ACTIVE, objects);
			ZombieUpdateAttack(hero, zombie, time);

			if (hero.state == BEAST)
			{
				if (IsZombieNearHero(hero, zombie))
				{
					CheckHeroBeastDamage(hero, zombie, time);
				}
			}
			if (heroPos.x != zombie->pos.x || heroPos.y != zombie->pos.y)
			{
				if (zombie->follow)
				{
					ZombieUpdatePosition(zombie);
				}
			}
		}
		ZombieUpdateSprite(zombie);
		//UpdateZombiePosition(i);  TODO: make it for all zombies, not jsut for following ones
 
		if (zombie->dir != zombie->dirLast)
		{
			//zombie->currentFrame = 0;
			zombie->dirLast = zombie->dir;
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
			pos.x += (DIAGONAL_STEP * STEP_HERO);
			pos.y -= (DIAGONAL_STEP * STEP_HERO);
			break;
		case RIGHT:
			pos.x += STEP_HERO;
			break;
		case DOWNRIGHT:
			pos.x += (DIAGONAL_STEP * STEP_HERO);
			pos.y += (DIAGONAL_STEP * STEP_HERO);
			break;
		case DOWN:
			pos.y += STEP_HERO;
			break;
		case DOWNLEFT:
			pos.x -= (DIAGONAL_STEP * STEP_HERO);
			pos.y += (DIAGONAL_STEP * STEP_HERO);
			break;
		case LEFT:
			pos.x -= STEP_HERO;
			break;
		case UPLEFT:
			pos.x -= (DIAGONAL_STEP * STEP_HERO);
			pos.y -= (DIAGONAL_STEP * STEP_HERO);
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

	if ((game.inventoryList[game.hero->slotNo].quantity > 0 && game.inventoryList[game.hero->slotNo].current == 0 && (game.inventoryList[game.hero->slotNo].name == PISTOL || game.inventoryList[game.hero->slotNo].name == RIFLE)))
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
	CheckCollisionWithMap(game.hero->sprite, game.hero->dirLast,STEP_HERO,game.lvl.GetObjects("solid"));
}

void ProcessEvents(Game & game, Sprites & sprites)
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
		CheckUsingItems(*game.hero, game.inventoryList, game.shotList, game.time, sprites.shot, sprites.grenade);

		//CheckWindowClose
		if (Keyboard::isKeyPressed(Keyboard::Escape) || event.type == Event::Closed)
			game.window->close();
	}
}


void Render(Game & game)
{
	DrawLoot(*game.window, game.lootList);
	DrawShots(*game.window, game.shotList, game.explosionList, *game.hero);
	DrawNpc(*game.window, game.npcList);
	DrawZombies(*game.window, game.zombieList);
	DrawHero(*game.window, *game.hero);
};


void CheckSpawnZombiesAndLoot(Game & game, Sprite & items, Sprite & sprite_zombie)
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
		GenerateLoot(game.lootList, game.lvl.GetAllObjects(), 1, item, items);
	}
	if (game.zombieList.size() < 10)
	{
		SpawnZombieRandomly(game.zombieList,game.lvl.GetAllObjects(), 1, game.time, sprite_zombie);
	}
}

void DrawBar(RenderWindow & window, vector<Inventory> & inventoryList, Hero & hero, View & view, Text & text, Sprites & sprites)
{
	Vector2f posView = GetInterfacePosition(view);

	sprites.bar.setTextureRect(IntRect(0, 0, 170, 35));
	sprites.bar.setPosition(posView);

	sprites.health.setTextureRect(IntRect(1, 0, 146 * (float(hero.health) / 100), 29));
	sprites.health.setPosition(posView.x + 10, posView.y + 1);

	inventoryList[hero.slotNo].sprite.setPosition(posView.x + 5, posView.y + 40);

	window.draw(inventoryList[hero.slotNo].sprite);
	window.draw(sprites.bar);
	window.draw(sprites.health);
}


void StartGame(Game * game)
{
	Clock clock;
	Clock gameSpeedClock;
	float gameSpeedTime = 0;
	while (game->window->isOpen())
	{
		gameSpeedTime = gameSpeedClock.getElapsedTime().asMilliseconds();
		if (gameSpeedTime > SCREEN_UPDATE_TIME)
		{
			gameSpeedClock.restart();
			game->time = clock.getElapsedTime().asSeconds();

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

				//cout << "TIME  " << game->time << endl;

				CheckSpawnZombiesAndLoot(*game, game->sprites.items, game->sprites.zombie);
				ProcessEvents(*game, game->sprites);
				UpdateHero(*game);
				UpdateInventory(*game->hero, game->inventoryList, game->time);
				CheckEventNpc(game->npcList, *game->hero);
				UpdateView(*game->window, game->hero->sprite, game->view);
				UpdateShots(*game, *game->hero, game->sprites.explosion);

				UpdateZombies(game->zombieList, *game->hero, game->npcList, game->solidObjects, game->time);
				CheckZombieExplosion(game->explosionList, game->zombieList);

				CheckLoot(*game->hero, game->lootList, game->inventoryList, game->sprites.items);
				UpdateInventory(*game->hero, game->inventoryList, game->time);

				ComputeNpcFrame(game->npcList);
				CheckGameOver(game->state, *game->hero);

				//Drawing
				game->lvl.Draw(*game->window);//������ ����� �����

				DrawInventoryText(*game->window, game->inventoryList, *game->hero, game->view, game->text);

				Render(*game);
				DrawBar(*game->window, game->inventoryList, *game->hero, game->view, game->text, game->sprites);
				break;
			}
			game->window->display();
		}
	}
};


void DeleteGame(Game * game)
{
	delete(game->hero);
	delete(game->window);
	DeleteExplosionList(game->explosionList);
	DeleteLoot(game->lootList);
	DeleteNpcList(game->npcList);
	DeleteShots(game->shotList);
	DeleteZombieList(game->zombieList);
	DeleteInventory(game->inventoryList);
	delete(game);
}
