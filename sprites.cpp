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
	//enemies
	//common
	Texture texture_zombie;
	texture_zombie.loadFromFile("resources/images/zombie.png");
	sprites.texture_zombie = texture_zombie;
	sprites.zombie.setTexture(sprites.texture_zombie);
	//axe
	Texture texture_axe_enemy;
	texture_axe_enemy.loadFromFile("resources/images/axe_enemy.png");
	sprites.texture_axe_enemy = texture_axe_enemy;
	sprites.axe_enemy.setTexture(sprites.texture_axe_enemy);

	//throwing_axe
	Texture texture_throwing_axe;
	texture_throwing_axe.loadFromFile("resources/images/throwing_axe.png");
	sprites.texture_throwing_axe = texture_throwing_axe;
	sprites.throwing_axe.setTexture(sprites.texture_throwing_axe);

	//boss
	Texture texture_boss;
	texture_boss.loadFromFile("resources/images/boss.png");
	sprites.texture_boss = texture_boss;
	sprites.boss.setTexture(sprites.texture_boss);

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

	//Explosion
	Texture texture_explosion;
	texture_explosion.loadFromFile("resources/images/explosion.png");
	sprites.texture_explosion = texture_explosion;
	sprites.explosion.setTexture(sprites.texture_explosion);
	//npc Minimap Marker
	Texture texture_npc_marker;
	texture_npc_marker.loadFromFile("resources/images/marker_npc.png");
	sprites.texture_minimapMarkerNpc = texture_npc_marker;
	sprites.minimapMarkerNpc.setTexture(sprites.texture_minimapMarkerNpc);
	//hero Minimap Marker
	Texture texture_hero_marker;
	texture_hero_marker.loadFromFile("resources/images/marker_hero.png");
	sprites.texture_minimapMarkerHero = texture_hero_marker;
	sprites.minimapMarkerHero.setTexture(sprites.texture_minimapMarkerHero);

}

const Vector2f ComputeDistanceBetweenSprites(Sprite & s1, Sprite & s2)
{
	return{ abs(s1.getPosition().x - s2.getPosition().x), abs(s1.getPosition().y - s2.getPosition().y) };
}