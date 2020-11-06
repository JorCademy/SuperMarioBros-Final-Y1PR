#include "precomp.h" // include this first in every .cpp file
#include "mario.h"
#include "gameobject.h"
#include "game.h"

namespace Tmpl8
{
	Mario::Mario(Surface* screen)
	{
		m_direction = 1.0f;
		m_fireBall_collision_with_enemy = false;
		m_shoot = false;
		m_caveVisited = false;
		m_movementInPipeActivated = false;
		m_inPipe = false;
		m_movementActivated = true;
		m_endOfLevelTimerOver = false;
		m_gameWon = false;
		m_resetGame = false;
		m_level = 1;
		m_startLevel = false;
		m_timer = 0;
		m_starTimerOver = false;
		m_collisionActivated = true;
		m_star = false;
		m_fire = false;
		m_showStartingMenu = true;
		m_killed = false;
		m_maximum_y_after_kill_reached = false;
		m_health = 1;
		m_lives = 3;
		m_type = "Mario";

		m_start_x = 100.0f;
		m_start_y = 340.0f;
		m_position_x = m_start_x;
		m_position_y = m_start_y;
		m_position_in_map_x = m_start_x + 35;
		
		m_fireBall_position_x = m_start_x;
		m_fireBall_position_y = m_start_y;
		
		m_map_x_speed = 0.0f;
		m_WIDTH = 80;
		m_HEIGHT = 32;
		m_hitEnemy = false;
		m_hitByEnemy = false;

		m_pMarioDefaultBig = new Sprite(new Surface("assets/mario-big-default-sheet-v2.png"), 14);
		m_pMarioDefaultSmall = new Sprite(new Surface("assets/mario-small-default-sheet.png"), 14);
		m_pMarioFireBig = new Sprite(new Surface("assets/mario-big-fire-sheet.png"), 14);
		m_pMarioStarBig = new Sprite(new Surface("assets/mario-big-star-sheet.png"), 14);
		m_pFireBallSprite = new Sprite(new Surface("assets/fireball.png"), 1);
		m_pSelectedSprite = m_pMarioDefaultBig;

		m_stopWalkingLeft = false;
		m_stopWalkingRight = false;
		m_playerWalkState = STOP_WALKING;
		m_playerJumpState = STOP_JUMPING;
		m_playerIdleState = IDLE_RIGHT;
		m_frame = 7;
		m_jumpForce = 0.0f;
		m_deathJumpForce = -20.0f;
		m_isGrounded = true;
		m_down = false;

		m_score = 0;
		m_coins = 0;
		
		m_addFrame = true;
		m_animation_interpreter = 1;
		m_frameTimer = 0.0f;
		const float m_MAX_SPEED = 9.0f;
		m_middleOfScreen_x = 350.0f;
		m_atMiddleOfScreen = false;
		m_ground_y = 800.0f;

		m_rightSideCollision = false;
		m_leftSideCollision = false;
		m_topSideCollision = false;
		m_downSideCollision = true;
		m_reachedEndOfLevel = false;
	}

	Mario::~Mario() {}

	void Mario::Update(Surface* screen, float deltaTime, float m_map_speed)
	{
		m_leftSide_x = m_position_x + 20;
		m_rightSide_x = m_position_x + m_WIDTH + 28;
		m_topSide_y = m_position_y;
		m_downSide_y = (int) m_position_y + m_HEIGHT;

		m_frameTimer++;

		if (m_level == 4)
		{
			m_inPipe = true;
		}
		else
		{
			m_inPipe = false;
		}

		if (m_movementActivated)
		{
			Movement();
		}

		m_normal_speed = 3.0f * deltaTime / 12;

		m_normal_map_speed = m_map_speed;

		if (!m_isGrounded)
		{
			if (m_downSide_y < m_ground_y)
			{
				m_isGrounded = false;
				m_position_y += m_gravity / 2;
			}
		}

		if (m_hitByEnemy)
		{
			if (m_collisionActivated)
			{
				m_collisionActivated = false;
			}

			m_hitByEnemy = false;
		}

		if (!m_collisionActivated)
		{
			m_collisionActivated = SetTimer(100);
		}

		HealthManagement();

		if (m_isGrounded && !((int) m_downSide_y % 32 == 0))
		{
			--m_position_y;
		}

		if (this->m_downSide_y >= 530)
		{
			this->m_killed = true;
		}

		if (m_killed)
		{
			m_lives -= 1;

			if (m_lives > 0)
			{
				m_resetGame = true;

				m_startLevel = false;
			}
		}

		if (m_fire)
		{
			if (!m_shoot)
			{
				if (this->m_playerIdleState == IDLE_LEFT)
				{
					m_direction = -1.0f;
				}
				else
				{
					m_direction = 1.0f;
				}
			}

			if (m_shoot)
			{
				if (CheckOutOfFrame(m_fireBall_position_x))
				{
					m_shoot = false;
			
					m_fireBall_position_x = this->m_position_x + 32;
					m_fireBall_position_y = this->m_position_y + 50;

					m_fireBall_collision_with_enemy = false;
				}
				else
				{
					m_fireBall_position_x += m_speed * (2 * m_direction);

					m_fireBall_position_y += 5 * sin(m_fireBall_position_x * (PI / 180) * 2);
				}
			}
			else
			{
				m_fireBall_position_x = this->m_position_x + 32;
				m_fireBall_position_y = this->m_position_y + 50;
			}
			
			m_pSelectedSprite = m_pMarioFireBig;
		}
		else
		{
			m_fireBall_position_x = this->m_position_x + 32;
			m_fireBall_position_y = this->m_position_y + 50;
		}

		EndOfLevelCheck();

		if (m_star)
		{
			StarTimer();
		}
	}

