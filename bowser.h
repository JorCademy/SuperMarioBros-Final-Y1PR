#pragma once
#include "monster.h"
#include "mario.h"

namespace Tmpl8
{
	class Bowser : public Monster
	{
	public:
		Bowser(float position_in_map_x, int level);
		~Bowser();
		virtual void Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x);
		virtual void DrawObject(Surface* screen);
		virtual void Movement(Mario* playerObject);
		virtual void HorizontalCollisionCheck(int grid[19][224]);
		virtual void ResetGame(Mario* playerObject);

	private:
		Sprite* m_pBowserSprite;
		bool m_walkRight;
		float m_deathTimer;
	};
}