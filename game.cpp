#include "game.h"


void InitializeGame(Game & game)
{
	game.hero = new Hero();
	InitiazlizeSprites(game.sprites);
	InitializeInventory(game.inventoryList, game.sprites);
	InitializeHero(*game.hero);
	game.hero->item = game.inventoryList[0];
	game.hero->item.sprite.setTextureRect(IntRect(32, 0, 32, 32));
	game.window = new RenderWindow(VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Shoot and Run");
	InitializeView(game.view);
	InitializeText(game.font,game.text);
	game.lvl.LoadFromFile("level0.tmx");
	game.allObjects = game.lvl.GetAllObjects();
	game.solidObjects = game.lvl.GetObjects("solid");

	//ZombieSpawn(game.zombieList, game.time, 100, 100, game.sprites,COMMON);
	SpawnEnemy(game.zombieList, game.time, 100, 100, game.sprites, AXE);
	InitializeLoot(game.lootList, game.allObjects, game.sprites.items);
	InitializeNpc(game.npcList, game.sprites.npc);
	game.time = 0;
	game.state = START_GAME;
	//game.hero->sprite.setPosition(game.view.getCenter());
	InitializeMinimap(game.miniMap, game.npcList, game.sprites);
};

void DeleteGame(Game * game)
{
	delete(game->hero);
	delete(game->window);
	DeleteExplosionList(game->explosionList);
	DeleteLoot(game->lootList);
	DeleteNpcList(game->npcList);
	DeleteShots(game->shotList);
	DeleteEnemyList(game->zombieList);
	DeleteInventory(game->inventoryList);
	delete(game);
}

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
		//game.time = 0;
		ResetEnemySpawnTime(game.zombieList, game.time);
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
		delete((&game)->hero);
		//delete((&game)->window);
		DeleteExplosionList((&game)->explosionList);
		DeleteLoot((&game)->lootList);
		DeleteNpcList((&game)->npcList);
		DeleteShots((&game)->shotList);
		DeleteEnemyList((&game)->zombieList);
		DeleteInventory((&game)->inventoryList);
		DeleteMinimap(game.miniMap);
		//delete((&game));

		game.hero = new Hero();
		//InitiazlizeSprites(game.sprites);
		InitializeInventory(game.inventoryList, game.sprites);
		InitializeHero(*game.hero);
		game.hero->item = game.inventoryList[0];
		game.hero->item.sprite.setTextureRect(IntRect(32, 0, 32, 32));
		//game.window = new RenderWindow(VideoMode(W_WIDTH, W_HEIGHT), "Shoot and Run");
		//InitializeView(game.view);
		//InitializeText(game.font, game.text);
		//game.lvl.LoadFromFile("level0.tmx");
		//game.solidObjects = game.lvl.GetObjects("solid");

		SpawnEnemy(game.zombieList, game.time, 100, 100, game.sprites, COMMON);
		InitializeLoot(game.lootList, game.allObjects, game.sprites.items);
		InitializeNpc(game.npcList, game.sprites.npc);
		InitializeMinimap(game.miniMap, game.npcList, game.sprites);
		game.time = 0;
		game.state = START_GAME;
		//game.hero->sprite.setPosition(game.view.getCenter());
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
	for (Shot & shot: shotList)
	{
		shot.sprite.setPosition(shot.pos);
		window.draw(shot.sprite);
	}
	for (Explosion & explosion: explosionList)
	{
		explosion.sprite.setTextureRect(IntRect(0 + 250 * int(explosion.currentFrame), 0, 250, 140));
		explosion.currentFrame += 0.7f;
		window.draw(explosion.sprite);
	}
};

void CheckHeroBeastDamage(Hero & hero, Enemy & enemy, float & time)  //killing zombie by melee attack
{
	if (hero.isBeastAttack && ((time - hero.lastAttackTime) > HERO_BEAST_ATTACK_TIME))
	{
		enemy.health -= HERO_BEAST_DAMAGE;
		hero.lastAttackTime = time;
	}
}

