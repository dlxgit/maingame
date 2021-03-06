#pragma once
#include "declarations.h"
#include <vector>
#include <iostream>
#include "explosion.h"


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

void AddNewShot(std::vector<Shot> & shotList, Direction & dirLast, Vector2f & pos, float & time, Sprite & sprite_shot, Sprite & sprite_grenade, ShotType shotType);

void DeleteShots(vector<Shot> & shot);