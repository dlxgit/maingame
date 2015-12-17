#pragma once
#include <SFML/Graphics.hpp>
#include "declarations.h"
//#include "hero.h"
#include <vector>

#include <iostream>

using namespace sf;


using namespace std;

struct Shot
{
	Vector2f pos;
	int distance;
	Direction dir;
	Sprite sprite;
	ShotType type;
	bool isExploded;
	float startTime;
	float currentFrame;
};

void AddNewShot(std::vector<Shot> & shotList,Direction & dirLast, Vector2f & pos,float & time, Sprite & sprite_shot, Sprite & sprite_grenade, ShotType  shotType) //adding new shot in list
{
	Shot shot;
	switch (dirLast)
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

	Texture texture;

	if (shotType == BULLET)
	{

		//shot.pos = hero.sprite.getPosition();
		shot.distance = 0;
		shot.sprite = sprite_shot;
		shot.type = shotType;

		switch(shot.dir)
		{
		case UP:
			shot.sprite.setTextureRect(IntRect(3, 11, 7, 13));
			break;
		case RIGHT:
			shot.sprite.setTextureRect(IntRect(0, 0, 13, 7));
			break;
		case DOWN:
			shot.sprite.setTextureRect(IntRect(19, 11, 7, 13));
			break;
		case LEFT:
			shot.sprite.setTextureRect(IntRect(16, 0, 13, 7));
			break;
		}
	}
	else
	{
		//shot.pos = hero.sprite.getPosition();
		shot.distance = 0;
		shot.sprite = sprite_grenade;
		shot.type = shotType;
		shot.startTime = time;
		Texture texture;
		//AddNewShot(hero, sprite_shot, game);
		//shot.sprite.setPosition(hero.sprite.getPosition());
		shot.currentFrame = 0;
		shot.isExploded = false;
	}
	shot.sprite.setPosition(pos);
	shot.pos = pos;
	
	//cout << "SHOTEEEEEE " << pos.x << " " << pos.y  << endl;
	shotList.push_back(shot);
};