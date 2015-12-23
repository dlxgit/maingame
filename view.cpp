#include "view.h"


void InitializeView(View & view)
{
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
	//TODO: no magic numbers
	Vector2f temp = hero.getPosition();

	if (temp.x < 640) temp.x = 640;
	else if (temp.x > 3008) temp.x = 3008;

	if (temp.y < 512) temp.y = 512;
	else if (temp.y > 928) temp.y = 928;

	view.setCenter(temp);
	window.setView(view);
	return view;
};