#include "minimap.h"

void ComputeNpcDotDegree(Sprite & hero, Sprite & npc)
{

};

void InitializeMinimap(MiniMap & miniMap, vector<Npc> & npcList)
{
	miniMap.circle.setRadius(50);  //TODO: const
	miniMap.circle.setFillColor(sf::Color(150, 50, 250));
	// set a 10-pixel wide orange outline
	miniMap.circle.setOutlineThickness(10);
	miniMap.circle.setOutlineColor(sf::Color(250, 150, 100));
	miniMap.circle.setPosition(MINIMAP_START_POS);
	Vector2f miniMapCenter = { miniMap.circle.getPosition().x + MINIMAP_CIRCLE_RADIUS,  miniMap.circle.getPosition().y + MINIMAP_CIRCLE_RADIUS };

	for (Npc & npc : npcList)
	{
		CircleShape npcDot;
		npcDot.setRadius(5);
		npcDot.setFillColor(sf::Color(255, 0, 255));
		npcDot.setOrigin(miniMapCenter);
		//npcDot.setPosition()
		miniMap.npcDotList.push_back(npcDot);
	}
	miniMap.heroDot.setRadius(5);
	miniMap.heroDot.setFillColor((sf::Color(255, 0, 255)));
	//miniMap.heroDot.setOrigin(miniMapCenter);
	//miniMap.heroDot.setPosition(100, 0);
};

void UpdateMinimap(MiniMap & miniMap, vector<Npc> & npcList, Sprite & hero)
{
	int i = 0;
	for (Npc & npc : npcList)
	{
		Vector2f npcPos = npc.sprite.getPosition();
		Vector2f heroPos = hero.getPosition();

		float angle;
		angle = atan((npcPos.x - heroPos.x) / (npcPos.y - heroPos.y));
		angle = M_PI_2 - angle;
		angle = angle * 180 / M_PI;
		if (heroPos.y < npcPos.y)
			angle = angle;
		else angle = angle + 180;
		if (heroPos.x < npcPos.x)
			angle = angle;
		else angle = 180 - angle;

		Vector2f dist = {
			abs(heroPos.x - npcPos.x),
			abs(heroPos.y - npcPos.y)
		};

		float distance = sqrt(pow(dist.x, 2) + pow(dist.y, 2));

		//float di = sqrt(pow(dx,2) + pow(dy,2));  //distance
		if (distance > (miniMap.circle.getRadius() * 2.f))
			distance = miniMap.circle.getRadius();
		miniMap.npcDotList[i].setPosition(
			miniMap.heroDot.getPosition().x + distance,
			miniMap.heroDot.getPosition().y);

		miniMap.npcDotList[i].setRotation(angle);

		i++;
	}

};

void DrawMiniMap(RenderWindow & window, MiniMap & miniMap)
{
	window.draw(miniMap.circle);
	for (CircleShape & dot : miniMap.npcDotList)
	{
		window.draw(dot);
	}
	window.draw(miniMap.heroDot);
}