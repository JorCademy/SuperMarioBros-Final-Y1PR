#include "precomp.h" // include this first in every .cpp file
#include "specialitem.h"
#include "game.h"
#include "gameobject.h"
#include "mario.h"

namespace Tmpl8
{
	SpecialItem::SpecialItem() {}

	SpecialItem::SpecialItem(int type, int position_in_map_x, int position_in_map_y, int level)
	{
		m_levelToBePresent = level;
		m_walkRight = true;
		m_direction = 1;
		m_startItemType = type;
		m_lootGiven = false;
		m_timerOver = true;
		m_timer = 0;
		m_isMoving = false;
		m_collidableWithPlayer = false;
		m_HEIGHT = 32;
		m_WIDTH = 32;
		m_start_x = position_in_map_x;
		m_start_y = position_in_map_y;
		m_start_y_when_spawned = m_start_y - 32;
		m_position_x = m_start_x;
		m_position_in_map_x = position_in_map_x;
		m_position_y = m_start_y;
		m_collidedWithPlayer = false;
		m_spawn = false;
		m_pMushroomSprite = new Sprite(new Surface("assets/mushroom.png"), 1);
		m_pCoinSprite = new Sprite(new Surface("assets/coin.png"), 1);
		m_pFlowerSprite = new Sprite(new Surface("assets/flower.png"), 1);
		m_pStarSprite = new Sprite(new Surface("assets/star.png"), 1);
		m_pTransparentSprite = new Sprite(new Surface("assets/transparent.png"), 1);
		m_pSelectedSprite = m_pMushroomSprite;
		m_itemType = type;
	}

	SpecialItem::~SpecialItem() {}