bool IsShotCollision(vector<Enemy> & zombieList, NameItem & weapon, vector<Object> & objects, Shot & shot)  //check if need delete shot from list
{
	Vector2f shotCenter = GetSpriteCenter(shot.sprite);

	if (shot.distance > SHOT_MAX_DISTANCE)
	{
		return true;
	}
	else
	{
		FloatRect spriteRect = GetSpriteRect(shot.sprite);
		if (std::any_of(objects.begin(), objects.end(), [&](Object object) {return object.rect.intersects(spriteRect); }))
			return true;
		/*
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (spriteRect.intersects(objects[i].rect))
			{
				return true;
			}
		}
		*/
	}

	for (Enemy & enemy: zombieList)
	{
		if (enemy.sprite.getGlobalBounds().contains(shotCenter))
		{
			enemy.health -= DMG_ITEM[weapon];
			return true;
		}
	}
	return false;
}

void UpdateShots(Game & game, Hero & hero, Sprite & sprite_explosion) //shots position update and delete if need
{
	for (vector<Shot>::iterator shot = game.shotList.begin(); shot != game.shotList.end();)
	{
		//TODO: PART FUNCTION
		if (shot->type == BULLET)
		{
			shot->pos = ComputeSpriteNewPosition(shot->sprite, shot->dir, STEP_SHOT);
			shot->distance += STEP_SHOT;

			if (IsShotCollision(game.zombieList,hero.item.name,game.solidObjects, *shot))  //shot delete
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
				if ((game.time - shot->startTime) < (GRENADE_MAX_TIME / 2.f))
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
				if (game.time - shot->startTime < GRENADE_MAX_TIME / 2.f)
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

Vector2f ComputeSpriteNewPosition(Sprite & sprite, Direction & dir, const float & speed)
{
	Vector2f pos = sprite.getPosition();
	switch (dir)
	{
	case UP:
		pos.y -= speed;
		break;
	case UPRIGHT:
		pos.x += (DIAGONAL_STEP * speed);
		pos.y -= (DIAGONAL_STEP * speed);
		break;
	case RIGHT:
		pos.x += speed;
		break;
	case DOWNRIGHT:
		pos.x += (DIAGONAL_STEP * speed);
		pos.y += (DIAGONAL_STEP * speed);
		break;
	case DOWN:
		pos.y += speed;
		break;
	case DOWNLEFT:
		pos.x -= (DIAGONAL_STEP * speed);
		pos.y += (DIAGONAL_STEP * speed);
		break;
	case LEFT:
		pos.x -= speed;
		break;
	case UPLEFT:
		pos.x -= (DIAGONAL_STEP * speed);
		pos.y -= (DIAGONAL_STEP * speed);
		break;
	case NONE:
		break;
	default:
		break;
	}
	return pos;
}

bool IsCollisionWithMap(Sprite & sprite, Direction & dir, vector<Object> &objects)
{
	FloatRect spriteRect = GetSpriteRect(sprite);

	for (size_t i = 0; i < objects.size(); ++i)
	{
		if (spriteRect.intersects(objects[i].rect))
		{
				return true;
		}
	}
	return false;
};

void UpdateEnemies(vector<Enemy> & zombieList, Hero & hero, vector<Npc> & npcList, vector<Object> & solidObjects, vector<Throwing> & throwingList, Sprites & gameSprites, float & time)
{
	Vector2f heroPos = hero.sprite.getPosition();
	int index = 0;
	for (vector<Enemy>::iterator zombie = zombieList.begin(); zombie != zombieList.end();)
	{
		//float di = sqrt(pow(dx,2) + pow(dy,2));  //distance
		if (zombie->state == ACTIVE)
		{
			CheckEnemyFollow(*zombie, hero);
			if (zombie->follow)
			{
				ComputeEnemyDirection(*zombie,heroPos);  //direction relatively to hero
				CheckNpcDeath(npcList, *zombie);
			}
			else
			{
				CheckEnemyDir(*zombie, time);
			}

			//CheckCollisionWithMap(zombie->sprite,zombie->dirLast,STEP_ZOMBIE_ACTIVE, objects);
			UpdateEnemyAttack(hero, *zombie, time);

			if (hero.state == BEAST)
			{
				if (IsEnemyNearHero(hero, *zombie))
				{
					CheckHeroBeastDamage(hero, *zombie, time);
				}
			}
			if (heroPos.x != zombie->pos.x || heroPos.y != zombie->pos.y)
			{
				if (zombie->follow)
				{
					//ZombieUpdatePosition(*zombie);
					//Vector2f newPos;
					Vector2f oldPosition = zombie->sprite.getPosition();
					if (zombie->follow == true)
					{
						UpdateSpritePosition(zombie->sprite, zombie->dir, STEP_ZOMBIE_ACTIVE, solidObjects);
						//cout << "1     1" << endl;
					}
					else
					{
						UpdateSpritePosition(zombie->sprite, zombie->dir, STEP_ZOMBIE, solidObjects);
						//cout << "2      2" << endl;
					}
					if (IsIntersectWithOtherEnemy(zombieList, index))
					{
						zombie->sprite.setPosition(oldPosition);
					}
					if (zombie->type == AXE)
					{
						if (IsIntersectWithHero(zombie->sprite, hero.sprite))
						{
							//zombie->pos = oldPosition;
							zombie->sprite.setPosition(oldPosition);
							if (time > zombie->attack_time + zombie->attackCooldown)
							{
								zombie->currentFrame = 0;
								zombie->isAttack = true;
								zombie->attack_time = time;
								hero.health -= ENEMY_AXE_ATTACK_DAMAGE;
								//cout << "ATTACKING!! " << endl;
							}
						}
						if (zombie->last_action_time + ENEMY_AXE_ACTION_COOLDOWN < time)
						{
							Vector2f spriteDistance = ComputeDistanceBetweenSprites(zombie->sprite, hero.sprite);
							zombie->last_action_time = time;
							//zombie->
							cout << "ACTION!!!  " << endl;
							throwingList.push_back(CreateThrowing(zombie->sprite,hero.sprite, "axe", gameSprites.throwing_axe, time));
							
						}
					}
					zombie->pos = zombie->sprite.getPosition();
				}
			}
		}

		UpdateEnemyFrame(*zombie, time);
		//UpdateZombiePosition(i);  TODO: make it for all zombies, not jsut for following ones
		//zombie->isAttack = false;
		if (zombie->dir != zombie->dirLast)
		{
			//zombie->currentFrame = 0;
			zombie->dirLast = zombie->dir;
		}
		if (zombie->state == EXPLODED)  //deleting
		{
			zombie = zombieList.erase(zombie);
		}
		else
		{
			zombie++;
			index++;
		}
	}
}

void CheckEventNpc(vector<Npc> & npcList, Hero & hero, MiniMap & miniMap)
{
	Vector2f heroCenter = GetSpriteCenter(hero.sprite);
	bool needDeleteNpc = false;
	bool isAnyNpcChanged = false;
	int i = 0;
	for (vector<Npc>::iterator npc = npcList.begin(); npc != npcList.end();)
	{
		needDeleteNpc = false;
		if (npc->state == LIVING)
		{
			Vector2f npcCenter = GetSpriteCenter(npc->sprite);
			if ((abs(npcCenter.x - heroCenter.x) < 35) && (abs(npcCenter.y - heroCenter.y)) < 35)
			{
				npc->state = SURVIVED;
				hero.savedNeighbors += 1;
				needDeleteNpc = true;
				isAnyNpcChanged = true;
			}
		}
		if (npc->health <= 0)
		{
			npc->state = KILLED;
			isAnyNpcChanged = true;
		}
		if (npc->state == KILLED && npc->currentFrame > 8)
			needDeleteNpc = true;

		if (needDeleteNpc) //deleting Npc from List
		{
			npc = npcList.erase(npc);
			DeleteNpcDot(miniMap, i);
		}
		else
		{
			npc++;
		}
		if (isAnyNpcChanged)
		{
			//SortNpcList(npcList);
		}
	}
};


void UpdateSpritePosition(Sprite & sprite, Direction & dir, const float & speed, vector<Object> & solidObjects)
{
	Vector2f oldPos = sprite.getPosition();
	Vector2f pos = ComputeSpriteNewPosition(sprite, dir, speed);

	sprite.setPosition(pos);

	if (IsCollisionWithMap(sprite, dir, solidObjects))
	{
		sprite.setPosition(oldPos);
		if (pos.x != oldPos.x && pos.y != oldPos.y)
		{
			sprite.setPosition(oldPos.x, pos.y);
			if (IsCollisionWithMap(sprite, dir, solidObjects))
			{
				sprite.setPosition(pos.x, oldPos.y);
				if (IsCollisionWithMap(sprite, dir, solidObjects))
				{
					sprite.setPosition(oldPos);
				}
			}
		}
	}
	
}

void UpdateHero(Game & game) //position + collision + sprite
{
	Vector2f pos = game.hero->sprite.getPosition();
	Vector2f oldPos = pos;
	//TODO: CHECK BEAST MOVE SPEED (after changing func (07.01.16))
	if (game.hero->state == NORMAL || game.hero->state == BEAST)
	{
		UpdateSpritePosition(game.hero->sprite, game.hero->dir, STEP_HERO, game.solidObjects);
		game.hero->pos = game.hero->sprite.getPosition();
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
	UpdateHeroFrame(*game.hero, game.time);
	if ((game.inventoryList[game.hero->slotNo].quantity > 0 && game.inventoryList[game.hero->slotNo].current == 0 && (game.inventoryList[game.hero->slotNo].name == PISTOL || game.inventoryList[game.hero->slotNo].name == RIFLE)))
	{
		if (game.hero->isReloading == false)
		{
			game.hero->lastReloadTime = game.time;
			game.hero->isReloading = true;
		}
	}
}

void UpdateThrowings(vector<Throwing> & throwingList, Hero & hero, vector<Object> & solidObjects, float & time)
{
	for (vector<Throwing>::iterator it = throwingList.begin(); it != throwingList.end(); )
	{
		UpdateThrowingPosition(*it);
		UpdateThrowingFrame(*it);
		if (it->startTime + it->maxTime < time)
		{
			it = throwingList.erase(it);
		}
		else if (IsIntersectWithHero(it->sprite,hero.sprite))
		{
			//deal damage (action)
			hero.health -= THROWING_AXE_DAMAGE;
			it = throwingList.erase(it);
		}
		else if (IsCollisionWithMap(it->sprite,it->dir,solidObjects))
		{
			it = throwingList.erase(it);
		}
		else it++;
	}
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
	DrawEnemies(*game.window, game.zombieList);
	DrawHero(*game.window, game.hero->sprite);
	DrawMiniMap(*game.window, game.miniMap);
	DrawThrowings(*game.window, game.throwingList);
};


void CheckSpawnEnemyAndLoot(Game & game)
{
	if (game.lootList.size() < 5)
	{
		int itemNo = rand() % 5;
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
		case 4:
			item = SODA;
			break;
		}
		GenerateLoot(game.lootList, game.allObjects, 1, item, game.sprites.items);
	}
	if (game.zombieList.size() < 20)
	{
		SpawnEnemyRandomly(game.zombieList,game.allObjects, 1, game.time, game.sprites);
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
				//cout << game->hero->item.name << " item " << endl;
				
				CheckSpawnEnemyAndLoot(*game);
				ProcessEvents(*game, game->sprites);
				
				//cout << "ZOMBIE_STATE " << game->zombieList[0].state << " SPAWN " << game->zombieList[0].spawnTime<< " GAME " << game->time <<  endl;

				UpdateHero(*game);
				UpdateInventory(*game->hero, game->inventoryList, game->time);
				CheckEventNpc(game->npcList, *game->hero,game->miniMap);
				UpdateView(*game->window, game->hero->sprite, game->view);
				UpdateShots(*game, *game->hero, game->sprites.explosion);

				UpdateEnemies(game->zombieList, *game->hero, game->npcList, game->solidObjects,game->throwingList, game->sprites, game->time);
				UpdateThrowings(game->throwingList, *game->hero, game->solidObjects, game->time);
				CheckEnemyExplosion(game->explosionList, game->zombieList);

				CheckLoot(*game->hero, game->lootList, game->inventoryList, game->sprites.items);
				UpdateInventory(*game->hero, game->inventoryList, game->time);
				
				ComputeNpcFrame(game->npcList);

				UpdateMinimap(game->miniMap, game->npcList, game->hero->sprite, game->view);

				CheckGameOver(game->state, *game->hero);

				game->lvl.Draw(*game->window);
				DrawInventoryText(*game->window, game->inventoryList, *game->hero, game->view, game->text);
				
				//game->miniMap.npcDotList[0].rotate(1);
				Render(*game);
				DrawBar(*game->window, game->inventoryList, *game->hero, game->view, game->text, game->sprites);
				//cout << game->hero->sprite.getPosition().x << " " << game->hero->sprite.getPosition().y << endl;
				cout << "STATE " << game->hero->state << endl;
				break;
			}
			game->window->display();
		}
	}
};