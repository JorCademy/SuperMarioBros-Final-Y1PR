#include "precomp.h" // include this first in every .cpp file
#include "game.h"
#include "gameobject.h"
#include "monster.h"
#include "goomba.h"
#include "turtle.h"
#include "specialitem.h"
#include "bowser.h"
#include "axe.h"

namespace Tmpl8
{
	void Game::Init()
	{
		pLevelReference = new Level();
		princessPeach_position_x = 7040;
		princessPeach_map_position_x = 7040;
		princessPeach_start_position_x = princessPeach_map_position_x;
		pPrincessPeachSprite = new Sprite(new Surface("assets/princess-peach-sprite.png"), 1);
		showFinalMessage = false;
		temp_screen_position_x = 0;
		pMushroom_level_1 = new SpecialItem(MUSHROOM, mushroomPositions_x[0], mushroomPositions_y[0], 1);
		pMushroom_level_2 = new SpecialItem(MUSHROOM, mushroomPositions_x[1], mushroomPositions_y[1], 2);
		timer = 0;
		smbLogo = new Sprite(new Surface("assets/super-mario-bros-logo.png"), 1);
		coin = new Sprite(new Surface("assets/coin.png"), 1);
		returnPressed = false;
		pSuperMarioFont = new Font("assets/super-mario-bros-font-white.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-©");
		GameOver = false;
		pMario = new Mario(screen);
		int amountOfTilesOnSheet = 32 * 28;
		pLevel_1_1_sheet = new Surface("assets/mario-world-sheet-v3.png");
		pulledTile = new Surface(16, 16);
		int x_sheet = 0;
		int y_sheet = 0;

		for (int i = 0; i < amountOfTilesOnSheet; i++)
		{
			if (x_sheet == 528)
			{
				x_sheet = 0;
				y_sheet += 16;
			}

			pOneTile = new Surface(32, 32);
			pLevel_1_1_sheet->CopyFromTo(x_sheet, y_sheet, pLevel_1_1_sheet->GetPitch(), pulledTile, 0, 0);

			Sprite* dummySprite = new Sprite(pulledTile, 1);
			dummySprite->DrawScaled(0, 0, 32, 32, pOneTile);

			theTiles[i] = pOneTile;

			x_sheet += 16;
		}

		pBowser = new Bowser(6752.0f, 3);

		for (int i = 0; i < 16; i++)
		{
			goomba_level_1[i] = new Goomba(goombaPositions_x[i], 1);
		}
		for (int i = 0; i < 12; i++)
		{
			goomba_level_2[i] = new Goomba(goombaPositions_x[i + 16], 2);
		}
		for (int i = 0; i < 5; i++)
		{
			goomba_level_3[i] = new Goomba(goombaPositions_x[i + 28], 3);
		}

		turtle_level_1[0] = new Turtle(turtlePositions_x[0], 1);
		for (int i = 0; i < 2; i++)
		{
			turtle_level_2[i] = new Turtle(turtlePositions_x[i + 1], 2);
		}
		for (int i = 0; i < 4; i++)
		{
			turtle_level_3[i] = new Turtle(turtlePositions_x[i + 3], 3);
		}

		for (int i = 0; i < 10; i++)
		{
			coins_level_1[i] = new SpecialItem(COIN, coinPositions_x[i], coinPositions_y[i], 1);
		}
		for (int i = 0; i < 18; i++)
		{
			coins_level_2[i] = new SpecialItem(COIN, coinPositions_x[i + 10], coinPositions_y[i + 10], 2);
		}
		for (int i = 0; i < 3; i++)
		{
			coins_level_4[i] = new SpecialItem(COIN, coinPositions_x[i + 28], coinPositions_y[i + 28], 4);
		}

		for (int i = 0; i < 2; i++)
		{
			flowers_level_1[i] = new SpecialItem(FLOWER, flowerPositions_x[i], flowerPositions_y[i], 1);
		}
		for (int i = 0; i < 1; i++)
		{
			flowers_level_2[i] = new SpecialItem(FLOWER, flowerPositions_x[i + 2], flowerPositions_y[i + 2], 2);
		}

		pStarObject = new SpecialItem(STAR, starPositions_x[0], starPositions_y[0], 2);

		pAxeObject = new Axe(6816.0f, 3);

		pLevelReference->m_map_position_x = 0;
	}

	void Game::Shutdown()
	{
		delete pMario;
	}

