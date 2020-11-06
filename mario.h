#pragma once
#include "gameobject.h"

namespace Tmpl8
{
	class Mario : public GameObject
	{
	public:
		Mario(Surface* screen);
		~Mario();
		virtual void Update(Surface* screen, float deltaTime, float m_map_speed);
		virtual bool CollisionDetection(float object_2_position_x, float object_2_position_y, int object_2_width, int object_2_height);
		void ActivePipeDetection(int x_coords[2]);
		virtual void GroundCollisionCheck(int grid[19][224]);
		void TopCollisionCheck(int grid[19][224]);
		virtual void HealthManagement();
		virtual void DrawObject(Surface* screen);
		void EndOfLevelCheck();
		void Jump();
		void Movement();
		void IdleLeft();
		void IdleRight();
		void MoveRight();
		void MoveLeft();
		void Dodge();
		void ShootFire();
		void StarTimer();
		void Grow();
		void Shrink();
		char* GetScore();
		char* GetCoins();
		char* GetLevel();
		char* GetLives();
		virtual void DeathAnimation();
		virtual void ResetGame();

		Sprite* m_pMarioDefaultSmall;
		bool m_showStartingMenu;
		int m_frame;
		float m_jumpForce;
		float m_smallJumpForce;
		const int m_END_LEVEL_X = 6816;
		bool m_isGrounded;
		enum playerWalkingState { WALK_LEFT, WALK_RIGHT, STOP_WALKING };
		enum playerIdleState { IDLE_LEFT, IDLE_RIGHT };
		enum playerJumpState { JUMP_LEFT, JUMP_RIGHT, STOP_JUMPING };
		int m_playerWalkState;
		int m_playerIdleState;
		int m_playerJumpState;
		bool m_down;
		bool m_gameWon;
		bool m_reachedEndOfLevel;
		bool m_atMiddleOfScreen;
		bool m_hitEnemy;
		bool m_hitByEnemy;
		bool m_fire;
		bool m_star;
		bool m_startLevel;
		bool m_resetGame;
		bool m_maximum_y_after_kill_reached;
		bool m_collisionActivated;
		int m_score;
		int m_coins;
		int m_lives;
		int m_level;
		bool m_movementActivated;
		bool m_inPipe;
		bool m_movementInPipeActivated;
		bool m_caveVisited;
		bool m_shoot;
		float m_fireBall_position_x;
		float m_fireBall_position_y;
		bool m_fireBall_collision_with_enemy;
		float m_direction;

	private:
		const float m_MAX_JUMPSPEED = 9.0f;
		bool m_starTimerOver;
		bool m_onActivePipe;
		float m_deathJumpForce;
		float m_normal_speed;
		float m_normal_map_speed;
		float m_map_x_speed;
		float m_deltaTime;
		float m_velocity;
		bool m_addFrame;
		bool m_endOfLevelTimerOver;
		int m_animation_interpreter;
		float m_frameTimer;
		float m_middleOfScreen_x;
		Sprite* m_pSelectedSprite;
		Sprite* m_pMarioDefaultBig;
		Sprite* m_pMarioFireBig;
		Sprite* m_pMarioStarBig;
		Sprite* m_pFireBallSprite;
	};
}