	bool Mario::CollisionDetection(float object_2_position_x, float object_2_position_y, int object_2_width, int object_2_height)
	{
		bool collisionDetected = false;

		int object_1_x_min = this->m_position_x;
		int object_1_x_max = this->m_position_x + this->m_WIDTH;
		int object_1_y_min = this->m_position_y;
		int object_1_y_max = this->m_position_y + this->m_HEIGHT;

		int object_2_x_min = object_2_position_x;
		int object_2_x_max = object_2_position_x + object_2_width;
		int object_2_y_min = object_2_position_y;
		int object_2_y_max = object_2_position_y + object_2_height;

		if (!((object_1_x_max < object_2_x_min || object_1_x_min > object_2_x_max) || ((object_1_y_max < object_2_y_min) || (object_1_y_min > object_2_y_max))))
		{
			collisionDetected = true;
		}

		return collisionDetected;
	}

	void Mario::ActivePipeDetection(int x_coords[2])
	{
		if (m_level == 1 && m_position_in_map_x >= (x_coords[0] - 32) && m_position_in_map_x <= x_coords[1])
		{
			m_onActivePipe = true;
		}
		else
		{
			m_onActivePipe = false;
		}
	}

	void Mario::GroundCollisionCheck(int grid[19][224])
	{
		int whatchar = grid[(int)((m_downSide_y / 32))][(int)((m_position_in_map_x + 32) / 32)];

		if ((whatchar == 0 || whatchar == 2 || whatchar == 1 || whatchar == 33 || whatchar == 24 || whatchar == 264 || whatchar == 265 || whatchar == 3 
			|| whatchar == 149 || whatchar == 48 || whatchar == 66 || whatchar == 68) && !CheckOutOfFrame(m_position_x))
		{
			m_isGrounded = true;

			if (m_isGrounded && !m_killed)
			{
				m_ground_y = (int)(m_downSide_y - (32 * 2.5));
			}
		}
		else
		{
			m_ground_y = 550;
			m_isGrounded = false;
		}
	}

	void Mario::TopCollisionCheck(int grid[19][224])
	{
		int whatchar = grid[(int)((m_topSide_y / 32))][(int)((m_position_in_map_x + 32) / 32)];

		if (whatchar == 0 || whatchar == 3 || whatchar == 2 || whatchar == 1 || whatchar == 33 || whatchar == 24 || whatchar == 149 || 
			whatchar == 66 || whatchar == 68)
		{
			m_topSideCollision = true;

			if (m_playerJumpState != STOP_JUMPING)
			{
				if (whatchar == 24)
				{
					grid[(int)((m_topSide_y / 32))][(int)((m_position_in_map_x + 32) / 32)] = 3;
				}

				if ((whatchar == 1 || whatchar == 2) && m_health == 2)
				{
					grid[(int)((m_topSide_y / 32))][(int)((m_position_in_map_x + 32) / 32)] = 28;
				} 
				else if (whatchar == 68)
				{
					grid[(int)((m_topSide_y / 32))][(int)((m_position_in_map_x + 32) / 32)] = 46;
				}
			}
		}
	}

	void Mario::HealthManagement()
	{
		if (m_health == 1)
		{
			Shrink();
		}
		else if (m_health <= 0)
		{
			if (!m_killed)
			{
				m_jumpForce = -25.0f;
			}

			DeathAnimation();
		}
	}

