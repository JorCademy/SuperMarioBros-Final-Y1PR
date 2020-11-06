#include "precomp.h" // include this first in every .cpp file
#include "game.h"
#include "monster.h"

namespace Tmpl8
{
	Monster::Monster() {}

	Monster::~Monster() {}

	void Monster::DrawSprite(Surface* screen, Sprite* objectSprite, float position_x, float position_y, int frame)
	{
		(*objectSprite).Draw(screen, position_x, position_y);

		(*objectSprite).SetFrame(frame);
	}

	bool Monster::HitBoundaries(int position_x, int position_y)
	{
		bool hitBoundaries = false;

		if (m_position_x <= 0 || m_position_x >= SCRWIDTH)
		{
			hitBoundaries = true;
		}

		return hitBoundaries;
	}

	void Monster::TimerManagement(int minFrame, int maxFrame)
	{
		if ((int)(this->m_frameTimer) % 11 == 0)
		{
			this->m_addFrame = true;
		}
		else
		{
			this->m_addFrame = false;
		}

		this->m_frameTimer++;

		if (this->m_addFrame)
		{
			if (m_frame < 3)
			{
				if (++this->m_frame == maxFrame)
				{
					this->m_frame = minFrame;
				}
			}
			else
			{
				this->m_frame = minFrame;
			}
		}

		if ((int)(this->m_frameTimer >= 10000))
		{
			this->m_frameTimer = 0;
		}
	}

	void Monster::GroundCollisionCheck(int grid[19][224])
	{
		int whatchar = grid[(int)((this->m_downSide_y / 32))][(int)((this->m_position_in_map_x + 32) / 32)];

		if ((whatchar == 0 || whatchar == 13 || whatchar == 1 || whatchar == 33 || whatchar == 24 || whatchar == 264 || whatchar == 265 || whatchar == 3 || whatchar == 149 || whatchar == 48))
		{
			this->m_isGrounded = true;

			if (this->m_isGrounded && !this->m_killed)
			{
				this->m_ground_y = (int)(this->m_downSide_y - (32 * 2.5));
			}
		}
		else
		{
			this->m_isGrounded = false;

			this->m_ground_y = 550;
		}
	}

	void Monster::HorizontalCollisionCheck(int grid[19][224])
	{
		/* Collision on the left side */
		int whatchar_left = grid[(int)(((m_downSide_y - 32) / 32))][(int)((m_position_in_map_x + 16) / 32)];

		if (whatchar_left == 1 || whatchar_left == 3 || whatchar_left == 13 || whatchar_left == 24 || whatchar_left == 33 || whatchar_left == 298 || whatchar_left == 149)
		{
			m_leftSideCollision = true;
		}
		else
		{
			m_leftSideCollision = false;
		}

		/* Collision on the right side */
		int whatchar_right = grid[(int)(((m_downSide_y - 32) / 32))][(int)((m_position_in_map_x + m_WIDTH) / 32)];

		if (whatchar_right == 1 || whatchar_right == 3 || whatchar_right == 13 || whatchar_right == 24 || whatchar_right == 33 || whatchar_right == 297 || whatchar_right == 149)
		{
			m_rightSideCollision = true;
		}
		else
		{
			m_rightSideCollision = false;
		}
	}
	
	void Monster::Movement() {}
}