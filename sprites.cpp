#include "sprites.h"

const Vector2f GetSpriteCenter(const Sprite & sprite)
{
	return { sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f,sprite.getPosition().y + sprite.getGlobalBounds().height / 2.f };
}

const FloatRect GetSpriteRect(const Sprite & sprite)
{
	//TODO: check if works good (after change)
	/*
	const Vector2f pos = sprite.getPosition();	
	const Vector2f size = { sprite.getGlobalBounds().width, sprite.getGlobalBounds().height };
	*/
	return FloatRect(sprite.getPosition(), { sprite.getGlobalBounds().width, sprite.getGlobalBounds().height });
}

void InitiazlizeSprites(Sprites & sprites)
{
	//map
	Texture texture_map;
	texture_map.loadFromFile("resources/images/map.png");
	sprites.texture_map = texture_map;
	sprites.map.setTexture(sprites.texture_map);
	//zombie
	Texture texture_zombie;
	texture_zombie.loadFromFile("resources/images/zombie.png");
	sprites.texture_zombie = texture_zombie;
	sprites.zombie.setTexture(sprites.texture_zombie);
	//shot
	Texture texture_shot;
	texture_shot.loadFromFile("resources/images/shots.png");
	sprites.texture_shot = texture_shot;
	sprites.shot.setTexture(sprites.texture_shot);
	//items
	Texture texture_items;
	texture_items.loadFromFile("resources/images/items.png");
	sprites.texture_items = texture_items;
	sprites.items.setTexture(sprites.texture_items);
	//healthbar
	Texture texture_bar;
	texture_bar.loadFromFile("resources/images/bar.png");
	sprites.texture_bar = texture_bar;
	sprites.bar.setTexture(sprites.texture_bar);
	//health
	Texture texture_health;
	texture_health.loadFromFile("resources/images/health.png");
	sprites.texture_health = texture_health;
	sprites.health.setTexture(sprites.texture_health);
	//Npc
	Texture texture_npc;
	texture_npc.loadFromFile("resources/images/npcs.png");
	sprites.texture_npc = texture_npc;
	sprites.npc.setTexture(sprites.texture_npc);
	//Grenade
	Texture texture_grenade;
	texture_grenade.loadFromFile("resources/images/grenade.png");
	sprites.texture_grenade = texture_grenade;
	sprites.grenade.setTexture(sprites.texture_grenade);
	//Door
	Texture texture_door;
	texture_door.loadFromFile("resources/images/door1.png");
	sprites.texture_door = texture_door;
	sprites.door.setTexture(sprites.texture_door);
	//Explosion
	Texture texture_explosion;
	texture_explosion.loadFromFile("resources/images/explosion.png");
	sprites.texture_explosion = texture_explosion;
	sprites.explosion.setTexture(sprites.texture_explosion);
	//Dot
	Texture texture_dot;
	texture_dot.loadFromFile("resources/images/dot.png");
	sprites.texture_dot = texture_dot;
	sprites.dot.setTexture(sprites.texture_dot);
}