	void Mario::Jump() 
	{
		if (!m_topSideCollision)
		{
			m_position_y += m_jumpForce;
		}

		if (m_position_y < m_ground_y)
		{
			m_isGrounded = false;
			if (m_jumpForce <= m_MAX_JUMPSPEED)
			{
				m_jumpForce += m_gravity / 20;
			}
		} 
		else if (m_downSide_y >= m_ground_y)
		{
			m_playerJumpState = STOP_JUMPING;
			m_isGrounded = true;
			m_topSideCollision = false;

			m_position_y = m_ground_y;

			m_jumpForce = 0;
		}
	}

	void Mario::Movement() 
	{
		switch (m_playerWalkState)
		{
		case WALK_LEFT:
			if (!m_leftSideCollision)
			{
				MoveLeft();
			}
			break;

		case WALK_RIGHT:
			if (!m_rightSideCollision)
			{
				MoveRight();
			}
			break;
		}

		switch (m_playerIdleState)
		{
		case IDLE_RIGHT:
			if (m_isGrounded && m_playerWalkState == STOP_WALKING && m_playerJumpState == STOP_JUMPING)
			{
				IdleRight();
			}
			break;

		case IDLE_LEFT:
			if (m_isGrounded && m_playerWalkState == STOP_WALKING && m_playerJumpState == STOP_JUMPING)
			{
				IdleLeft();
			}
			break;
		}

		switch (m_playerJumpState)
		{
		case JUMP_LEFT:
			m_isGrounded = false;
			m_frame = 1;
			Jump();
			break;

		case JUMP_RIGHT:
			m_isGrounded = false;
			m_frame = 12;
			Jump();
			break;
		}
	}

	void Mario::IdleLeft() 
	{
		m_frame = 6;
	}

	void Mario::IdleRight() 
	{
		m_frame = 7;
	}

	void Mario::MoveRight() 
	{
		if (m_frame >= 3 && m_frame <= 6)
		{
			m_frame == 9;
		}

		if ((int)m_frameTimer % 11 == 0)
		{
			m_addFrame = true;
		}
		else
		{
			m_addFrame = false;
		}

		if (m_addFrame == true)
		{
			m_frame += m_animation_interpreter;

			if (m_animation_interpreter == 1 && m_frame + m_animation_interpreter >= 11)
			{
				m_animation_interpreter = -1;
			}
			else if (m_animation_interpreter == -1 && m_frame + m_animation_interpreter <= 7)
			{
				m_animation_interpreter = 1;
			}
		}

		m_map_x_speed = m_normal_map_speed;

		if (m_position_x < m_middleOfScreen_x)
		{
			if (!(m_leftSideCollision || m_rightSideCollision))
			{
				m_speed = m_normal_speed;
			}

			m_map_x_speed = m_normal_map_speed;

			m_atMiddleOfScreen = false;

			m_position_x += m_speed;
		}

		if (m_position_x >= m_middleOfScreen_x)
		{
			m_atMiddleOfScreen = true;

			m_position_x += 0;
		}

		m_position_in_map_x += m_map_x_speed;
	}

	void Mario::MoveLeft() 
	{
		if (m_frame >= 8 && m_frame <= 11)
		{
			m_frame == 2;
		}
		else if ((int)m_frameTimer % 11 == 0)
		{
			m_addFrame = true;
		}
		else
		{
			m_addFrame = false;
		}

		if (m_addFrame == true)
		{
			m_frame += m_animation_interpreter;

			if (m_animation_interpreter == -1 && m_frame + m_animation_interpreter <= 3)
			{
				m_animation_interpreter = 1;
			}
			else if (m_animation_interpreter == 1 && m_frame + m_animation_interpreter >= 5)
			{
				m_animation_interpreter = -1;
			}
		}

		m_map_x_speed = m_speed;

		if (m_position_x <= 32)
		{
			m_speed = 0;

			m_map_x_speed = 0.0f;
		}
		else
		{
			m_speed = m_normal_speed;

			m_map_x_speed = m_normal_map_speed;
		}

		m_position_x -= m_speed;

		m_position_in_map_x -= m_map_x_speed;
	}

	void Mario::ShootFire() {}

	void Mario::Grow() 
	{
		if (!m_fire)
		{
			m_pSelectedSprite = m_pMarioDefaultBig;
		}

		if (m_topSide_y == (m_position_y + 40))
		{
			m_topSide_y = m_position_y;
		}
	}

	void Mario::StarTimer()
	{
		if (m_star)
		{
			m_health = 2;

			m_pSelectedSprite = m_pMarioStarBig;

			m_starTimerOver = SetTimer(600.0f);
		
			if (m_starTimerOver)
			{
				if (m_fire)
				{
					m_pSelectedSprite = m_pMarioFireBig;
				}
				else
				{
					m_pSelectedSprite = m_pMarioDefaultBig;
				}

				m_star = false;
			}
		}
	}

