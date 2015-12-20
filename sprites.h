#pragma once
#include "SFML\Graphics.hpp"

using namespace sf;

struct Sprites
{
	Sprite hero;
	Sprite zombie;
	Sprite items;
	Sprite npc;
	Sprite shot;
	Sprite grenade;
	Sprite map;
	Sprite health;
	Sprite bar;
	Sprite explosion;
	Sprite house;
	Sprite door;
	
	Texture texture_hero;
	Texture texture_zombie;
	Texture texture_items;
	Texture texture_npc;
	Texture texture_shot;
	Texture texture_grenade;
	Texture texture_map;
	Texture texture_health;
	Texture texture_bar;
	Texture texture_explosion;
	Texture texture_house;
	Texture texture_door;
};

const Vector2f GetSpriteCenter(const Sprite & sprite);

void InitiazlizeSprites(Sprites & sprites);
