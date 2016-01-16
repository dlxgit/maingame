#include "throwing.h"

//void Compute


void UpdateThrowingFrame(Throwing & throwing)
{
	if (throwing.name != "milk")
	{
		throwing.sprite.setTextureRect(IntRect(0 + 15 * int(throwing.currentFrame), 0, 16, 16));
		throwing.currentFrame += 0.6f;
		if (throwing.currentFrame > 4)
		{
			throwing.currentFrame = 0;
		}
	}
}

bool IsMilkOnGround(Throwing & throwing)
{
	Vector2f throwPos = throwing.sprite.getPosition();
	if (abs(throwPos.x - throwing.endPos.x) < 10 && abs(throwPos.y - throwing.endPos.y) < 10)
	{
		return true;
	}
	return false;
}

void UpdateThrowingPosition(Throwing & throwing)
{
	Vector2f pos = throwing.sprite.getPosition();
	throwing.sprite.setPosition(pos.x + throwing.stepPerLoop.x, pos.y + throwing.stepPerLoop.y);
}

void DrawThrowings(RenderWindow & window, vector<Throwing> & throwingList)
{
	for (Throwing & throwing : throwingList)
	{
		window.draw(throwing.sprite);
	}
}



void DeleteThrowings(vector<Throwing> & throwingList)
{
	throwingList.clear();
}