	void Game::KeyDown(int key)
	{
		switch (key)
		{
		case 80: // left
			if (pMario->m_rightSideCollision)
			{
				pMario->m_rightSideCollision = false;
			}

			pMario->m_playerWalkState = pMario->WALK_LEFT;
			pMario->m_playerIdleState = pMario->IDLE_LEFT;
			break;

		case 79: // right
			if (pMario->m_leftSideCollision)
			{
				pMario->m_leftSideCollision = false;
			}

			pMario->m_playerWalkState = pMario->WALK_RIGHT;
			pMario->m_playerIdleState = pMario->IDLE_RIGHT;
			break;

		case 82: // up
			if (pMario->m_isGrounded)
			{
				pMario->m_jumpForce = -18.0f;

				if (pMario->m_playerIdleState == pMario->IDLE_LEFT)
				{
					pMario->m_playerJumpState = pMario->JUMP_LEFT;
				}
				else
				{
					pMario->m_playerJumpState = pMario->JUMP_RIGHT;
				}
			}
			break;

		case 44: // space
			pMario->m_shoot = true;

		case 81: // down
			if (pMario->m_isGrounded && pMario->m_level == 1)
			{
				pMario->m_down = true;
			}
			break;

		case 40: // return
			returnPressed = true;
		}
	}

	void Game::KeyUp(int key)
	{
		switch (key)
		{
		case 80: // left
			pMario->m_playerIdleState = pMario->IDLE_LEFT;
			pMario->m_playerWalkState = pMario->STOP_WALKING;
			break;

		case 79: // right
			pMario->m_playerIdleState = pMario->IDLE_RIGHT;
			pMario->m_playerWalkState = pMario->STOP_WALKING;
			break;

		case 81: // down
			pMario->m_down = false;
		}

		if (pMario->m_isGrounded)
		{
			switch (key)
			{
			case 80: // left
				pMario->m_playerJumpState = pMario->STOP_JUMPING;
				break;

			case 79: // right
				pMario->m_playerJumpState = pMario->STOP_JUMPING;
				break;

			case 82: // up
				pMario->m_playerJumpState = pMario->STOP_JUMPING;
				break;
			}
		}
	}

