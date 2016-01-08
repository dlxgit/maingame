#include "minimap.h"

void ComputeNpcDotDegree(Sprite & hero, Sprite & npc)
{

};

void InitializeMinimap(MiniMap & miniMap, vector<Npc> & npcList, Sprites & sprites)
{
	miniMap.circle.setRadius(50);  //TODO: const
	miniMap.circle.setFillColor(sf::Color(64, 64, 64));
	// set a 10-pixel wide orange outline
	miniMap.circle.setOutlineThickness(10);
	miniMap.circle.setOutlineColor(sf::Color(0, 0, 0));
	miniMap.circle.setPosition(MINIMAP_START_POS);
	Vector2f miniMapCenter = { miniMap.circle.getPosition().x + MINIMAP_CIRCLE_RADIUS,  miniMap.circle.getPosition().y + MINIMAP_CIRCLE_RADIUS };

	for (Npc & npc : npcList)
	{
		Sprite npcDot;
		npcDot = sprites.minimapMarkerNpc;
		npcDot.setPosition(750,750);
		npcDot.setOrigin(-50, 0);
		//npcDot.setPosition()
		miniMap.npcDotList.push_back(npcDot);
	}
	miniMap.heroDot = sprites.minimapMarkerHero;
	//miniMap.heroDot.setOrigin(-miniMapCenter.x, -miniMapCenter.y);

};

void UpdateMinimap(MiniMap & miniMap, vector<Npc> & npcList, Sprite & hero, View & view)
{
	int i = 0;

	Vector2f viewCenter = view.getCenter();
	miniMap.circle.setPosition(viewCenter.x + 450, viewCenter.y - 450);
	miniMap.heroDot.setPosition(miniMap.circle.getPosition().x + 50 - (miniMap.heroDot.getGlobalBounds().width / 2), miniMap.circle.getPosition().y + 50 - (miniMap.heroDot.getGlobalBounds().width / 2));

	for (Npc & npc : npcList)
	{
		Vector2f npcPos = GetSpriteCenter(npc.sprite);
		Vector2f heroPos = GetSpriteCenter(hero);

		float angle;
		
		angle = atan((heroPos.x - npcPos.x) / (heroPos.y - npcPos.y));
		angle = M_PI_2 - angle;
		angle = angle * 180 / M_PI;
		
		
		if (npcPos.y < heroPos.y)
		{
			angle = angle + 180;
		}
		
		Vector2f dist = {
			abs(npcPos.x - heroPos.x),
			abs(npcPos.y - heroPos.y)
		};

		float distance = sqrt(pow(dist.x, 2) + pow(dist.y, 2));

		if (distance < 5 * MINIMAP_CIRCLE_RADIUS)
		{
			miniMap.npcDotList[i].setOrigin(-distance / 5, 0);
		}
		else miniMap.npcDotList[i].setOrigin(-50, 0);

		miniMap.npcDotList[i].setRotation(angle);


		//miniMap.npcDotList[i].setPosition(miniMap.heroDot.getPosition());
		miniMap.npcDotList[i].setPosition(miniMap.circle.getPosition().x + 50, miniMap.circle.getPosition().y + 50);
		i++;
	}
};

void DrawMiniMap(RenderWindow & window, MiniMap & miniMap)
{
	window.draw(miniMap.circle);
	for (Sprite & dot : miniMap.npcDotList)
	{
		window.draw(dot);
	}
	window.draw(miniMap.heroDot);
}

void DeleteNpcDot(MiniMap & miniMap, int & index)
{
	miniMap.npcDotList.erase(miniMap.npcDotList.begin() + index);
}