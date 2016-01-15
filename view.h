#pragma once
#include "declarations.h"

void InitializeView(View & view);

Vector2f GetInterfacePosition(View & view);

View UpdateView(RenderWindow & window, Sprite & hero, View & view, int & level);