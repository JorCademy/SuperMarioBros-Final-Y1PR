#include "precomp.h" // include this first in every .cpp file
#include "turtle.h"
#include "monster.h"

namespace Tmpl8
{
	Turtle::Turtle(float position_in_map_x, int level)
	{
		m_timer = 0;
		m_enabledDamagePlayer = true;
		m_hitByFireBall = false;
		m_minFrame = 2;
		m_maxFrame = 4;
		m_levelToBePresent = level;
		m_shoot = false;
		m_walkRight = false;
		m_leftSideCollision = false;
		m_rightSideCollision = false;
		m_lootGiven = false;
		m_startMoving = false;
		m_start_x = position_in_map_x;
		m_position_in_map_x = position_in_map_x;
		m_killed = false;
		m_WIDTH = 32;
		m_HEIGHT = 48;
		m_health = 2;
		m_frame = 0;
		m_green_turtle_sheet = new Sprite(new Surface("assets/turtle-green-sheet.png"), 4);
		m_green_shell_sheet = new Sprite(new Surface("assets/turtle-green-shell-sheet.png"), 2);
		m_selectedSprite = m_green_turtle_sheet;
		m_frameTimer = 0.0f;
		m_position_x = position_in_map_x;
		m_position_y = 350.0f;
		m_addFrame = false;
		m_direction = -1;
		m_collisionWithPlayer = false;
		m_topSideCollision = false;
	}

	Turtle::~Turtle() {}

	void Turtle::Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x)
	{
		TimerManagement(m_minFrame, m_maxFrame);

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
			m_minFrame = 0;
			m_maxFrame = 2;

			m_direction = 1.0f;
		}
		else
		{
			m_minFrame = 2;
			m_maxFrame = 4;

			m_direction = -1.0f;
		}

		m_speed = map_speed_x * m_direction;

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

		if (playerObject->CollisionDetection(this->m_position_x, this->m_position_y, this->m_WIDTH, this->m_HEIGHT)
			&& !(m_topSideCollision || m_hitByFireBall))
		{
			m_collisionWithPlayer = true;
		}

		if (m_collisionWithPlayer && (playerObject->m_downSide_y - this->m_topSide_y) <= 5)
		{
			this->m_topSideCollision = true;

			playerObject->m_hitEnemy = true;

			m_health -= 1;
		}

		if (m_collisionWithPlayer && !m_topSideCollision)
		{
			if (!playerObject->m_star && playerObject->m_collisionActivated && m_enabledDamagePlayer)
			{
				playerObject->m_health -= 1;
				m_collisionWithPlayer = false;
			}
			else
			{
				m_health = 1;
			}
		}

		if (m_hitByFireBall)
		{
			m_health = 1;
		}

		HealthCheck();

		if ((this->m_position_in_map_x - playerObject->m_position_in_map_x) <= 420 && !m_hitByFireBall)
		{
			m_spawn = true;

			this->m_startMoving = true;
		}

		if (playerObject->m_fireBall_position_x >= m_leftSide_x && playerObject->m_fireBall_position_x <= m_rightSide_x
			&& playerObject->m_fireBall_position_y <= m_downSide_y && playerObject->m_fireBall_position_y >= m_topSide_y)
		{
			playerObject->m_fireBall_collision_with_enemy = true;
			m_hitByFireBall = true;
		}

		if (m_health >= 2)
		{
			if (m_startMoving)
			{
				Movement();
			}
		}
		else if (m_health <= 1)
		{
			if (!m_lootGiven)
			{
				playerObject->m_score += 100;

				m_lootGiven = true;
			}

			m_HEIGHT = 32;
			m_WIDTH = 32;

			TurnIntoShell(playerObject, map_speed_x);
		}

		if (m_shoot)
		{
			m_enabledDamagePlayer = SetTimer(50.0f);
		}
	}

	void Turtle::Movement()
	{
		m_position_in_map_x += m_speed / 2;
	}

	void Turtle::ShootShell(Mario* playerObject) 
	{
		if (m_selectedSprite = m_green_shell_sheet)
		{
			m_position_in_map_x += m_speed * 2;

			if (playerObject->CollisionDetection(m_position_x, m_position_y, m_WIDTH, m_HEIGHT) &&
				playerObject->m_collisionActivated && !playerObject->m_star && m_shoot && m_enabledDamagePlayer)
			{
				playerObject->m_health -= 1;
			}
		}
	}

	void Turtle::TurnIntoShell(Mario* playerObject, float m_map_speed) 
	{
		m_frame = 0;

		if (playerObject->m_rightSide_x >= m_leftSide_x && playerObject->m_rightSide_x < (m_rightSide_x - 20) 
			&& (m_downSide_y - playerObject->m_downSide_y) < 5 && !m_shoot)
		{
			m_walkRight = true;

			m_shoot = true;
		} if (playerObject->m_leftSide_x <= m_rightSide_x && playerObject->m_leftSide_x > (m_leftSide_x + 20)
			&& (m_downSide_y - playerObject->m_downSide_y) < 5 && !m_shoot)
		{
			m_walkRight = false;

			m_shoot = true;
		}
		else
		{
			if (!m_shoot)
			{
				m_speed = 0;

				if (playerObject->m_playerWalkState == playerObject->WALK_RIGHT && playerObject->m_atMiddleOfScreen)
				{
					m_position_x -= m_map_speed;
				}

				m_selectedSprite = m_green_shell_sheet;
			}
		}

		if (m_shoot)
		{
			ShootShell(playerObject);
		}
	}

	void Turtle::HealthCheck()
	{
		if (m_topSideCollision && m_health == 2)
		{
			m_health -= 1;

			m_collisionWithPlayer = false;
		}
	}

	void Turtle::DrawObject(Surface* screen)
	{
		if (m_spawn && !CheckOutOfFrame(m_position_x))
		{
			DrawSprite(screen, m_selectedSprite, m_position_x, m_position_y, m_frame);
		}
	}

	void Turtle::ResetGame(Mario* playerObject)
	{
		m_timer = 0;
		m_enabledDamagePlayer = true;
		m_hitByFireBall = false;
		m_spawn = false;
		m_walkRight = false;
		m_leftSideCollision = false;
		m_rightSideCollision = false;
		m_position_in_map_x = m_start_x;
		m_killed = false;
		m_WIDTH = 32;
		m_HEIGHT = 48;
		m_health = 2;
		m_selectedSprite = m_green_turtle_sheet;
		m_frameTimer = 0.0f;
		m_position_x = m_position_in_map_x;

		if (playerObject->m_level == 3)
		{
			m_position_y = 256.0f;
		}
		else
		{
			m_position_y = 370.0f;
		}

		m_addFrame = false;
		m_direction = -1;
		m_startMoving = false;
		m_collisionWithPlayer = false;
		m_topSideCollision = false;
		m_shoot = false;
		m_lootGiven = false;
	}
}