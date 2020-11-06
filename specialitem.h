#pragma once
#include "gameobject.h"
#include "mario.h"

namespace Tmpl8
{
	class SpecialItem : public GameObject
	{
	public:
		SpecialItem();
		SpecialItem(int type, int position_in_map_x, int position_in_map_y, int level);
		~SpecialItem();
		void Spawn(Mario* playerObject);
		virtual void Update(Surface* screen, float deltaTime, float map_position_x, Mario* playerObject, float map_speed_x);
		virtual void GroundCollisionCheck(int grid[19][224]);
		virtual void HorizontalCollisionCheck(int grid[19][224]);
		virtual void Movement(float deltaTime);
		void DrawWhenSpawning(Surface* screen);
		void DrawWhenMoving(Surface* screen);
		void TimerUntilDisappearing(float time);
		virtual void ResetGame();

		bool m_isMoving;

	private:
		Sprite* m_pSelectedSprite;
		Sprite* m_pMushroomSprite;
		Sprite* m_pCoinSprite;
		Sprite* m_pFlowerSprite;
		Sprite* m_pStarSprite;
		Sprite* m_pTransparentSprite;
		int m_itemType;
		int m_startItemType;
		int m_start_y_when_spawned;
		enum types { MUSHROOM, COIN, FLOWER, STAR, INVISIBLE };
		bool m_spawn;
		bool m_collidableWithPlayer;
		bool m_collidedWithPlayer;
		bool m_timerOver;
		bool m_lootGiven;
		float m_timer;
		// Sprite* m_star;
		// Sprite* m_flower;
	};
}
