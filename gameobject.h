#pragma once
#include "level.h"

namespace Tmpl8
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(float position_in_map_x, int level);
		~GameObject();
		virtual void Update(Surface* screen);
		virtual void DrawSprite(Surface* screen, Sprite* objectSprite, float position_x, float position_y, int frame);
		void Movement();
		bool SetTimer(float time);
		void RotationManagement();
		virtual bool CollisionDetection(float object_2_position_x, float object_2_position_y, int object_2_width, int object_2_height);
		virtual void GroundCollisionCheck(int grid[19][224]);
		virtual void HorizontalCollisionCheck(int grid[19][224]);
		virtual void HealthManagement();
		bool CheckOutOfFrame(float position_x);
		void DrawLines(Surface* screen, float leftSide_x, float rightSide_x, float topSide_y, float downSide_y);
		virtual void DrawObject(Surface* screen);
		virtual void DeathAnimation();
		virtual void ResetGame();

		int m_levelToBePresent;
		float m_timer;
		char* m_type;
		float m_position_x;
		float m_position_y;
		float m_position_in_map_x;
		float m_position_in_map_y;
		float m_start_x;
		float m_start_y;
		int m_WIDTH;
		int m_HEIGHT;
		float m_speed;
		bool m_killed;
		int m_health;
		float m_topSide_y;
		float m_downSide_y;
		float m_leftSide_x;
		float m_rightSide_x;
		bool m_spawn = false;
		bool m_rightSideCollision;
		bool m_leftSideCollision;
		bool m_topSideCollision;
		bool m_downSideCollision;
		bool m_stopWalkingLeft;
		bool m_stopWalkingRight;
		int m_ground_y;

	protected:
		Level* m_refLevel = new Level();
		bool m_walkRight;
		bool m_isGrounded;
		const float m_gravity = 9.81f; 
		int m_frame = 0;
		float m_rotation;
		int m_pointsWorth;
		int m_direction;
	};
}