	void SpecialItem::Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x)
	{
		m_speed = map_speed_x;

		m_topSide_y = m_position_y;
		m_rightSide_x = m_position_x + m_WIDTH;
		m_leftSide_x = m_position_x;
		m_downSide_y = m_position_y + m_HEIGHT;

		if (((m_position_x - 32) - playerObject->m_position_x) <= 5 && ((m_position_x + 32) - playerObject->m_position_x) >= 0 && !(playerObject->m_playerJumpState == playerObject->STOP_JUMPING))
		{
			if ((playerObject->m_topSide_y - m_downSide_y) <= 5 && playerObject->m_topSide_y > m_downSide_y && playerObject->m_topSideCollision)
			{
				m_spawn = true;
			}
		}

		switch (m_itemType)
		{
		case MUSHROOM:
			m_pSelectedSprite = m_pMushroomSprite;
			break;
		case COIN:
			m_pSelectedSprite = m_pCoinSprite;
			m_collidableWithPlayer = false;
			break;
		case FLOWER:
			m_pSelectedSprite = m_pFlowerSprite;
			break;			
		case STAR:
			m_pSelectedSprite = m_pStarSprite;
			break;
		case INVISIBLE:
			m_pSelectedSprite = m_pTransparentSprite;
			m_collidableWithPlayer = false;
			break;
		}

		if (m_collidableWithPlayer && playerObject->CollisionDetection(m_position_x, m_position_y, 32, 32))
		{
			switch (m_itemType)
			{
			case MUSHROOM: 
				if (playerObject->m_health == 1)
				{
					playerObject->m_health += 1;

					playerObject->Grow();
				}
				break;

			case FLOWER:
				if (playerObject->m_health == 1)
				{
					playerObject->m_fire = true;

					playerObject->Grow();

					playerObject->m_health++;
				}
				else
				{
					playerObject->m_fire = true;
				}
				break;

			case STAR:
				playerObject->m_star = true;
				break;
			}

			if (!m_lootGiven)
			{
				playerObject->m_score += 200;

				m_lootGiven = true;
			}

			m_collidedWithPlayer = true;

			m_itemType = INVISIBLE;
		}

		if (m_downSide_y > 500 || m_position_x <= 0)
		{
			m_itemType = INVISIBLE;
		}

		if (m_spawn && !CheckOutOfFrame(m_position_x) && m_collidedWithPlayer == false)
		{
			if (m_itemType != INVISIBLE)
			{
				Movement(deltaTime);

				Spawn(playerObject);
			}
		}
	}

	void SpecialItem::Spawn(Mario* playerObject) 
	{
		switch (m_itemType)
		{
		case MUSHROOM:
			if (!m_isMoving && m_position_y > m_start_y_when_spawned)
			{
				m_position_y--;
			}
			else
			{
				m_isMoving = true;
				m_collidableWithPlayer = true;
			}

			if (m_isMoving && m_isGrounded && !((int)m_downSide_y % 32 == 0))
			{
				--m_position_y;
			}

			if (!m_isGrounded && m_isMoving)
			{
				if (m_downSide_y < m_ground_y)
				{
					m_isGrounded = false;
					m_position_y += m_gravity / 2;
				}
			}

			if (CheckOutOfFrame(m_position_x))
			{
				m_itemType = INVISIBLE;
			}
			break;

		case COIN:
			if (!m_isMoving && m_position_y > m_start_y_when_spawned)
			{
				if (!m_lootGiven)
				{
					playerObject->m_coins += 1;
					playerObject->m_score += 200;

					m_lootGiven = true;
				}

				m_position_y--;
			}
			else
			{
				TimerUntilDisappearing(100);

				if (m_timerOver)
				{
					m_itemType = INVISIBLE;
				}
			}
			break;

		case FLOWER:
			if (!m_isMoving && m_position_y > m_start_y_when_spawned)
			{
				m_position_y--;
			}
			else
			{
				m_collidableWithPlayer = true;
			}

			if (CheckOutOfFrame(m_position_x))
			{
				m_itemType = INVISIBLE;
			}
			break;
		 
		case STAR:
			if (!m_isMoving && m_position_y > m_start_y_when_spawned)
			{
				m_position_y--;
			}
			else
			{
				m_collidableWithPlayer = true;
			}

			if (CheckOutOfFrame(m_position_x))
			{
				m_itemType = INVISIBLE;
			}
			break;

		case INVISIBLE:
			m_pSelectedSprite = m_pTransparentSprite;
			m_collidableWithPlayer = false;
			break;
		}
	}

	void SpecialItem::Movement(float deltaTime) 
	{
		if (m_isMoving)
		{
			if (m_rightSideCollision)
			{
				m_walkRight = false;
			}
			else if (m_leftSideCollision)
			{
				m_walkRight = true;
			}

			if (m_walkRight)
			{
				m_direction = 1;
			}
			else
			{
				m_direction = -1;
			}

			m_position_in_map_x += m_speed * m_direction;
			m_position_x += m_speed * m_direction;
		}
	}

	void SpecialItem::DrawWhenSpawning(Surface* screen)
	{
		if (!m_isMoving && m_spawn)
		{
			m_pSelectedSprite->DrawScaled(m_position_x, m_position_y, 32, 32, screen);
		}
	}

	void SpecialItem::DrawWhenMoving(Surface* screen)
	{
		if (m_isMoving && m_spawn)
		{
			m_pSelectedSprite->DrawScaled(m_position_x, m_position_y, 32, 32, screen);
		}
	}

	void SpecialItem::GroundCollisionCheck(int grid[19][224])
	{
		int whatchar = grid[(int)((this->m_downSide_y / 32))][(int)((this->m_position_in_map_x + 32) / 32)];

		if ((whatchar == 0 || whatchar == 2 || whatchar == 1 || whatchar == 33 || whatchar == 24 || whatchar == 264 || whatchar == 265 || whatchar == 3 || whatchar == 149))
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

	void SpecialItem::HorizontalCollisionCheck(int grid[19][224])
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
		int whatchar_right = grid[(int)(((m_downSide_y - 32) / 32))][(int)((m_position_in_map_x) / 32)];

		if (whatchar_right == 1 || whatchar_right == 3 || whatchar_right == 13 || whatchar_right == 24 || whatchar_right == 33 || whatchar_right == 297 || whatchar_right == 149)
		{
			m_rightSideCollision = true;
		}
		else
		{
			m_rightSideCollision = false;
		}
	}

	void SpecialItem::TimerUntilDisappearing(float time)
	{
		if (this->m_timerOver == true)
		{
			this->m_timer++;

			if (this->m_timer > time)
			{
				this->m_timerOver = false;
			}
		}
	}

	void SpecialItem::ResetGame()
	{
		m_itemType = m_startItemType;
		m_lootGiven = false;
		m_timerOver = true;
		m_timer = 0;
		m_isMoving = false;
		m_collidableWithPlayer = false;
		m_HEIGHT = 32;
		m_WIDTH = 32;
		m_start_y_when_spawned = m_start_y - 32;
		m_position_x = m_start_x;
		m_position_in_map_x = m_start_x;
		m_position_y = m_start_y;
		m_collidedWithPlayer = false;
		m_spawn = false;
		m_pSelectedSprite = m_pMushroomSprite;
		m_direction = 1;
		m_walkRight = true;
	}
}