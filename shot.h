#pragma once
#include <SFML/Graphics.hpp>
#include "hero.h"

using namespace sf;

enum ShotType
{
	BULLET,
	USED_GRENADE
};

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