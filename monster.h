#pragma once
#include "gameobject.h"

namespace Tmpl8
{
	class Monster : public GameObject
	{
	public:
		Monster();
		~Monster();
		void TimerManagement(int minFrame, int maxFrame);
		virtual void Movement();
		virtual void DrawSprite(Surface* screen, Sprite* objectSprite, float position_x, float position_y, int frame);
		bool HitBoundaries(int position_x, int position_y);
		void GroundCollisionCheck(int grid[19][224]);
		virtual void HorizontalCollisionCheck(int grid[19][224]);

		float m_position_x;
		float m_position_y;
		float m_speed;
		int m_position_in_map_x;
		bool m_startMoving;
		int m_frame;

	protected:
		bool m_hitByFireBall;
		bool m_hitPlayer;
		bool m_lootGiven;
		float m_collisionTimer;
		bool m_walkRight;
		bool m_addFrame;
		int m_direction;
		float m_frameTimer;
		bool m_collisionWithPlayer;
		bool m_outOfFrame;
		bool hitBoundaries;
		float m_died_x;
	};
}