	void Game::GroundCollisionCheck()
	{
		switch (pMario->m_level)
		{
		case 1:
			pMario->GroundCollisionCheck(pLevelReference->level_1_1_grid);
			break;

		case 2:
			pMario->GroundCollisionCheck(pLevelReference->level_1_2_grid);
			break;

		case 3:
			pMario->GroundCollisionCheck(pLevelReference->level_1_3_grid);
			break;

		case 4:
			pMario->GroundCollisionCheck(pLevelReference->pipe_section_level_1_1_grid);
			break;
		}

		if (pBowser->m_startMoving && pBowser->m_position_in_map_x >= 50)
		{
			pBowser->GroundCollisionCheck(pLevelReference->level_1_3_grid);
		}

		for (int i = 0; i < 16; i++)
		{
			if (goomba_level_1[i]->m_startMoving && goomba_level_1[i]->m_position_in_map_x >= 50)
			{
				goomba_level_1[i]->GroundCollisionCheck(pLevelReference->level_1_1_grid);
			}
		}
		for (int i = 0; i < 12; i++)
		{
			if (goomba_level_2[i]->m_startMoving && goomba_level_2[i]->m_position_in_map_x >= 50)
			{
				goomba_level_2[i]->GroundCollisionCheck(pLevelReference->level_1_2_grid);
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (goomba_level_3[i]->m_startMoving && goomba_level_3[i]->m_position_in_map_x >= 50)
			{
				goomba_level_3[i]->GroundCollisionCheck(pLevelReference->level_1_3_grid);
			}
		}

		for (int i = 0; i < 1; i++)
		{
			if (turtle_level_1[i]->m_startMoving && turtle_level_1[i]->m_position_in_map_x >= 50)
			{
				turtle_level_1[i]->GroundCollisionCheck(pLevelReference->level_1_1_grid);
			}
		}
		for (int i = 0; i < 2; i++)
		{
			if (turtle_level_2[i]->m_startMoving && turtle_level_2[i]->m_position_in_map_x >= 50)
			{
				turtle_level_2[i]->GroundCollisionCheck(pLevelReference->level_1_2_grid);
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (turtle_level_3[i]->m_startMoving && turtle_level_3[i]->m_position_in_map_x >= 50)
			{
				turtle_level_3[i]->GroundCollisionCheck(pLevelReference->level_1_3_grid);
			}
		}

		if (pMushroom_level_1->m_isMoving && pMushroom_level_1->m_position_in_map_x >= 50)
		{
			pMushroom_level_1->GroundCollisionCheck(pLevelReference->level_1_1_grid);
		}
		if (pMushroom_level_2->m_isMoving && pMushroom_level_2->m_position_in_map_x >= 50)
		{
			pMushroom_level_2->GroundCollisionCheck(pLevelReference->level_1_2_grid);
		}
	}

	void Game::HorizontalCollisionCheck()
	{
		switch (pMario->m_level)
		{
		case 1:
			pMario->HorizontalCollisionCheck(pLevelReference->level_1_1_grid);
			break;

		case 2:
			pMario->HorizontalCollisionCheck(pLevelReference->level_1_2_grid);
			break;

		case 3:
			pMario->HorizontalCollisionCheck(pLevelReference->level_1_3_grid);
			break;

		case 4:
			pMario->HorizontalCollisionCheck(pLevelReference->pipe_section_level_1_1_grid);
			break;
		}

		if (pBowser->m_startMoving)
		{
			pBowser->HorizontalCollisionCheck(pLevelReference->level_1_3_grid);
		}

		for (int i = 0; i < 16; i++)
		{
			if (goomba_level_1[i]->m_startMoving)
			{
				goomba_level_1[i]->HorizontalCollisionCheck(pLevelReference->level_1_1_grid);

				if (goomba_level_1[i]->m_rightSide_x > turtle_level_1[0]->m_leftSide_x && goomba_level_1[i]->m_rightSide_x < turtle_level_1[0]->m_rightSide_x)
				{
					goomba_level_1[i]->m_topSideCollision = true;
				}
			}
		}
		for (int i = 0; i < 12; i++)
		{
			if (goomba_level_2[i]->m_startMoving)
			{
				goomba_level_2[i]->HorizontalCollisionCheck(pLevelReference->level_1_2_grid);

				if ((goomba_level_2[i]->m_rightSide_x > turtle_level_2[0]->m_leftSide_x && goomba_level_2[i]->m_rightSide_x < turtle_level_2[0]->m_rightSide_x) ||
					(goomba_level_2[i]->m_rightSide_x > turtle_level_2[1]->m_leftSide_x && goomba_level_2[i]->m_rightSide_x < turtle_level_2[1]->m_rightSide_x))
				{
					goomba_level_2[i]->m_topSideCollision = true;
				}
			}
		}
		for (int i = 0; i < 5; i++)
		{
			if (goomba_level_3[i]->m_startMoving)
			{
				goomba_level_3[i]->HorizontalCollisionCheck(pLevelReference->level_1_3_grid);

				if ((goomba_level_3[i]->m_rightSide_x > turtle_level_3[0]->m_leftSide_x && goomba_level_3[i]->m_rightSide_x < turtle_level_3[0]->m_rightSide_x) ||
					(goomba_level_3[i]->m_rightSide_x > turtle_level_3[1]->m_leftSide_x && goomba_level_3[i]->m_rightSide_x < turtle_level_3[1]->m_rightSide_x) ||
					(goomba_level_3[i]->m_rightSide_x > turtle_level_3[2]->m_leftSide_x && goomba_level_3[i]->m_rightSide_x < turtle_level_3[2]->m_rightSide_x) ||
					(goomba_level_3[i]->m_rightSide_x > turtle_level_3[3]->m_leftSide_x && goomba_level_3[i]->m_rightSide_x < turtle_level_3[3]->m_rightSide_x))
				{
					goomba_level_2[i]->m_topSideCollision = true;
				}
			}
		}

		if (turtle_level_1[0]->m_startMoving)
		{
			turtle_level_1[0]->HorizontalCollisionCheck(pLevelReference->level_1_1_grid);
		}
		for (int i = 0; i < 2; i++)
		{
			if (turtle_level_2[i]->m_startMoving)
			{
				turtle_level_2[i]->HorizontalCollisionCheck(pLevelReference->level_1_2_grid);
			}
		}
		for (int i = 0; i < 4; i++)
		{
			if (turtle_level_3[i]->m_startMoving)
			{
				turtle_level_3[i]->HorizontalCollisionCheck(pLevelReference->level_1_3_grid);
			}
		}

		if (pMushroom_level_1->m_isMoving)
		{
			pMushroom_level_1->HorizontalCollisionCheck(pLevelReference->level_1_1_grid);
		}
		if (pMushroom_level_2->m_isMoving)
		{
			pMushroom_level_2->HorizontalCollisionCheck(pLevelReference->level_1_2_grid);
		}
	}

	bool Game::SetTimer(float time)
	{
		timer++;

		if (timer >= time)
		{
			timer = 0;

			return true;
		}

		return false;
	}

	void Game::ShowStartingMenu(float deltaTime)
	{
		ResetGame();

		screen->Clear(0x9492FF);

		for (int row = 0; row < 16; row++)
		{
			for (int col = 0; col < 26; col++)
			{
				int tileToDraw = starting_menu_grid[row][col];

				if (tileToDraw != 28)
				{
					theTiles[tileToDraw]->CopyTo(screen, col * pOneTile->GetWidth(), row * pOneTile->GetHeight());
				}
			}
		}

		pMario->DrawSprite(screen, pMario->m_pMarioDefaultSmall, 50, 337, 7);
		smbLogo->Draw(screen, 190, 90);

		pSuperMarioFont->Print(screen, "©2020    NICK JORDAN", 255, 290, true);
		pSuperMarioFont->Print(screen, "1    PLAYER    GAME", 275, 350, true);

		if (returnPressed)
		{
			pMario->m_showStartingMenu = false;

			returnPressed = false;
		}
	}

	void Game::ShowGameOverScreen(float deltaTime)
	{
		screen->Clear(0x000000);

		pSuperMarioFont->Print(screen, "GAME    OVER", 304, 250, true);

		pMario->m_showStartingMenu = SetTimer(250.0f);

		if (pMario->m_showStartingMenu)
		{
			this->ResetGame();

			GameOver = false;
		}
	}

	void Game::ShowLivesAndWorldScreen()
	{
		ResetGame();

		screen->Clear(0x000000);

		pSuperMarioFont->Print(screen, "WORLD", 300, 200, true);

		pSuperMarioFont->Print(screen, "1 - ", 430, 200, true);
		pSuperMarioFont->Print(screen, pMario->GetLevel(), 478, 200, true);

		pMario->DrawSprite(screen, pMario->m_pMarioDefaultSmall, 300, 250, 7);

		pSuperMarioFont->Print(screen, "X", 390, 300, true);

		pSuperMarioFont->Print(screen, pMario->GetLives(), 440, 300, true);

		pMario->m_startLevel = SetTimer(250.0f);
	}

	void Game::ShowFinalMessage()
	{
		pSuperMarioFont->Print(screen, "THANK  YOU  MARIO", 80, 160, true);
		pSuperMarioFont->Print(screen, "YOUR QUEST IS OVER", 65, 200, true);
	}

	void Game::UpdateGame(float deltaTime)
	{
		screen->Clear(0x9492FF);

		pLevelReference->m_screen_position_y = 0;

		pMushroom_level_1->DrawWhenSpawning(screen);
		pMushroom_level_2->DrawWhenSpawning(screen);

		for (int i = 0; i < 2; i++)
		{
			flowers_level_1[i]->DrawWhenSpawning(screen);
		}
		flowers_level_2[0]->DrawWhenSpawning(screen);

		pStarObject->DrawWhenSpawning(screen);

		if (pMario->m_down && !pMario->m_caveVisited)
		{
			if (pMario->m_position_in_map_x >= pipe_position_in_x[0] && pMario->m_position_in_map_x <= pipe_position_in_x[1])
			{
				pMario->m_inPipe = true;
			}
		}

		if (pMario->m_inPipe)
		{
			pMario->m_level = 4;

			if (!pMario->m_movementInPipeActivated)
			{
				pMario->m_position_in_map_x = pMario->m_start_x;

				pMario->m_movementInPipeActivated = true;
			}

			pLevelReference->m_screen_position_x = 0;
		}
		else
		{
			temp_screen_position_x = pLevelReference->m_screen_position_x;
		}

		if (pMario->m_level == 4)
		{
			pMario->m_caveVisited = true;

			if (pMario->m_position_x >= 702)
			{
				pMario->m_position_in_map_x = 1782;
				pLevelReference->m_screen_position_x = temp_screen_position_x;
				pMario->m_position_y = 256;
				pMario->m_inPipe = false;
				pMario->m_level = 1;
			}
		}

		if (pMario->m_lives == 0)
		{
			pMario->m_showStartingMenu = false;

			pMario->m_startLevel = false;

			GameOver = true;
		}

		if (pMario->m_resetGame)
		{
			ResetGame();
		}

		if (!pMario->m_killed)
		{
			princessPeach_position_x = princessPeach_map_position_x - pLevelReference->m_screen_position_x;

			if (pBowser->m_levelToBePresent == pMario->m_level)
			{
				pBowser->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
				pBowser->m_position_x = pBowser->m_position_in_map_x - pLevelReference->m_screen_position_x;
			}

			for (int i = 0; i < 16; i++)
			{
				if (goomba_level_1[i]->m_levelToBePresent == pMario->m_level)
				{
					goomba_level_1[i]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
					goomba_level_1[i]->m_position_x = goomba_level_1[i]->m_position_in_map_x - pLevelReference->m_screen_position_x;
				}
			}
			for (int i = 0; i < 12; i++)
			{
				if (goomba_level_2[i]->m_levelToBePresent == pMario->m_level)
				{
					goomba_level_2[i]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
					goomba_level_2[i]->m_position_x = goomba_level_2[i]->m_position_in_map_x - pLevelReference->m_screen_position_x;
				}
			}
			for (int i = 0; i < 5; i++)
			{
				if (goomba_level_3[i]->m_levelToBePresent == pMario->m_level)
				{
					goomba_level_3[i]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
					goomba_level_3[i]->m_position_x = goomba_level_3[i]->m_position_in_map_x - pLevelReference->m_screen_position_x;
				}
			}

			if (turtle_level_1[0]->m_levelToBePresent == pMario->m_level)
			{
				turtle_level_1[0]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
				turtle_level_1[0]->m_position_x = turtle_level_1[0]->m_position_in_map_x - pLevelReference->m_screen_position_x;
			}
			for (int i = 0; i < 2; i++)
			{
				if (turtle_level_2[i]->m_levelToBePresent == pMario->m_level)
				{
					turtle_level_2[i]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
					turtle_level_2[i]->m_position_x = turtle_level_2[i]->m_position_in_map_x - pLevelReference->m_screen_position_x;
				}
			}
			for (int i = 0; i < 4; i++)
			{
				if (turtle_level_3[i]->m_levelToBePresent == pMario->m_level)
				{
					turtle_level_3[i]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
					turtle_level_3[i]->m_position_x = turtle_level_3[i]->m_position_in_map_x - pLevelReference->m_screen_position_x;
				}
			}

			if (pMushroom_level_1->m_levelToBePresent == pMario->m_level)
			{
				pMushroom_level_1->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
				pMushroom_level_1->m_position_x = pMushroom_level_1->m_position_in_map_x - pLevelReference->m_screen_position_x;
			}
			if (pMushroom_level_2->m_levelToBePresent == pMario->m_level)
			{
				pMushroom_level_2->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
				pMushroom_level_2->m_position_x = pMushroom_level_2->m_position_in_map_x - pLevelReference->m_screen_position_x;
			}

			for (int i = 0; i < 10; i++)
			{
				if (coins_level_1[i]->m_levelToBePresent == pMario->m_level)
				{
					coins_level_1[i]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
					coins_level_1[i]->m_position_x = coins_level_1[i]->m_position_in_map_x - pLevelReference->m_screen_position_x;
				}
			}
			for (int i = 0; i < 18; i++)
			{
				if (coins_level_2[i]->m_levelToBePresent == pMario->m_level)
				{
					coins_level_2[i]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
					coins_level_2[i]->m_position_x = coins_level_2[i]->m_position_in_map_x - pLevelReference->m_screen_position_x;
				}
			}
			for (int i = 0; i < 3; i++)
			{
				if (coins_level_4[i]->m_levelToBePresent == pMario->m_level)
				{
					coins_level_4[i]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
					coins_level_4[i]->m_position_x = coins_level_4[i]->m_position_in_map_x - pLevelReference->m_screen_position_x;
				}
			}

			for (int i = 0; i < 2; i++)
			{
				if (flowers_level_1[i]->m_levelToBePresent == pMario->m_level)
				{
					flowers_level_1[i]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
					flowers_level_1[i]->m_position_x = flowers_level_1[i]->m_position_in_map_x - pLevelReference->m_screen_position_x;
				}
			}
			if (flowers_level_2[0]->m_levelToBePresent == pMario->m_level)
			{
				flowers_level_2[0]->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
				flowers_level_2[0]->m_position_x = flowers_level_2[0]->m_position_in_map_x - pLevelReference->m_screen_position_x;
			}

			if (pStarObject->m_levelToBePresent == pMario->m_level)
			{
				pStarObject->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
				pStarObject->m_position_x = pStarObject->m_position_in_map_x - pLevelReference->m_screen_position_x;
			}

			if (pAxeObject->m_levelToBePresent == pMario->m_level)
			{
				pAxeObject->Update(screen, deltaTime, pLevelReference->m_map_position_x, pMario, pLevelReference->m_map_speed);
				pAxeObject->m_position_x = pAxeObject->m_position_in_map_x - pLevelReference->m_screen_position_x;
			}
		}

		if (pMario->m_playerWalkState == pMario->WALK_RIGHT && pMario->m_position_x >= (SCRWIDTH / 2) && pMario->m_position_in_map_x <= END_LEVEL_1_1_X && !pMario->m_inPipe)
		{
			pLevelReference->m_screen_position_x = (int)pMario->m_position_in_map_x - (SCRWIDTH / 2);
			if (pLevelReference->m_screen_position_x < 0) pLevelReference->m_screen_position_x = 0;

			pLevelReference->m_screen_position_y = (int)pMario->m_position_y - (SCRHEIGHT / 2);
			if (pLevelReference->m_screen_position_y < 0) pLevelReference->m_screen_position_y = 0;
		}

		pMario->m_position_x = pMario->m_position_in_map_x - pLevelReference->m_screen_position_x;

		if (pAxeObject->m_collisionWithPlayer)
		{
			pMario->m_frame = 7;
			pMario->m_movementActivated = false;
		}

		pMario->Update(screen, deltaTime, pLevelReference->m_map_speed);
		pMario->ActivePipeDetection(pipe_position_in_x);

		HorizontalCollisionCheck();

		switch (pMario->m_level)
		{
		case 1:
			pMario->TopCollisionCheck(pLevelReference->level_1_1_grid);
			break;

		case 2:
			pMario->TopCollisionCheck(pLevelReference->level_1_2_grid);
			break;

		case 3:
			pMario->TopCollisionCheck(pLevelReference->level_1_3_grid);
			break;

		case 4:
			pMario->TopCollisionCheck(pLevelReference->pipe_section_level_1_1_grid);
			break;
		}

		if (!GameOver)
		{
			GroundCollisionCheck();
		}
	}

	void Game::DrawGame(float deltaTime)
	{
		for (int i = 0; i < 2; i++)
		{
			flowers_level_1[i]->DrawWhenSpawning(screen);
		}
		flowers_level_2[0]->DrawWhenSpawning(screen);

		pLevelReference->MapManagement(screen, theTiles, pOneTile, deltaTime, pMario->m_level);

		if (pAxeObject->m_collisionWithPlayer)
		{
			pMario->m_frame = 7;
			pMario->m_movementActivated = false;

			pLevelReference->RemoveBridge();

			if (pBowser->m_killed)
			{
				showFinalMessage = !pBowser->SetTimer(500);

				if (showFinalMessage)
				{
					ShowFinalMessage();
				}
				else
				{
					pMario->m_gameWon = true;
				}
			}
		}

		pMario->DrawObject(screen);
		pBowser->DrawObject(screen);

		for (int i = 0; i < 16; i++)
		{
			if (pMario->m_level != 4)
			{
				goomba_level_1[i]->DrawObject(screen);
			}
		}
		for (int i = 0; i < 12; i++)
		{
			goomba_level_2[i]->DrawObject(screen);
		}
		for (int i = 0; i < 5; i++)
		{
			goomba_level_3[i]->DrawObject(screen);
		}

		turtle_level_1[0]->DrawObject(screen);
		if (turtle_level_1[0]->m_spawn && !turtle_level_1[0]->CheckOutOfFrame(turtle_level_1[0]->m_position_x))
		{
			turtle_level_1[0]->DrawObject(screen);
		}
		for (int i = 0; i < 2; i++)
		{
			turtle_level_2[i]->DrawObject(screen);
		}
		for (int i = 0; i < 4; i++)
		{
			turtle_level_3[i]->DrawObject(screen);
		}

		for (int i = 0; i < 10; i++)
		{
			coins_level_1[i]->DrawWhenSpawning(screen);
		}
		for (int i = 0; i < 18; i++)
		{
			coins_level_2[i]->DrawWhenSpawning(screen);
		}
		for (int i = 0; i < 3; i++)
		{
			coins_level_4[i]->DrawWhenSpawning(screen);
		}

		pStarObject->DrawWhenSpawning(screen);

		pMushroom_level_1->DrawWhenMoving(screen);
		pMushroom_level_2->DrawWhenMoving(screen);

		pAxeObject->DrawObject(screen, pMario);

		if (pMario->m_level == 3)
		{
			pPrincessPeachSprite->Draw(screen, princessPeach_position_x, 224);
		}
	}

	void Game::PlayGame(float deltaTime)
	{
		UpdateGame(deltaTime);

		DrawGame(deltaTime);
	}

	void Game::ResetGame()
	{
		pLevelReference->ResetMap();

		pMario->ResetGame();
		pBowser->ResetGame(pMario);

		for (int i = 0; i < 16; i++)
		{
			goomba_level_1[i]->ResetGame(pMario);
		}
		for (int i = 0; i < 12; i++)
		{
			goomba_level_2[i]->ResetGame(pMario);
		}
		for (int i = 0; i < 5; i++)
		{
			goomba_level_3[i]->ResetGame(pMario);
		}

		turtle_level_1[0]->ResetGame(pMario);
		for (int i = 0; i < 2; i++)
		{
			turtle_level_2[i]->ResetGame(pMario);
		}
		for (int i = 0; i < 4; i++)
		{
			turtle_level_3[i]->ResetGame(pMario);
		}

		pMushroom_level_1->ResetGame();
		pMushroom_level_2->ResetGame();

		for (int i = 0; i < 10; i++)
		{
			coins_level_1[i]->ResetGame();
		}
		for (int i = 0; i < 18; i++)
		{
			coins_level_2[i]->ResetGame();
		}
		for (int i = 0; i < 3; i++)
		{
			coins_level_4[i]->ResetGame();
		}

		for (int i = 0; i < 2; i++)
		{
			flowers_level_1[i]->ResetGame();
		}
		flowers_level_2[0]->ResetGame();

		pStarObject->ResetGame();
		pAxeObject->ResetGame();

		princessPeach_position_x = princessPeach_start_position_x;

		showFinalMessage = false;
	}

	void Game::Tick(float deltaTime)
	{
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				KeyDown(event.key.keysym.scancode);
			}
			else if (event.type == SDL_KEYUP)
			{
				KeyUp(event.key.keysym.scancode);
			}
		}

		if (pMario->m_showStartingMenu && GameOver == false && pMario->m_startLevel == false)
		{
			ShowStartingMenu(deltaTime);
			pMario->m_movementActivated = false;
		}
		else if (GameOver && pMario->m_showStartingMenu == false && pMario->m_startLevel == false)
		{
			ShowGameOverScreen(deltaTime);
			pMario->m_movementActivated = false;
		}
		else if (!GameOver && !pMario->m_showStartingMenu && pMario->m_startLevel)
		{
			PlayGame(deltaTime);
			pMario->m_movementActivated = true;
		}
		else if (!GameOver && !pMario->m_showStartingMenu && !pMario->m_startLevel)
		{
			ShowLivesAndWorldScreen();
			pMario->m_movementActivated = false;
		}

		coin->DrawScaled(245, 40, 30, 30, screen);
		pSuperMarioFont->Print(screen, "MARIO", 15, 10, true);
		pSuperMarioFont->Print(screen, pMario->GetScore(), 15, 40, true);
		pSuperMarioFont->Print(screen, ("X %", pMario->GetCoins()), 280, 40, true);
		pSuperMarioFont->Print(screen, "WORLD", 500, 10, true);
		pSuperMarioFont->Print(screen, "1  -  ", 520, 40, true);

		if (pMario->m_level == 4)
		{
			pSuperMarioFont->Print(screen, "1", 575, 40, true);
		}
		else
		{
			pSuperMarioFont->Print(screen, pMario->GetLevel(), 575, 40, true);
		}

		pSuperMarioFont->Print(screen, "L  -  ", 680, 10, true);
		pSuperMarioFont->Print(screen, pMario->GetLives(), 740, 10, true);
	}
};