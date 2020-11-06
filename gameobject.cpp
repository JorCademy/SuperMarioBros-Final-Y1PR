#include "precomp.h" // include this first in every .cpp file
#include "gameobject.h"

namespace Tmpl8
{
	GameObject::GameObject() {}

	GameObject::GameObject(float position_in_map_x, int level) {}

	GameObject::~GameObject() {}

	void GameObject::Update(Surface* screen) {}

	void GameObject::DrawSprite(Surface* screen, Sprite* objectSprite, float position_x, float position_y, int frame)
	{
		(*objectSprite).Draw(screen, position_x, position_y);

		(*objectSprite).SetFrame(frame);
	}

	void GameObject::Movement() {}

	bool GameObject::SetTimer(float time) 
	{
		m_timer++;

		if (m_timer >= time)
		{
			m_timer = 0;

			return true;
		}

		return false;
	}

	void GameObject::RotationManagement() {}

	bool GameObject::CollisionDetection(float object_2_position_x, float object_2_position_y, int object_2_width, int object_2_height)
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

	void GameObject::GroundCollisionCheck(int grid[19][224]) {}

	void GameObject::HealthManagement() {}

	bool GameObject::CheckOutOfFrame(float position_x) 
	{
		if (position_x < 0 || position_x > 800)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void GameObject::HorizontalCollisionCheck(int grid[19][224])
	{
		/* Collision on the left side */
		int whatchar_left = grid[(int)(((m_downSide_y - 32) / 32))][(int)((m_position_in_map_x + 16) / 32)];

		if (whatchar_left == 1 || whatchar_left == 3 || whatchar_left == 2 || whatchar_left == 24 || whatchar_left == 33 || whatchar_left == 298 
			|| whatchar_left == 149 || whatchar_left == 266 || whatchar_left == 299 || whatchar_left == 68 || whatchar_left == 66)
		{
			m_leftSideCollision = true;
		}
		else
		{
			m_leftSideCollision = false;
		}

		/* Collision on the right side */
		int whatchar_right = grid[(int)(((m_downSide_y - 32) / 32))][(int)((m_position_in_map_x + 64) / 32)];

		if (whatchar_right == 1 || whatchar_right == 3 || whatchar_right == 2 || whatchar_right == 24 || whatchar_right == 33 || whatchar_right == 297 
			|| whatchar_right == 149 || whatchar_right == 266 || whatchar_right == 299 || whatchar_left == 68 || whatchar_left == 66)
		{
			m_rightSideCollision = true;
		}
		else
		{
			m_rightSideCollision = false;
		}
	}

	void GameObject::DrawLines(Surface* screen, float leftSide_x, float rightSide_x, float topSide_y, float downSide_y)
	{
		screen->Line(leftSide_x, topSide_y, rightSide_x, topSide_y, 0xffffff);
		screen->Line(leftSide_x, downSide_y, rightSide_x, downSide_y, 0xffffff);
		screen->Line(rightSide_x, downSide_y, rightSide_x, topSide_y, 0xffffff);
		screen->Line(leftSide_x, downSide_y, leftSide_x, topSide_y, 0xffffff);
	}

	void GameObject::DrawObject(Surface* screen) {}

	void GameObject::DeathAnimation() {}

	void GameObject::ResetGame() {}
}