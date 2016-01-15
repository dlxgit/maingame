#pragma once
#include "shot.h"

void AddNewShot(std::vector<Shot> & shotList, Direction & dirLast, Vector2f & pos, float & time, Sprite & sprite_shot, Sprite & sprite_grenade, ShotType  shotType) //adding new shot in list
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
		shot.sprite.setPosition(pos);
		switch (shot.dir)
		{
		case UP:
			shot.sprite.setTextureRect(IntRect(2, 5, 3, 7));
			break;
		case RIGHT:
			shot.sprite.setTextureRect(IntRect(0, 0, 7, 3));
			shot.sprite.setPosition(pos.x, pos.y + 10.f);
			break;
		case DOWN:
			shot.sprite.setTextureRect(IntRect(10, 5, 3, 7));
			break;
		case LEFT:
			shot.sprite.setTextureRect(IntRect(8, 0, 7, 3));
			shot.sprite.setPosition(pos.x, pos.y + 10.f);
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
		shot.sprite.setPosition(pos);
	}
	shot.pos = shot.sprite.getPosition();

	//cout << "SHOTEEEEEE " << pos.x << " " << pos.y  << endl;
	shotList.push_back(shot);
};

void DeleteShots(vector<Shot> & shots)
{
	shots.clear();
}