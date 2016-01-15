#pragma once
#include "declarations.h"



struct Throwing
{
	Texture texture;
	Sprite sprite;
	string name;
	Direction dir;
	Vector2f stepPerLoop;
	Vector2f startPos;
	float startTime;
	float maxTime;
	float cooldown;
	//float maxDistance;
	//float currentTime;
	bool isEnd;
	float currentFrame;

	Vector2f endPos;
};

void UpdateThrowingFrame(Throwing & throwing);

bool IsMilkOnGround(Throwing & throwing);

void UpdateThrowingPosition(Throwing & throwing);

void DrawThrowings(RenderWindow & window, vector<Throwing> & throwingList);

void DeleteThrowings(vector<Throwing> & throwingList);