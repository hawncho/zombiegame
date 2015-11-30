/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#ifndef GAME_H_
#define GAME_H_

#include <GL\glew.h>
#include <GL\glut.h>
#include <SFML\Audio.hpp>
#include <vector>
#include <random>

#include "Global.h"
#include "CollisionSystem.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "BulletProjectile.h"
#include "GrenadePickup.h"
#include "GrenadeProjectile.h"
#include "Explosion.h"
#include "GameLevel.h"
#include "ObjectPool.h"
#include "Crosshair.h"
#include "HUDGrenadeCount.h"
#include "TextRenderer.h"

#define GAME_SPEED							1.0

class Game
{
	public:
		Game();
		~Game();

		bool Init();

		void MainGameLoop(int worldMousePosX, int worldMousePosY);
		bool HandleKeyboardDown(unsigned char key, int x, int y);
		bool HandleKeyboardUp(unsigned char key, int x, int y);
		bool HandleMouse(int button, int state, int x, int y);
		bool HandleMovingMouse(int x, int y);
		bool HandlePassiveMouse(int x, int y);

		std::vector<Sprite*> GetSprites();
		std::vector<IHUDElement*> GetHUDElements();
		GameLevel* GetCurrentLevel();
		glm::dvec2 GetPlayerCenter();
		std::vector<glm::vec2> GetBulletPositions();
		std::vector<glm::vec3> GetExplosionData();
		std::vector<glm::vec4> GetPickupData();

	private:
		int m_CurrentTimeMS;
		double m_GameSpeed = GAME_SPEED;
		double m_CurrentTick;
		const double m_TickRate = TICK_RATE;
		const double m_SecondsPerTick = 1.0 / TICK_RATE;
		double m_AccumulatedTime;
		
		int m_StartTimeMS;
		int m_Score;

		bool m_IsGamePlaying;

		CollisionSystem m_CollisionSystem;

		PlayerCharacter m_Player;

		Crosshair m_Crosshair;
		HUDGrenadeCount m_HUDGrenadeCount;

		ObjectPool<BulletProjectile>* m_BulletPool;
		std::vector<BulletProjectile*> m_Bullets;

		ObjectPool<GrenadePickup>* m_GrenadePickupPool;
		std::vector<GrenadePickup*> m_GrenadePickups;

		ObjectPool<GrenadeProjectile>* m_GrenadePool;
		std::vector<GrenadeProjectile*> m_Grenades;
		ObjectPool<Explosion>* m_ExplosionPool;
		std::vector<Explosion*> m_Explosions;

		ObjectPool<EnemyCharacter>* m_EnemyPool;
		std::vector<EnemyCharacter*> m_Enemies;
		int m_CurrentTotalEnemyCount;
		double m_LastSpawnIncreaseTick;

		GameLevel* m_pGameLevel;
		double m_GameLevelWidth;
		double m_GameLevelHeight;

		sf::Music m_BackgroundMusic;
		sf::SoundBuffer m_SoundBuffers[7];
		sf::Sound m_Sounds[11];
		double m_PlayerFootstepSoundLastTick;

		TextRenderer m_TextRenderer;

		bool LoadAudio();

		int GetCurrentTimeInMilliseconds();
		void UpdatePlayer(int worldMousePosX, int worldMousePosY);

		template <typename T> 
			void UpdateObjects(std::vector<T*>& activeObjects, ObjectPool<T>* objectPool, Game* context = nullptr, void(Game::*additionalUpdate)(T*) = nullptr);
		template <typename T> 
			void RemoveAllInactiveObjects(std::vector<T*>& activeObjects);

		template <typename T> 
			void AddSpritesToList(std::vector<Sprite*>& spriteList, std::vector<T*>& objectList, bool(*condition)(T*) = nullptr);

		void ExplodeGrenade(GrenadeProjectile* grenade);

		void UpdateEnemySpawns();
		void UpdateEnemyTargetPoint(EnemyCharacter* enemy);
		void CleanUpLostEnemies();

		void KillEnemy(EnemyCharacter* enemy);

		template <typename ObjectWithCollider, typename ColliderType> 
			void SpawnObjectAtRandomLocation(std::vector<ObjectWithCollider*>& activeObjects, ObjectPool<ObjectWithCollider>* objectPool);
		void SpawnEnemy(glm::dvec2 location);
		void SpawnBullet(glm::dvec2 centerLocation, glm::dvec2 direction);
		void SpawnGrenade(glm::dvec2 centerLocation, glm::dvec2 targetPoint);
		void SpawnExplosion(glm::dvec2 centerLocation, int damageValue);
		template <typename T>
			void SpawnObject(glm::dvec2 location, std::vector<T*>& activeObjects, ObjectPool<T>* objectPool);

		void CheckForCollisions();

		bool IsWithinSpawnRange(glm::dvec2 position);
		template <typename ObjectWithCollider, typename ColliderType>
			bool IsValidPosition(ObjectWithCollider* object);
		template <typename ObjectWithCollider, typename ColliderType>
			bool IsValidSpawnPosition(ObjectWithCollider* object);
		template <typename ColliderType> 
			bool IsInsideWall(ColliderType* collider);

		void UpdateScore(int amount);
		void UpdateScoreDisplay();
		bool IsObjectiveComplete();

		void ResetGame();
		void EndGame(bool isWinner);
		std::string GetTimeSurvivedString();
		void DisplayWinningResults();
		void DisplayLosingResults();
		void ResetHUD();
};

#endif /* GAME_H_ */