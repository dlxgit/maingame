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
	game.lvl = game.sprites.level0;
	game.level = 0;
	game.hero->maxNeighbors = MAX_NUMBER_OF_NEIGHBORS;
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

	InitializeSoundResouces(game.audio);
};

void DeleteGame(Game * game)
{
	delete(game->hero);
	//delete(game->window);
	DeleteExplosionList(game->explosionList);
	DeleteLoot(game->lootList);
	DeleteNpcList(game->npcList);
	DeleteShots(game->shotList);
	DeleteEnemyList(game->zombieList);
	DeleteInventory(game->inventoryList);
	DeleteMinimap(game->miniMap);
	DeleteThrowings(game->throwingList);
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

void BeginEvent(Game & game)  //process startGame events
{
	Vector2f posView = game.view.getCenter();

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
		game.audio.menu.stop();
		game.state = PLAY;
		if (game.level == 0)
		{
			game.audio.level0.play();
		}
	}
	CheckWindowClose(game);
};

void EndGameEvent(Game & game)
{
	Vector2f posView = game.view.getCenter();

	game.text.setString("GAME OVER");
	game.text.setPosition(posView.x - 40, posView.y - 100);
	game.window->draw(game.text);
	CheckWindowClose(game);

	game.audio.level0.stop();
	game.audio.beastAttackSound.stop();
	game.audio.lootSound.stop();
	game.audio.npcSurviveSound.stop();
	game.audio.npcDeathSound.stop();
	game.audio.shotSound.stop();
	game.audio.takenDamageSound.stop();
	game.audio.enemyExplosionSound.stop();
	game.audio.menu.play();
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
		DeleteThrowings(game.throwingList);
		//delete((&game));


		game.hero = new Hero();
		//InitiazlizeSprites(game.sprites);
		InitializeInventory(game.inventoryList, game.sprites);
		InitializeHero(*game.hero);
		if (game.level == 1)
		{
			game.hero->maxNeighbors = 0;
		}
		else game.hero->maxNeighbors = MAX_NUMBER_OF_NEIGHBORS;

		game.hero->item = game.inventoryList[0];
		game.hero->item.sprite.setTextureRect(IntRect(32, 0, 32, 32));
		//game.window = new RenderWindow(VideoMode(W_WIDTH, W_HEIGHT), "Shoot and Run");
		//InitializeView(game.view);
		//InitializeText(game.font, game.text);
		//game.lvl.LoadFromFile("level0.tmx");
		//game.solidObjects = game.lvl.GetObjects("solid");

		SpawnEnemy(game.zombieList, game.time, 100, 100, game.sprites, COMMON);
		//InitializeLoot(game.lootList, game.allObjects, game.sprites.items);
		if(game.level == 0)
			InitializeNpc(game.npcList, game.sprites.npc);
		InitializeMinimap(game.miniMap, game.npcList, game.sprites);
		game.time = 0;
		game.state = PLAY;

		game.audio.menu.stop();
		if (game.level == 1)
		{
			game.audio.level1.play();
		}
		else game.audio.level0.play();
		//game.hero->sprite.setPosition(game.view.getCenter());
	}
};

void LevelFinishEvent(Game & game)
{
	Vector2f posView = game.view.getCenter();

	game.text.setString("LEVEL HAS FINISHED!");
	game.text.setPosition(posView.x - 100, posView.y - 100);
	game.window->draw(game.text);
	CheckWindowClose(game);
	game.audio.level0.stop();
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
		DeleteThrowings(game.throwingList);

		game.hero = new Hero();
		InitializeHero(*game.hero);
		InitializeInventory(game.inventoryList, game.sprites);
		game.hero->item = game.inventoryList[0];
		game.time = 0;
		game.hero->sprite.setPosition(2 * 64 + 10, 8 * 64);
		game.hero->maxNeighbors = 0;
		game.level = 1;

		game.lvl = game.sprites.level1;
		game.allObjects = game.lvl.GetAllObjects();
		game.solidObjects = game.lvl.GetObjects("solid");

		//SpawnEnemy(game.zombieList, game.time, 100, 100, game.sprites, COMMON);
		//InitializeLoot(game.lootList, game.allObjects, game.sprites.items);
		//InitializeNpc(game.npcList, game.sprites.npc);
		InitializeMinimap(game.miniMap, game.npcList, game.sprites);
		
		InitializeBoss(game.boss, game.sprites, game.time);

		game.audio.level1.play();

		game.state = PLAY;
	}
};