	void Mario::Shrink() 
	{
		m_fire = false;

		m_pSelectedSprite = m_pMarioDefaultSmall;
	
		m_topSide_y = m_position_y + 40;
	}

	char* Mario::GetScore()
	{
		char* scoreToChar = new char[10];

		itoa(m_score, scoreToChar, 10);

		return scoreToChar;

		/* Reference: https://docs.microsoft.com/en-us/cpp/code-quality/c6283?view=vs-2019 */
		delete[] scoreToChar;
	}

	char* Mario::GetCoins()
	{
		char* coinsToChar = new char[10];

		itoa(m_coins, coinsToChar, 10);

		return coinsToChar;
	
		delete[] coinsToChar;
	}

	char* Mario::GetLevel()
	{
		char* levelToChar = new char[10];

		itoa(m_level, levelToChar, 10);

		return levelToChar;

		delete[] levelToChar;
	}

	char* Mario::GetLives()
	{
		char* livesToChar = new char[10];

		itoa(m_lives, livesToChar, 10);

		return livesToChar;

		delete[] livesToChar;
	}

	void Mario::DeathAnimation()
	{
		m_frame = 0;

		m_ground_y = 500;

		m_position_y += m_deathJumpForce;

		if (m_position_y < m_ground_y)
		{
			m_isGrounded = false;
			m_deathJumpForce += m_gravity / 10;
		}
		else if (m_downSide_y >= m_ground_y)
		{
			m_playerJumpState = STOP_JUMPING;
			m_isGrounded = true;
			m_topSideCollision = false;

			m_position_y = m_ground_y;

			m_killed = true;
		}
	}

	void Mario::DrawObject(Surface* screen) 
	{
		DrawSprite(screen, m_pSelectedSprite, m_position_x, m_position_y, m_frame);

		if (m_shoot && m_fire)
		{
			DrawSprite(screen, m_pFireBallSprite, m_fireBall_position_x, m_fireBall_position_y, 0);
		}
	}

	void Mario::EndOfLevelCheck()
	{
		if (this->m_position_in_map_x >= m_END_LEVEL_X)
		{
			if (m_level < 3)
			{
				this->m_reachedEndOfLevel = true;
			}
		}
		
		if (m_gameWon)
		{
			m_reachedEndOfLevel = true;
		}

		if (m_reachedEndOfLevel)
		{
			m_endOfLevelTimerOver = SetTimer(0);

			if (m_endOfLevelTimerOver)
			{
				if (m_level == 1)
				{
					m_level = 2;
				}
				else if (m_level == 2)
				{
					m_level = 3;
				}
				else if (m_level == 3)
				{
					m_level = 1;
					m_showStartingMenu = true;
				}

				m_startLevel = false;
			}
		}
	}

	void Mario::ResetGame()
	{
		if (m_showStartingMenu)
		{
			m_gameWon = false;

			m_score = 0;

			m_coins = 0;

			m_health = 1;

			m_lives = 3;

			m_level = 1;

			m_fire = false;
		}

		m_direction = 1.0f;
		m_fireBall_collision_with_enemy = false;
		m_movementActivated = true;
		m_resetGame = false;
		m_killed = false;
		m_start_x = 100.0f;
		m_start_y = 340.0f;
		m_position_x = m_start_x;

		if (m_level == 3)
		{
			m_position_y = 230;
		}
		else
		{
			m_position_y = m_start_y;
		}

		m_health = 1;
		m_down = false;
		m_position_in_map_x = m_start_x;
		m_map_x_speed = 0.0f;
		m_WIDTH = 32;
		m_HEIGHT = 80;
		m_hitEnemy = false;
		m_hitByEnemy = false;
		m_playerWalkState = STOP_WALKING;
		m_playerJumpState = STOP_JUMPING;
		m_playerIdleState = IDLE_RIGHT;
		m_frame = 7;
		m_jumpForce = 0.0f;
		m_deathJumpForce = -20.0f;
		m_isGrounded = true;
		m_addFrame = true;
		m_animation_interpreter = 1;
		m_frameTimer = 0.0f;
		m_middleOfScreen_x = 350.0f;
		m_atMiddleOfScreen = true;
		m_ground_y = 800.0f;
		m_rightSideCollision = false;
		m_leftSideCollision = false;
		m_topSideCollision = false;
		m_downSideCollision = true;
		m_reachedEndOfLevel = false;
		m_endOfLevelTimerOver = false;
		m_collisionActivated = true;
		m_maximum_y_after_kill_reached = false;
		m_inPipe = false;
		m_movementInPipeActivated = false;
		m_caveVisited = false;
	}
}
