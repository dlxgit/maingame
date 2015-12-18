#include "sprites.h"

void InitiazlizeSprites(Sprites & sprites)
{
	//map
	Texture texture_map;
	texture_map.loadFromFile("images/map.png");
	sprites.texture_map = texture_map;
	sprites.map.setTexture(sprites.texture_map);
	//zombie
	Texture texture_zombie;
	texture_zombie.loadFromFile("images/zombie.png");
	sprites.texture_zombie = texture_zombie;
	sprites.zombie.setTexture(sprites.texture_zombie);

	//shot
	Texture texture_shot;
	texture_shot.loadFromFile("images/shots.png");
	sprites.texture_shot = texture_shot;
	sprites.shot.setTexture(sprites.texture_shot);
	//items
	Texture texture_items;
	texture_items.loadFromFile("images/items.png");
	sprites.texture_items = texture_items;
	sprites.items.setTexture(sprites.texture_items);
	//healthbar
	Texture texture_bar;
	texture_bar.loadFromFile("images/bar.png");
	sprites.texture_bar = texture_bar;
	sprites.bar.setTexture(sprites.texture_bar);
	//health
	Texture texture_health;
	texture_health.loadFromFile("images/health.png");
	sprites.texture_health = texture_health;
	sprites.health.setTexture(sprites.texture_health);
	//Npc
	Texture texture_npc;
	texture_npc.loadFromFile("images/npcs.png");
	sprites.texture_npc = texture_npc;
	sprites.npc.setTexture(sprites.texture_npc);
	//Grenade
	Texture texture_grenade;
	texture_grenade.loadFromFile("images/grenade.png");
	sprites.texture_grenade = texture_grenade;
	sprites.grenade.setTexture(sprites.texture_grenade);
	//Door
	Texture texture_door;
	texture_door.loadFromFile("images/door1.png");
	sprites.texture_door = texture_door;
	sprites.door.setTexture(sprites.texture_door);
	//Explosion
	Texture texture_explosion;
	texture_explosion.loadFromFile("images/explosion.png");
	sprites.texture_explosion = texture_explosion;
	sprites.explosion.setTexture(sprites.texture_explosion);
}