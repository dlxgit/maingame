#pragma once
#include <vector>
#include "SFML\Graphics.hpp"
#include "declarations.h"
#include "hero.h"

#include "map.h"

using namespace sf;
using namespace std;



struct Inventory
{
	NameItem name;
	int quantity;
	int current;
	Texture texture;
	Sprite sprite;
};

struct Grenade
{
	Vector2f startPos;
	Vector2f distance;
	float startTime;
	bool isExploded;
};

struct Explosion
{
	Vector2f pos;
	float currentFrame;
	Sprite sprite;
};

Vector2f GetInterfacePosition(View & view)
{
	Vector2f posView;
	posView.x = view.getCenter().x - 600;
	posView.y = view.getCenter().y - 450;
	return posView;
};