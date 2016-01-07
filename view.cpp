#include "view.h"


void InitializeView(View & view)
{
	//view.reset(sf::FloatRect(0, 0, 640, 512));
	view.reset(sf::FloatRect(0, 0, 1280, 1024));
}

Vector2f GetInterfacePosition(View & view)
{
	Vector2f posView;
	posView.x = view.getCenter().x - 600;
	posView.y = view.getCenter().y - 450;
	return posView;
};

View UpdateView(RenderWindow & window, Sprite & hero, View & view)  //UpdateCameraPosition
{
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