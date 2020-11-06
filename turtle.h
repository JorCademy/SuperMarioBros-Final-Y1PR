#pragma once
#include "mario.h"
#include "monster.h"

namespace Tmpl8
{
	class Turtle : public Monster
	{
	public:
		Turtle(float m_position_in_map_x, int level);
		~Turtle();
		virtual void Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x);
		virtual void Movement();
		void ShootShell(Mario* playerObject);
		void TurnIntoShell(Mario* playerObject, float m_map_speed);
		void HealthCheck();
		virtual void DrawObject(Surface* screen);
		virtual void ResetGame(Mario* playerObject);

	private:
		Sprite* m_selectedSprite;
		Sprite* m_green_turtle_sheet;
		Sprite* m_green_shell_sheet;
		bool m_enabledDamagePlayer;
		int m_minFrame;
		int m_maxFrame;
		bool m_hitByPlayer;
		bool m_walkRight;
		bool m_shoot;
	};
}
