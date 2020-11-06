#pragma once
#include "gameobject.h"
#include "specialitem.h"

namespace Tmpl8
{
	class Axe : public SpecialItem
	{
	public:
		Axe(float position_in_map_x, int level);
		~Axe();
		virtual void Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x);
		virtual void DrawObject(Surface* screen, Mario* playerObject);
		virtual void ResetGame();

		Sprite* m_pAxeSprite;
		bool m_collisionWithPlayer;
	};
}