#include "precomp.h"
#include "axe.h"

namespace Tmpl8
{
	Axe::Axe(float position_in_map_x, int level) 
	{
		m_pAxeSprite = new Sprite(new Surface("assets/axe.png"), 1);
		m_levelToBePresent = level;
		m_position_in_map_x = position_in_map_x;
		m_collisionWithPlayer = false;
		m_WIDTH = 32;
		m_HEIGHT = 32;
		m_start_x = position_in_map_x;
		m_position_x = position_in_map_x;
		m_position_y = 256.0f;
		m_frame = 0;
	}

	Axe::~Axe() {}

	void Axe::Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x)
	{
		m_topSide_y = m_position_y;
		m_rightSide_x = m_position_x + m_WIDTH;
		m_leftSide_x = m_position_x;
		m_downSide_y = m_position_y + m_HEIGHT;

		m_speed = map_speed_x * m_direction;

		m_position_in_map_x = m_start_x - map_position_x;

		if (playerObject->CollisionDetection(this->m_position_x, this->m_position_y, this->m_WIDTH, this->m_HEIGHT))
		{
			m_collisionWithPlayer = true;
		}
	}

	void Axe::DrawObject(Surface* screen, Mario* playerObject)
	{
		if (m_collisionWithPlayer == false && playerObject->m_level == 3)
		{
			DrawSprite(screen, m_pAxeSprite, m_position_x, m_position_y, 0);
		}
	}

	void Axe::ResetGame()
	{
		m_collisionWithPlayer = false;
	}
}