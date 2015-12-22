#include "text.h"

void InitializeText(Font & font, Text & text)
{
	font.loadFromFile("resources/font/Arialbd.ttf");
	text.setString("");
	text.setFont(font);
	text.setCharacterSize(20);
	text.setColor(Color::White);
}