void GameFinishEvent(Game & game)
{
	Vector2f posView = game.view.getCenter();

	game.text.setString("You won. Wanna try again?(Enter) \nQuit(Esc)");
	game.text.setPosition(posView.x - 100, posView.y - 100);
	game.window->draw(game.text);
	CheckWindowClose(game);
	if (Keyboard::isKeyPressed(Keyboard::Return))
	{
		delete((&game)->hero);
		DeleteExplosionList((&game)->explosionList);
		DeleteLoot((&game)->lootList);
		DeleteNpcList((&game)->npcList);
		DeleteShots((&game)->shotList);
		DeleteEnemyList((&game)->zombieList);
		DeleteInventory((&game)->inventoryList);
		DeleteMinimap(game.miniMap);
		DeleteThrowings(game.throwingList);


		game.hero = new Hero();
		InitiazlizeSprites(game.sprites);
		InitializeInventory(game.inventoryList, game.sprites);
		InitializeHero(*game.hero);
		game.hero->item = game.inventoryList[0];
		game.hero->item.sprite.setTextureRect(IntRect(32, 0, 32, 32));
		InitializeView(game.view);
		InitializeText(game.font, game.text);
		game.lvl = game.sprites.level0;
		game.level = 0;
		game.hero->maxNeighbors = MAX_NUMBER_OF_NEIGHBORS;
		game.allObjects = game.lvl.GetAllObjects();
		game.solidObjects = game.lvl.GetObjects("solid");

		SpawnEnemy(game.zombieList, game.time, 100, 100, game.sprites, AXE);
		InitializeLoot(game.lootList, game.allObjects, game.sprites.items);
		InitializeNpc(game.npcList, game.sprites.npc);
		game.time = 0;
		game.state = START_GAME;
		//game.hero->sprite.setPosition(game.view.getCenter());
		InitializeMinimap(game.miniMap, game.npcList, game.sprites);
		InitializeSoundResouces(game.audio);
	}
}

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

