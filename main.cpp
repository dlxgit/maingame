#include <SFML/Graphics.hpp>
#include <iostream>

#include <cmath>
#include <vector>
#include <cstdlib>

#include "game.h"

using namespace sf;
using namespace std;

void main()
{
	Game * game = new Game();
	InitializeGame(*game);

	StartGame(game);
	DestroyGame(game);
}