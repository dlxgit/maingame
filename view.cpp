#include "view.h"


void InitializeView(View & view)
{
	//view.reset(sf::FloatRect(0, 0, 640, 512));
	view.reset(sf::FloatRect(0, 0, WINDOW_SIZE.x ,WINDOW_SIZE.y));
}

Vector2f GetInterfacePosition(View & view)
{
	Vector2f posView;
	posView.x = view.getCenter().x - 350;
	posView.y = view.getCenter().y - 265;
	return posView;
};

View UpdateView(RenderWindow & window, Sprite & hero, View & view, int & level)  //UpdateCameraPosition
{
	/*
	Vector2f temp = hero.getPosition();

	Vector2i WIN_SIZE;
	Vector2i TILE_SIZE;
	if (level == 0)
	{
		WIN_SIZE = WINDOW_SIZE;
		TILE_SIZE = TILEMAP_SIZE;
	}
	else
	{
		WIN_SIZE = { (LEVEL_BOSS_SIZE.x + 2) * STEP_TILE, (LEVEL_BOSS_SIZE.y + 2) * STEP_TILE };
		TILE_SIZE = { LEVEL_BOSS_SIZE.x + 2, LEVEL_BOSS_SIZE.y + 2 };
	}
	if (temp.x < float(WIN_SIZE.x) / 2.f) 
		temp.x = float(WIN_SIZE.x) / 2.f;
	else if (TILE_SIZE.x * STEP_TILE - float(WIN_SIZE.x) / 2.f < temp.x) 
		temp.x = (TILE_SIZE.x * STEP_TILE - float(WIN_SIZE.x) / 2.f);

	if (temp.y < float(WIN_SIZE.y) / 2.f) 
		temp.y = float(WIN_SIZE.y) / 2.f;
	else if (TILE_SIZE.y * STEP_TILE - float(WIN_SIZE.y) / 2.f < temp.y) 
		temp.y = (TILE_SIZE.y * STEP_TILE - float(WIN_SIZE.y) / 2.f);

	view.setCenter(temp);
	window.setView(view);
	return view;
	*/
	Vector2f temp = hero.getPosition();

	if (temp.x < float(WINDOW_SIZE.x) / 2.f)
		temp.x = float(WINDOW_SIZE.x) / 2.f;
	else if (TILEMAP_SIZE.x * STEP_TILE - float(WINDOW_SIZE.x) / 2.f < temp.x)
		temp.x = (TILEMAP_SIZE.x * STEP_TILE - float(WINDOW_SIZE.x) / 2.f);

	if (temp.y < float(WINDOW_SIZE.y) / 2.f)
		temp.y = float(WINDOW_SIZE.y) / 2.f;
	else if (TILEMAP_SIZE.y * STEP_TILE - float(WINDOW_SIZE.y) / 2.f < temp.y)
		temp.y = (TILEMAP_SIZE.y * STEP_TILE - float(WINDOW_SIZE.y) / 2.f);

	view.setCenter(temp);
	window.setView(view);
	return view;
};