bool IsShotCollision(vector<Enemy> & zombieList, Hero & hero, vector<Object> & objects, Shot & shot)  //check if need delete shot from list
{
	//Vector2f shotCenter = GetSpriteCenter(shot.sprite);
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
		cout << enemy.state;
		if (enemy.sprite.getGlobalBounds().intersects(shot.sprite.getGlobalBounds()))
		{
			enemy.health -= DMG_ITEM[hero.item.name];
			hero.isSoundEnemyExplosion = true;
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
			shot->distance += int(STEP_SHOT);


			bool isDeleted = false;
			if (game.level == 1)
			{
				if (game.boss.sprite.getGlobalBounds().intersects(shot->sprite.getGlobalBounds()))
				{
					game.boss.health -= DMG_ITEM[hero.item.name];
					shot = game.shotList.erase(shot);
					isDeleted = true;
				}
			}
			if (isDeleted == false &&(IsShotCollision(game.zombieList,hero,game.solidObjects, *shot)) ) //shot delete
			{
				shot = game.shotList.erase(shot);
			}
			else if (isDeleted == false)
				shot++;
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

bool IsCollisionWithMap(FloatRect & spriteRect, Direction & dir, vector<Object> &objects)
{
	for (size_t i = 0; i < objects.size(); ++i)
	{
		if (spriteRect.intersects(objects[i].rect))
		{
				return true;
		}
	}
	return false;
};

void UpdateEnemies(Game & game)
{
	Vector2f heroPos = game.hero->sprite.getPosition();
	int index = 0;
	for (vector<Enemy>::iterator zombie = game.zombieList.begin(); zombie != game.zombieList.end();)
	{
		//float di = sqrt(pow(dx,2) + pow(dy,2));  //distance
		if (zombie->state == ACTIVE)
		{
			CheckEnemyFollow(*zombie, *game.hero);
			if (zombie->follow)
			{
				ComputeEnemyDirection(*zombie,heroPos);  //direction relatively to hero
				CheckNpcDeath(game.npcList, *zombie);
			}
			else
			{
				CheckEnemyDir(*zombie, game.time);
			}

			//CheckCollisionWithMap(zombie->sprite,zombie->dirLast,STEP_ZOMBIE_ACTIVE, objects);
			UpdateEnemyAttack(*game.hero, *zombie, game.time);

			if (game.hero->state == BEAST)
			{
				if (IsEnemyNearHero(*game.hero, *zombie))
				{
					CheckHeroBeastDamage(*game.hero, *zombie, game.time);
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
						UpdateSpritePosition(zombie->sprite, zombie->dir, STEP_ZOMBIE_ACTIVE, game.solidObjects);
						//cout << "1     1" << endl;
					}
					else
					{
						UpdateSpritePosition(zombie->sprite, zombie->dir, STEP_ZOMBIE, game.solidObjects);
						//cout << "2      2" << endl;
					}
					if (IsIntersectWithOtherEnemy(game.zombieList, index))
					{
						zombie->sprite.setPosition(oldPosition);
					}
					if (zombie->type == AXE)
					{
						if (IsIntersectWithHero(zombie->sprite, game.hero->sprite))
						{
							//zombie->pos = oldPosition;
							zombie->sprite.setPosition(oldPosition);
							if (game.time > zombie->attack_time + zombie->attackCooldown)
							{
								zombie->currentFrame = 0;
								zombie->isAttack = true;
								zombie->attack_time = game.time;
								game.hero->health -= int(ENEMY_AXE_ATTACK_DAMAGE * game.hero->damageResistance);
							}
						}
						if (zombie->last_action_time + ENEMY_AXE_ACTION_COOLDOWN < game.time)
						{
							Vector2f spriteDistance = ComputeDistanceBetweenSprites(zombie->sprite, game.hero->sprite);
							zombie->last_action_time = game.time;

							game.throwingList.push_back(CreateThrowing(zombie->sprite, game.hero->sprite, "axe", game.sprites.throwing_axe, game.time));
							
						}
					}
					zombie->pos = zombie->sprite.getPosition();
				}
			}
		}

		UpdateEnemyFrame(*zombie, game.time);
		//UpdateZombiePosition(i);  TODO: make it for all zombies, not jsut for following ones
		//zombie->isAttack = false;
		if (zombie->dir != zombie->dirLast)
		{
			//zombie->currentFrame = 0;
			zombie->dirLast = zombie->dir;
		}
		if (zombie->state == EXPLODED)  //deleting
		{
			zombie = game.zombieList.erase(zombie);
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
				hero.isSoundNpcSurvive = true;
				npc->state = SURVIVED;
				hero.savedNeighbors += 1;
				needDeleteNpc = true;
				isAnyNpcChanged = true;
			}
		}
		if (npc->health <= 0)
		{
			hero.isSoundNpcDeath = true;
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
	
	FloatRect spriteRect = GetSpriteRect(sprite);

	if (IsCollisionWithMap(spriteRect, dir, solidObjects))
	{
		sprite.setPosition(oldPos);
		if (pos.x != oldPos.x && pos.y != oldPos.y)
		{
			sprite.setPosition(oldPos.x, pos.y);
			if (IsCollisionWithMap(spriteRect, dir, solidObjects))
			{
				sprite.setPosition(pos.x, oldPos.y);
				if (IsCollisionWithMap(spriteRect, dir, solidObjects))
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
			game.hero->damageResistance = 1;
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
	if (game.hero->health < 0)
	{
		game.hero->health = 0;
	}
}

void UpdateBossPosition(Boss & boss, vector<Object> & solidObjects)
{
	Vector2f oldPos = boss.sprite.getPosition();
	if (boss.eventType == CHARGE)
	{
		boss.speed = STEP_BOSS_CHARGE;
	}
	else if (boss.eventType == MOVE_OUT)
	{
		boss.speed = STEP_BOSS;
	}
	else if (boss.eventType == MOVE_TO)
	{
		boss.speed = STEP_BOSS_FOLLOW;
	}

	Vector2f pos = ComputeSpriteNewPosition(boss.sprite, boss.dir, boss.speed);

	boss.sprite.setPosition(pos);

	FloatRect spriteRect = GetBossCollisionRect(boss.sprite);

	if (IsCollisionWithMap(spriteRect, boss.dir, solidObjects))
	{
		boss.sprite.setPosition(oldPos);
		if (pos.x != oldPos.x && pos.y != oldPos.y)
		{
			boss.sprite.setPosition(oldPos.x, pos.y);
			if (IsCollisionWithMap(spriteRect, boss.dir, solidObjects))
			{
				boss.sprite.setPosition(pos.x, oldPos.y);
				if (IsCollisionWithMap(spriteRect, boss.dir, solidObjects))
				{
					boss.sprite.setPosition(oldPos);
				}
			}
		}
	}
}


void UpdateThrowings(vector<Throwing> & throwingList, Hero & hero, vector<Object> & solidObjects, float & time)
{
	for (vector<Throwing>::iterator it = throwingList.begin(); it != throwingList.end(); )
	{
		if (it->name != "milk" || (it->name == "milk" && it->currentFrame < 1))
		{
			UpdateThrowingPosition(*it);
		}
		UpdateThrowingFrame(*it);

		if (it->name == "milk")
		{
			if (IsMilkOnGround(*it))
			{

				if (it->currentFrame < 1)
				{
					it->currentFrame = 1;
				}

			}
		}

		if (it->startTime + it->maxTime < time)
		{
			it = throwingList.erase(it);
		}
		else if (IsIntersectWithHero(it->sprite,hero.sprite))
		{
			//deal damage (action)
			if(it->name != "milk")
				hero.health -= int(THROWING_AXE_DAMAGE * hero.damageResistance);
			else hero.health -= int(BOSS_MILK_DAMAGE * hero.damageResistance);
			it = throwingList.erase(it);
			hero.isSoundTakeDamage = true;
		}
		else if (IsCollisionWithMap(GetSpriteRect(it->sprite),it->dir,solidObjects))
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

	if (game.level == 1)
	{
		DrawBoss(*game.window, game.boss.sprite);
		DrawBossBar(*game.window, game.boss, game.view.getCenter());
	}

	DrawMiniMap(*game.window, game.miniMap);
	DrawThrowings(*game.window, game.throwingList);
};


void CheckSpawnEnemyAndLoot(Game & game)
{
	if ((game.level == 0 && game.lootList.size() < 5) || (game.level == 1 && (game.lootList.size() < LEVEL_BOSS_MAX_LOOT_QUANTITY)))
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
			item = GRENADE;
			break;
		}
		if (game.level == 0)
		{
			GenerateLoot(game.lootList, game.allObjects, { WIDTH_MAP, HEIGHT_MAP }, 1, item, game.sprites.items);
		}
		else 
			GenerateLoot(game.lootList, game.allObjects, { LEVEL_BOSS_SIZE.x - 1, LEVEL_BOSS_SIZE.y - 1 }, 1, item, game.sprites.items);
	}
	if (game.level == 0 && game.zombieList.size() < 20)
	{
		SpawnEnemyRandomly(game.zombieList,game.allObjects, 1, game.time, game.sprites);
	}
}

void CheckGameOver(Game & game)
{
	if (game.level == 0)
	{
		if (game.hero->health <= 0)
		{
			game.state = END_GAME;
		}
		if ((game.hero->savedNeighbors >= MAX_NUMBER_OF_NEIGHBORS) || (game.npcList.size() == 0 && game.hero->savedNeighbors >= 6))
		{
			game.state = LEVEL_FINISH;
		}
	}
}

void DrawBar(RenderWindow & window, vector<Inventory> & inventoryList, Hero & hero, View & view, Text & text, Sprites & sprites)
{
	Vector2f posView = GetInterfacePosition(view);

	sprites.bar.setTextureRect(IntRect(0, 0, 170, 35));
	sprites.bar.setPosition(posView);

	sprites.health.setTextureRect(IntRect(1, 0, 146 * int(float(hero.health) / 100), 29));
	sprites.health.setPosition(posView.x + 10.f, posView.y + 1.f);

	inventoryList[hero.slotNo].sprite.setPosition(posView.x + 5, posView.y + 40);

	window.draw(inventoryList[hero.slotNo].sprite);
	window.draw(sprites.bar);
	if(hero.health > 0)
		window.draw(sprites.health);
}


void UpdateAudio(Game & game)
{
	if (game.hero->isSoundBeastAtttack)
	{
		game.audio.beastAttackSound.play();
		game.hero->isSoundBeastAtttack = false;
	}
	if (game.hero->isSoundLoot)
	{
		game.audio.lootSound.play();
		game.hero->isSoundLoot = false;
	}
	if (game.hero->isSoundNpcSurvive)
	{
		game.audio.npcSurviveSound.play();
		game.hero->isSoundNpcSurvive = false;
	}
	if (game.hero->isSoundNpcDeath)
	{
		game.audio.npcDeathSound.play();
		game.hero->isSoundNpcDeath = false;
	}
	if (game.hero->isSoundShoot)
	{
		game.audio.shotSound.play();
		game.hero->isSoundShoot = false;
	}
	if (game.hero->isSoundTakeDamage)
	{
		game.audio.takenDamageSound.play();
		game.hero->isSoundTakeDamage = false;
	}
	if (game.hero->isSoundEnemyExplosion)
	{
		game.audio.enemyExplosionSound.play();
		game.hero->isSoundEnemyExplosion = false;
	}
	//if(game.)
}

void UpdateBossEvent(Boss & boss, Hero & hero, float & time)
{
	if (boss.eventType == MOVE_OUT)
	{
		cout << "MOVE_OUT " << endl;
		if (IsBossNearMapCenter(boss.sprite))//(boss.eventstartTime + 7.f < time)
		{
			cout << "B" << endl;
			boss.eventType = STOMP_FOR_ZOMBIES;
			if (boss.isCommonZombie == true)
			{
				boss.isCommonZombie = false;
			}
			else boss.isCommonZombie = true;
			boss.spawnedEnemies = 0;
			boss.eventstartTime = time;
		}
	}
	else if (boss.eventType == MOVE_TO)
	{
		cout << "MOVE_TO " << endl;
		//cout << "A";

		if (boss.shootStartTime == -1)
		{
			if (IsBossAbleToShoot(hero.sprite, boss.sprite))
			{
				boss.eventType = SHOOT;
				boss.eventstartTime = time;
				boss.shootStartTime = time;
				return;
			}
		}
		else if (boss.shootStartTime + BOSS_FOLLOW_FOR_SHOOT_TIME > time)
		{
			if (IsBossAbleToShoot(hero.sprite, boss.sprite))
			{
				boss.eventType = SHOOT;
			}
			return;
		}
		else if (boss.eventstartTime + 5.f < time)
		{
			//cout << "B " << time << endl;
			boss.eventType = MOVE_OUT;
			boss.wasCharged = false;
			boss.eventstartTime = time;
			return;
		}
		if (boss.wasCharged == false && boss.eventstartTime + 2.f < time)
		{
			boss.eventType = CHARGE;
			boss.eventstartTime = time;

			boss.wasCharged = true;
		}
	}
	else if (boss.eventType == STOMP_FOR_ZOMBIES)
	{
		cout << "STOMP_FOR_ZOMBIES" << endl;
		if (boss.spawnedEnemies == 4) //(boss.eventstartTime + 2.5f < time)
		{
			boss.shootStartTime = -1;
			boss.eventType = MOVE_TO;
			boss.eventstartTime = time;
		}
	}
	else if (boss.eventType == SHOOT)
	{
		cout << "SHOOT" << endl;
		if (boss.eventstartTime + BOSS_FOLLOW_FOR_SHOOT_TIME < time)
		{
			boss.eventType = MOVE_OUT;
			boss.eventstartTime = time;
			return;
		}
		if (IsBossAbleToShoot(hero.sprite, boss.sprite) == false)
		{
			boss.eventType = MOVE_TO;
		}
	}
	else if (boss.eventType == CHARGE)
	{
		cout << "CHARGE" << endl;
		if (IsReachedHero(boss.sprite, hero.sprite))
		{
			hero.lastSmashTime = time;
			hero.isSmashed = true;
			hero.state = SMASHED;
			boss.lastShootTime = time + 0.3f;
			hero.isSoundTakeDamage = true;
			hero.health -= int(BOSS_SMASH_DAMAGE);

			boss.eventType = SHOOT;
			boss.shootStartTime = time;
			boss.eventstartTime = time;
			return;
		}
		else if (boss.eventstartTime + 2.f < time)
		{
			//boss.wasCharged = false;
			boss.eventType = MOVE_TO;
			boss.eventstartTime = time;
		}
	}
}

void UpdateBoss(Game & game)
{
	if (game.level == 1)
	{
		FloatRect collideRect = GetBossCollisionRect(game.boss.sprite);

		BossEvent bossEvent = game.boss.eventType;
		UpdateBossEvent(game.boss, *game.hero, game.time);
		if (bossEvent == STOMP_FOR_ZOMBIES)
		{
			if (game.boss.eventstartTime + 1 + game.boss.spawnedEnemies * BOSS_SPAWN_ENEMY_INTERVAL < game.time)
			{
				EnemyType type;
				if (game.boss.isCommonZombie)
				{
					type = COMMON;
				}
				else type = AXE;

				SpawnEnemy(game.zombieList, game.time, LEVEL1_ZOMBIE_SPAWN_SPOTS[game.boss.spawnedEnemies].x, LEVEL1_ZOMBIE_SPAWN_SPOTS[game.boss.spawnedEnemies].y, game.sprites, type);
				game.boss.spawnedEnemies += 1;
			}

		}
		ComputeBossDirection(game.boss, collideRect, game.hero->sprite);
		UpdateBossPosition(game.boss, game.solidObjects);

		//UpdateSpritePosition(boss.sprite, boss.dir, STEP_BOSS, solidObjects);
		UpdateBossFrame(game.boss);

		if (game.hero->state == BEAST)
		{
			if (IsFootRectIntersectWithHero(game.hero->sprite, GetBossCollisionRect(game.boss.sprite)))
			{
				if (game.hero->isBeastAttack && ((game.time - game.hero->lastAttackTime) > HERO_BEAST_ATTACK_TIME))
				{
					game.boss.health -= HERO_BEAST_DAMAGE;
					game.hero->lastAttackTime = game.time;
				}
			}
		}
		if (game.boss.eventType == SHOOT)
		{
			if (game.hero->isSmashed == true)
			{
				//boss.lastShootTime = time;
			}
			else if (IsReadyToShoot(game.boss, game.time))
			{
				game.boss.lastShootTime = game.time;
				Sprite milk = game.boss.sprite;
				milk.setTextureRect(IntRect(392, 404, 42, 19));
				Throwing thr = CreateThrowing(game.boss.sprite, game.hero->sprite, "milk", milk, game.time);

				if (game.boss.lastSide == LEFT)
				{
					thr.sprite.setPosition(game.boss.sprite.getPosition().x - 5.f, game.boss.sprite.getPosition().y + 75.f);
				}
				else thr.sprite.setPosition(game.boss.sprite.getPosition().x + 78.f, game.boss.sprite.getPosition().y + 71.f);

				game.throwingList.push_back(thr);
				game.boss.isAttack = false;
			}
		}

		if (game.hero->health <= 0)
		{
			game.state = END_GAME;
		}
		else if (game.boss.health <= 0)
		{
			game.state = FINISH;
		}
	}
}


void StartGame(Game * game)
{
	Clock clock;
	Clock gameSpeedClock;
	float gameSpeedTime = 0;
	game->audio.menu.play();
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
				BeginEvent(*game);
				break;
			case END_GAME:
				EndGameEvent(*game);
				break;
			case LEVEL_FINISH:
				LevelFinishEvent(*game);
				break;
			case FINISH:
				GameFinishEvent(*game);
				break;
			case PLAY:
				//TODO: spawn zombie at definite time (and change SpawnZombie func (spawn only near hero))			
							

				CheckSpawnEnemyAndLoot(*game);

				ProcessEvents(*game, game->sprites);

				UpdateHero(*game);
				UpdateInventory(*game->hero, game->inventoryList, game->time);
				CheckEventNpc(game->npcList, *game->hero,game->miniMap);
				UpdateView(*game->window, game->hero->sprite, game->view,game->level);
				UpdateShots(*game, *game->hero, game->sprites.explosion);

				UpdateEnemies(*game);
				UpdateThrowings(game->throwingList, *game->hero, game->solidObjects, game->time);


				CheckBossExplosion(game->boss, game->explosionList, game->level);
				CheckEnemyExplosion(game->explosionList, game->zombieList);

				CheckLoot(*game->hero, game->lootList, game->inventoryList, game->sprites.items);
				UpdateInventory(*game->hero, game->inventoryList, game->time);
				
				ComputeNpcFrame(game->npcList);

				UpdateMinimap(game->miniMap, game->npcList, game->hero->sprite, game->view);


				UpdateBoss(*game);

				CheckGameOver(*game);

				game->lvl.Draw(*game->window);
				
				//game->miniMap.npcDotList[0].rotate(1);


				Render(*game);

				DrawBar(*game->window, game->inventoryList, *game->hero, game->view, game->text, game->sprites);
				DrawText(*game->window, game->inventoryList, *game->hero, game->view, game->text);
				//cout << game->hero->sprite.getPosition().x << " " << game->hero->sprite.getPosition().y << endl;
				

				UpdateAudio(*game);
				//cout << "LEVEL -  " << game->level << endl;

				break;
			}
			game->window->display();
		}
	}
};