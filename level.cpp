#include "precomp.h"
#include "level.h"

namespace Tmpl8
{
	Level::Level()
	{
		m_screen_position_x = 0;
		m_screen_position_y = 0;
	}

	Level::~Level() {}

	void Level::MapManagement(Surface* screen, Surface* theTiles[900], Surface* pOneTile, float deltaTime, int levelNumber)
	{
		for (int row = 0; row < 19; row++)
		{
			for (int col = 0; col < 26; col++)
			{
				int tileToDraw;

				switch (levelNumber)
				{
				case 1:
					tileToDraw = this->level_1_1_grid[row][col + (m_screen_position_x / 32)];
					break;

				case 2:
					tileToDraw = this->level_1_2_grid[row][col + (m_screen_position_x / 32)];
					break;

				case 3:
					tileToDraw = this->level_1_3_grid[row][col + (m_screen_position_x / 32)];
					break;

				case 4:
					tileToDraw = this->pipe_section_level_1_1_grid[row][col + (m_screen_position_x / 32)];
					break;
				}

				if (tileToDraw != 28)
				{
					theTiles[tileToDraw]->CopyTo(screen, col * pOneTile->GetWidth() - (m_screen_position_x % 32), row * pOneTile->GetHeight());
				}
			}
		}

		m_start_map_speed = 0;
		m_map_speed = 0.15f * deltaTime * 1.5f;
	}

	void Level::ResetMap()
	{
		for (int row = 0; row < 19; row++)
		{
			for (int col = 0; col < 224; col++)
			{
				level_1_1_grid[row][col] = start_level_1_1_grid[row][col];
				level_1_2_grid[row][col] = start_level_1_2_grid[row][col];
				level_1_3_grid[row][col] = start_level_1_3_grid[row][col];
			}
		}

		for (int row = 0; row < 19; row++)
		{
			for (int col = 0; col < 26; col++)
			{
				pipe_section_level_1_1_grid[row][col] = start_pipe_section_level_1_1_grid[row][col];
			}
		}

		this->m_map_position_x = 0;

		m_screen_position_x = 0;
		m_screen_position_y = 0;
	}

	void Level::RemoveBridge()
	{
		for (int row = 0; row < 19; row++)
		{
			for (int col = 0; col < 224; col++)
			{
				int tile = level_1_3_grid[row][col];

				if (tile == 48)
				{
					level_1_3_grid[row][col] = 55;
				}
			}
		}
	}
}