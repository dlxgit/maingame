#pragma once
#include  "npc.h"

void InitializeNpc(vector<Npc> & npcList, Sprite & sprite_npc)
{
	Npc npc;
	npc.currentFrame = 0;
	npc.health = 20;
	npc.sprite = sprite_npc;
	npc.state = LIVING;

	npc.type = PHOTOGRAPHS;
	npc.pos = { 5 * STEP, 8 * STEP };
	npc.sprite.setPosition(npc.pos);
	npcList.push_back(npc);

	npc.type = BABY;
	npc.pos = { 50 * STEP, 10 * STEP };
	npc.sprite.setPosition(npc.pos);
	npcList.push_back(npc);

	npc.type = TEACHER;
	npc.pos = { 10 * STEP, 15 * STEP };
	npc.sprite.setPosition(npc.pos);
	npcList.push_back(npc);

	npc.type = DOG;
	npc.pos = { 53 * STEP, 15 * STEP };
	npc.sprite.setPosition(npc.pos);
	npcList.push_back(npc);

	npc.type = SOLDIER;
	npc.pos = { 28 * STEP, 23 * STEP };
	npc.sprite.setPosition(npc.pos);
	npcList.push_back(npc);

	npc.type = SEARCHER;
	npc.pos = { 55 * STEP, 4 * STEP };
	npc.sprite.setPosition(npc.pos);
	npcList.push_back(npc);

	npc.type = COOK;
	npc.pos = { 20 * STEP, 15 * STEP };
	npc.sprite.setPosition(npc.pos);
	npcList.push_back(npc);
	npc.type = GIRL;
	npc.pos = { 15 * STEP, 6 * STEP };
	npc.sprite.setPosition(npc.pos);
	npcList.push_back(npc);
};


void ComputeNpcFrame(vector<Npc> & npcList)
{
	for (vector<Npc>::iterator npc = npcList.begin(); npc != npcList.end(); npc++)
	{
		if (npc->state == LIVING)
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
		else if (npc->state == DEAD)
		{
			npc->sprite.setTextureRect(IntRect(4 + 45 * int(npc->currentFrame), 593, 53, 45));
			npc->currentFrame += 0.06;
		}
		npc->currentFrame += 0.02;
	}
};

void CheckEventNpc(vector<Npc> & npcList, Hero & hero)
{
	Vector2f itemCenter;
	Vector2f heroCenter;
	heroCenter.x = hero.pos.x + hero.sprite.getGlobalBounds().width / 2;
	heroCenter.y = hero.pos.y + hero.sprite.getGlobalBounds().height / 2;
	bool needDeleteNpc;
	for (vector<Npc>::iterator npc = npcList.begin(); npc != npcList.end();)
	{
		needDeleteNpc = false;

		if (npc->state == LIVING)  //if loot.item.center contains heroSprite  -> add new item in inventory
		{
			itemCenter.x = npc->sprite.getPosition().x + npc->sprite.getGlobalBounds().width / 2;
			itemCenter.y = npc->sprite.getPosition().y + npc->sprite.getGlobalBounds().height / 2;
			if ((abs(heroCenter.x - itemCenter.x) < 15) && (abs(heroCenter.y - itemCenter.y)) < 15)
			{
				//cout << "NEAR" << endl;
				npc->state = SURVIVED;
				hero.savedNeighbors += 1;
				npc->currentFrame = 0;
			}
			if (npc->health <= 0)
			{
				npc->state = KILLED;
			}
		}
		if (npc->state == DEAD)
		{
			if (npc->currentFrame > 8)
			{
				needDeleteNpc = true;
			}
		}
		if (npc->state == SURVIVED)
		{
			//if (npc->currentFrame > 5)
			{
				needDeleteNpc = true;
			}
		}

		//deleting Npc from List
		if (needDeleteNpc)
		{
			npc = npcList.erase(npc);
		}
		else
		{
			npc++;
		}
	}
};

