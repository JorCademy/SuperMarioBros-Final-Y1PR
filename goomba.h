#pragma once
#include "monster.h"
#include "gameobject.h"
#include "mario.h"

namespace Tmpl8
{
	class Goomba : public Monster
	{
	public:
		Goomba(float m_position_in_map_x, int level);
		~Goomba();
		virtual void Movement();
		virtual void Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x);
		virtual void DrawObject(Surface* screen);
		virtual void DeathAnimation();
		virtual void ResetGame(Mario* playerObject);

	private:
		float m_deathTimer;
		Sprite* goomba_sheet;
		bool m_walkRight;
	};
}
