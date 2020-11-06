#include "precomp.h"
#include "bowser.h"

namespace Tmpl8
{
	Bowser::Bowser(float position_in_map_x, int level) 
	{
		m_timer = 0;
		m_pBowserSprite = new Sprite(new Surface("assets/bowser-sheet.png"), 2);
		m_frame = 0;
		m_pBowserSprite->SetFrame(m_frame);
		m_killed = false;
		m_levelToBePresent = level;
		m_walkRight = false;
		m_leftSideCollision = false;
		m_rightSideCollision = false;
		m_lootGiven = false;
		m_position_in_map_x = position_in_map_x;
		m_topSideCollision = false;
		m_killed = false;
		m_deathTimer = 0;
		m_collisionWithPlayer = false;
		m_WIDTH = 148;
		m_HEIGHT = 64;
		m_health = 1;
		m_startMoving = false;
		m_start_x = position_in_map_x;
		m_position_x = position_in_map_x;
		m_direction = -1.0f;
		m_position_y = 224.0f;
		m_addFrame = false;
		m_frameTimer = 0.0f;
	}

	Bowser::~Bowser() {}

	void Bowser::Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x)
	{
		if (!m_killed)
		{
			m_topSide_y = m_position_y;
			m_rightSide_x = m_position_x + m_WIDTH;
			m_leftSide_x = m_position_x + 16;
			m_downSide_y = m_position_y + m_HEIGHT + 32;
		}

		if (m_position_y >= 450)
		{
			m_killed = true;
		}

		if (playerObject->m_position_in_map_x < this->m_position_in_map_x)
		{
			m_walkRight = false;
			m_frame = 0;
		}
		else
		{
			m_walkRight = true;
			m_frame = 1;
		}

		if (m_leftSideCollision)
		{
			m_leftSideCollision = false;
			m_speed = 0;
		}
		else if (m_rightSideCollision)
		{
			m_rightSideCollision = false;
			m_speed = 0;
		}
		else
		{
			m_speed = map_speed_x * m_direction;
		}

		if (m_walkRight)
		{
			m_direction = 1.0f;
		}
		else
		{
			m_direction = -1.0f;
		}

		if (m_position_x > SCRWIDTH)
		{
			m_position_x = m_start_x - map_position_x;
		}

		if (!m_isGrounded)
		{
			if (m_downSide_y < m_ground_y)
			{
				m_isGrounded = false;
				m_position_y += m_gravity / 2;
			}
		}

		if (m_isGrounded && !((int)m_downSide_y % 32 == 0))
		{
			--m_position_y;
		}

		if (playerObject->CollisionDetection(this->m_position_x, this->m_position_y, this->m_WIDTH, this->m_HEIGHT))
		{
			if (playerObject->m_collisionActivated)
			{
				m_collisionWithPlayer = true;
			}
		}

		if (m_collisionWithPlayer && playerObject->m_collisionActivated)
		{
			if (!playerObject->m_star && playerObject->m_collisionActivated)
			{
				playerObject->m_health -= 1;

				playerObject->m_collisionActivated = false;
			}
		}

		if ((m_position_x - playerObject->m_position_in_map_x) <= 420 && !m_collisionWithPlayer)
		{
			m_spawn = true;

			this->m_startMoving = true;

			Movement(playerObject);
		}

		if (playerObject->m_atMiddleOfScreen && playerObject->m_playerWalkState == playerObject->WALK_RIGHT)
		{
			m_speed = map_speed_x * m_direction;
		}
	}

	void Bowser::DrawObject(Surface* screen) 
	{
		if (m_spawn && !m_killed)
		{
			DrawSprite(screen, m_pBowserSprite, m_position_x, m_position_y, m_frame);
		}
	}

	void Bowser::Movement(Mario* playerObject)
	{
		this->m_position_in_map_x += m_speed / 2;
		this->m_position_x += m_speed / 2;
	}

	void Bowser::HorizontalCollisionCheck(int grid[19][224])
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
		int whatchar_right = grid[(int)(((m_downSide_y - 32) / 32))][(int)((m_position_in_map_x + m_WIDTH + 48) / 32)];

		if (whatchar_right == 1 || whatchar_right == 3 || whatchar_right == 13 || whatchar_right == 24 || whatchar_right == 33 || whatchar_right == 297 || whatchar_right == 149)
		{
			m_rightSideCollision = true;
		}
		else
		{
			m_rightSideCollision = false;
		}
	}

	void Bowser::ResetGame(Mario* playerObject)
	{
		m_timer = 0;
		m_spawn = false;
		m_walkRight = false;
		m_leftSideCollision = false;
		m_rightSideCollision = false;
		m_position_in_map_x = m_start_x;
		m_topSideCollision = false;
		m_killed = false;
		m_deathTimer = 0;
		m_collisionWithPlayer = false;
		m_WIDTH = 32;
		m_HEIGHT = 32;
		m_health = 1;
		m_startMoving = false;
		m_direction = -1.0f;
		m_position_x = m_start_x;
		m_position_y = 224.0f;
		m_addFrame = false;
		m_frameTimer = 0.0f;
		m_frame = 0;
	}
}