#include "precomp.h"
#include "game.h"
#include "goomba.h"
#include "gameobject.h"

namespace Tmpl8
{
	Goomba::Goomba(float position_in_map_x, int level)
	{
		m_hitByFireBall = false;
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
		m_WIDTH = 32;
		m_HEIGHT = 32;
		m_health = 1;
		m_startMoving = false;
		m_start_x = position_in_map_x;
		m_position_x = position_in_map_x;
		m_direction = -1.0f;
		m_position_y = 400.0f;
		m_addFrame = false;
		m_frameTimer = 0.0f;
		goomba_sheet = new Sprite(new Surface("assets/goomba-sheet.png"), 3);
		m_frame = 0;
	}

	Goomba::~Goomba() {}

	void Goomba::Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x)
	{
		if (!m_killed)
		{
			m_topSide_y = m_position_y;
			m_rightSide_x = m_position_x + m_WIDTH;
			m_leftSide_x = m_position_x;
			m_downSide_y = m_position_y + m_HEIGHT;
		}

		if (m_leftSideCollision)
		{
			m_walkRight = true;

			m_leftSideCollision = false;
		}
		else if (m_rightSideCollision)
		{
			m_walkRight = false;

			m_rightSideCollision = false;
		}

		if (m_walkRight)
		{
			m_direction = 1.0f;
		}
		else
		{
			m_direction = -1.0f;
		}

		m_speed = map_speed_x * m_direction;

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
			if (playerObject->m_collisionActivated && !(m_topSideCollision || m_hitByFireBall))
			{
				m_collisionWithPlayer = true;
			}
		}

		if (m_collisionWithPlayer && (playerObject->m_downSide_y - this->m_topSide_y) <= 5)
		{
			this->m_topSideCollision = true;

			playerObject->m_hitEnemy = true;
		}

		if (m_collisionWithPlayer && !m_topSideCollision && playerObject->m_collisionActivated)
		{
			if (!playerObject->m_star)
			{
				playerObject->m_health -= 1;

				playerObject->m_collisionActivated = false;
			}
		}

		if ((m_position_x - playerObject->m_position_in_map_x) <= 420 && !m_topSideCollision && !m_hitByFireBall 
			&& !(m_collisionWithPlayer && playerObject->m_star))
		{
			m_spawn = true;

			this->m_startMoving = true;

			Movement();
		} 

		if (playerObject->m_atMiddleOfScreen && playerObject->m_playerWalkState == playerObject->WALK_RIGHT)
		{
			m_speed = map_speed_x * m_direction;
		}

		if (playerObject->m_fireBall_position_x >= m_leftSide_x && playerObject->m_fireBall_position_x <= m_rightSide_x
			&& playerObject->m_fireBall_position_y <= m_downSide_y && playerObject->m_fireBall_position_y >= m_topSide_y)
		{
			playerObject->m_fireBall_collision_with_enemy = true;
			m_hitByFireBall = true;
		}

		if (m_topSideCollision || m_hitByFireBall || (m_collisionWithPlayer && playerObject->m_star))
		{
			if (!m_lootGiven)
			{
				playerObject->m_score += 100;

				m_lootGiven = true;
			}

			DeathAnimation();
		}
	}

	void Goomba::DrawObject(Surface* screen)
	{
		if (!m_killed || (m_spawn && CheckOutOfFrame(m_position_x + 32)))
		{
			DrawSprite(screen, goomba_sheet, m_position_x, m_position_y, m_frame);
		}
	}

	void Goomba::Movement()
	{
		m_position_in_map_x += m_speed / 2;

		if (!m_collisionWithPlayer)
		{
			TimerManagement(0, 2);
		}
	}

	void Goomba::DeathAnimation() 
	{
		m_frame = 2;

		m_deathTimer++;

		if (m_deathTimer >= 50.0f)
		{
			m_killed = true;
		}
	}

	void Goomba::ResetGame(Mario* playerObject)
	{
		m_hitByFireBall = false;
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

		if (playerObject->m_level == 3)
		{
			m_position_y = 256.0f;
		}
		else
		{
			m_position_y = 370.0f;
		}

		m_addFrame = false;
		m_frameTimer = 0.0f;
		m_frame = 0;
		m_lootGiven = false;
	}
}