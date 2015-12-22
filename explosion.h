#pragma once
#include "declarations.h"

struct Explosion
{
	Vector2f pos;
	float currentFrame;
	Sprite sprite;
};

void DeleteExplosionList(vector<Explosion> & explosions);