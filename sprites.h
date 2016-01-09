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
	Sprite minimapMarkerNpc;
	Sprite minimapMarkerHero;
	
	Sprite axe_enemy;
	Sprite mummy_enemy;

	Sprite throwing_axe;

	Sprite boss;

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

	Texture texture_minimapMarkerNpc;
	Texture texture_minimapMarkerHero;

	Texture texture_axe_enemy;
	Texture texture_mummy_enemy;

	Texture texture_throwing_axe;

	Texture texture_boss;
};


const FloatRect GetSpriteRect(const Sprite & sprite);

const Vector2f GetSpriteCenter(const Sprite & sprite);

void InitiazlizeSprites(Sprites & sprites);

const Vector2f ComputeDistanceBetweenSprites(Sprite & s1, Sprite & s2);