#include <SFML/Graphics.hpp>
#include <iostream>

#include <cmath>
#include <vector>
#include <cstdlib>

#include "game.h"

/*
зверь не может пройти через одинарный проем в заборе(размер спрайта), - нужно убрать одинарные и заменить двойными. 
*/

using namespace sf;
using namespace std;

void main()
{
	Game * game = new Game();
	InitializeGame(*game);

	StartGame(game);
	DeleteGame(game);
}