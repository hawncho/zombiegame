/**
*
* Copyright (c) 2015 John Pham
*
* See the file license.txt for copying permission.
*
**/

#include <algorithm>

#include "Game.h"

#define STARTING_ENEMY_COUNT				5
#define MAX_ENEMY_COUNT						200
#define ENEMY_INCREASE_AMOUNT				5
#define ENEMY_INCREASE_TIME_SECONDS			8

#define MIN_SPAWN_DISTANCE					400.0
#define MAX_SPAWN_DISTANCE					1500.0
#define MAX_DISTANCE_FROM_PLAYER			2000.0

#define PICKUP_GRENADE_FREQ					1280 // 20 seconds

#define MAX_PICKUP_GRENADE_COUNT			3
#define MAX_GRENADE_COUNT					3
#define MAX_EXPLOSION_COUNT					3

#define SOUND_SHOTGUN						0
#define SOUND_PLAYER_FOOTSTEP				1
#define SOUND_PLAYER_DEATH					2
#define SOUND_EXPLOSION						3
#define SOUND_PICKUP_GRENADE				4
#define SOUND_GRENADE_THROW					5
#define SOUND_ENEMY_DEATH0					6
#define SOUND_ENEMY_DEATH1					7
#define SOUND_ENEMY_DEATH2					8
#define SOUND_ENEMY_DEATH3					9
#define SOUND_ENEMY_DEATH4					10

#define SOUND_PLAYER_FOOTSTEP_FREQ			24

#define PLAYER_START_POSX					896.0
#define PLAYER_START_POSY					2176.0

#define GRENADE_PICKUP_START_POSX			928.0
#define GRENADE_PICKUP_START_POSY			1824.0

#define OBJECTIVE_KILL_COUNT				100
#define OBJECTIVE_TEXT						"Kill 100 Zombies!"
#define OBJECTIVE_TEXT_DURATION				80

#define GOD_MODE							false

Game::Game()
{
	m_CurrentTick = 0.0;
	m_AccumulatedTime = 0.0;

	m_CurrentTotalEnemyCount = STARTING_ENEMY_COUNT;
	m_LastSpawnIncreaseTick = m_CurrentTick;

	m_PlayerFootstepSoundLastTick = m_CurrentTick;
}

Game::~Game()
{
	delete m_BulletPool;
	m_Bullets.clear();

	delete m_GrenadePickupPool;
	m_GrenadePickups.clear();

	delete m_GrenadePool;
	m_Grenades.clear();

	delete m_ExplosionPool;
	m_Explosions.clear();

	delete m_EnemyPool;
	m_Enemies.clear();
	
	delete m_pGameLevel;
	m_pGameLevel = nullptr;
}

bool Game::Init()
{
	m_BulletPool = new ObjectPool<BulletProjectile>(MAX_BULLET_COUNT, "BulletProjectile");

	m_GrenadePickupPool = new ObjectPool<GrenadePickup>(MAX_PICKUP_GRENADE_COUNT, "GrenadePickup");

	m_GrenadePool = new ObjectPool<GrenadeProjectile>(MAX_GRENADE_COUNT, "GrenadeProjectile");

	m_ExplosionPool = new ObjectPool<Explosion>(MAX_EXPLOSION_COUNT, "Explosion");

	m_EnemyPool = new ObjectPool<EnemyCharacter>(MAX_ENEMY_COUNT, "EnemyCharacter");

	int levelWidth = 78;
	int levelHeight = 37;
	const char* levelLayout = 
		"------------WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW--------------------------"
		"------------W                                      W--------------------------"
		"------------W                                      W--------------------------"
		"------------W                                      W--------------------------"
		"------------W    WWWWWWWWWWWWWWWWWWWWWWWWWWWWWW    W--------------------------"
		"------------W    W----------------------------W    W--------------------------"
		"------------W    W----------------------------W    W--------------------------"
		"WWWWWWWWWWWWW    WWWWWWWWWWWWW-----------WWWWWW    WWWWWW---------------------"
		"W                            W----------WW              WW--------------------"
		"W                            W---------WW                WW-------------------"
		"W                            WWWWWWWWWWW                  WW------------------"
		"W           WW  WW                          W  W  W  W     W------------------"
		"W    WW    WW    WW    WW                                  W-----WWWW---------"
		"W    WW    WW    WW    WW                                  WWWWWWW  WWWWWW----"
		"W                            WWWWWWWWWW     W  W  W  W  W                WW---"
		"W                            W--------W                                   WW--"
		"W                            W--------W                                    WW-"
		"W    W                       W--------W     W  W  W  W  W                   WW"
		"W     W                      W--------W                                      W"
		"W      W                W    W--------WW                                     W"
		"W       W              W     W---------WW      W  W  W  W                    W"
		"W                     W      W----------WW                                   W"
		"W                    W       W-----------WW                                  W"
		"W                            W------------WW               WWWWWWWWWW        W"
		"W                            W-------------WW              W--------W        W"
		"W                            W--------------W              W--------W        W"
		"W                            W--------------W              W---WW---W        W"
		"W          WW    WW          W--------------W              W---WW---W        W"
		"W          WW    WW          WWWWWWWWWWWWWWWW              W---WW---W        W"
		"W                                                          W--------W        W"
		"W                                                          W--------W        W"
		"W                                                          WWWWWWWWWW        W"
		"W                            WWWWWWWWWWWWWWWW                                W"
		"WWWWWWWWWWWWWW  WWWWWWWWWWWWWW--------------W                                W"
		"-------------W  W---------------------------W                                W"
		"-------------W  W---------------------------WWWWWWWWWWWWWW  WWWWWWWWWWWWWWWWWW"
		"-------------WWWW----------------------------------------WWWW-----------------";

	m_pGameLevel = new GameLevel(levelLayout, levelWidth, levelHeight);
	if (!m_pGameLevel->Init())
	{
		return false;
	}

	m_GameLevelWidth = (double)m_pGameLevel->GetWidthInPixels();
	m_GameLevelHeight = (double)m_pGameLevel->GetHeightInPixels();

	if (!LoadAudio())
	{
		return false;
	}

	if (!m_TextRenderer.Init())
	{
		fprintf(stderr, "Failed to initialize text renderer.\n");
		return false;
	}

	ResetGame();

	return true;
}

bool Game::LoadAudio()
{
	int soundIndex = SOUND_SHOTGUN;
	if (!m_SoundBuffers[soundIndex].loadFromFile("sound/shotgun.ogg"))
	{
		fprintf(stderr, "Failed to load audio: shotgun.ogg\n");
		return false;
	}
	m_Sounds[soundIndex].setBuffer(m_SoundBuffers[soundIndex]);

	soundIndex = SOUND_PLAYER_FOOTSTEP;
	if (!m_SoundBuffers[soundIndex].loadFromFile("sound/footstep.ogg"))
	{
		fprintf(stderr, "Failed to load audio: footstep.ogg\n");
		return false;
	}
	m_Sounds[soundIndex].setBuffer(m_SoundBuffers[soundIndex]);

	soundIndex = SOUND_PLAYER_DEATH;
	if (!m_SoundBuffers[soundIndex].loadFromFile("sound/player_death.ogg"))
	{
		fprintf(stderr, "Failed to load audio: player_death.ogg\n");
		return false;
	}
	m_Sounds[soundIndex].setBuffer(m_SoundBuffers[soundIndex]);

	soundIndex = SOUND_ENEMY_DEATH0;
	if (!m_SoundBuffers[soundIndex].loadFromFile("sound/zombie_death.ogg"))
	{
		fprintf(stderr, "Failed to load audio: zombie_death.ogg\n");
		return false;
	}
	m_Sounds[soundIndex + 0].setBuffer(m_SoundBuffers[soundIndex]);
	m_Sounds[soundIndex + 1].setBuffer(m_SoundBuffers[soundIndex]);
	m_Sounds[soundIndex + 2].setBuffer(m_SoundBuffers[soundIndex]);
	m_Sounds[soundIndex + 3].setBuffer(m_SoundBuffers[soundIndex]);
	m_Sounds[soundIndex + 4].setBuffer(m_SoundBuffers[soundIndex]);

	soundIndex = SOUND_EXPLOSION;
	if (!m_SoundBuffers[soundIndex].loadFromFile("sound/explosion.ogg"))
	{
		fprintf(stderr, "Failed to load audio : explosion.ogg\n");
		return false;
	}
	m_Sounds[soundIndex].setBuffer(m_SoundBuffers[soundIndex]);

	soundIndex = SOUND_PICKUP_GRENADE;
	if (!m_SoundBuffers[soundIndex].loadFromFile("sound/grenadepickup.ogg"))
	{
		fprintf(stderr, "Failed to load audio : grenadepickup.ogg\n");
		return false;
	}
	m_Sounds[soundIndex].setBuffer(m_SoundBuffers[soundIndex]);
	m_Sounds[soundIndex].setVolume(60.0f);

	soundIndex = SOUND_GRENADE_THROW;
	if (!m_SoundBuffers[soundIndex].loadFromFile("sound/grenadethrow.ogg"))
	{
		fprintf(stderr, "Failed to load audio : grenadethrow.ogg\n");
		return false;
	}
	m_Sounds[soundIndex].setBuffer(m_SoundBuffers[soundIndex]);

	if (!m_BackgroundMusic.openFromFile("sound/Breaching_the_Gates_Remastered.ogg"))
	{
		fprintf(stderr, "Failed to load audio: Breaching_the_Gates_Remastered.ogg\n");
		return false;
	}
	m_BackgroundMusic.setLoop(true);
	m_BackgroundMusic.play();

	return true;
}

void Game::ResetGame()
{
	// reset score
	m_StartTimeMS = GetCurrentTimeInMilliseconds();
	m_Score = 0;

	m_IsGamePlaying = true;

	// reset player
	m_Player.Reset();
	m_Player.SetPosition(PLAYER_START_POSX, PLAYER_START_POSY);
	m_Player.SetActive(true);

	// reset timer
	m_CurrentTimeMS = GetCurrentTimeInMilliseconds();

	// clean up enemies
	for (std::vector<EnemyCharacter*>::iterator enemyIter = m_Enemies.begin(); enemyIter != m_Enemies.end(); ++enemyIter)
	{
		EnemyCharacter* enemy = *enemyIter;
		enemy->Reset();
		enemy->SetActive(false);

		m_EnemyPool->RemoveObject(enemy);
	}
	RemoveAllInactiveObjects<EnemyCharacter>(m_Enemies);
	m_CurrentTotalEnemyCount = STARTING_ENEMY_COUNT;

	// clean up all grenade pickups
	for (std::vector<GrenadePickup*>::iterator pickupIter = m_GrenadePickups.begin(); pickupIter != m_GrenadePickups.end(); ++pickupIter)
	{
		(*pickupIter)->SetActive(false);

		m_GrenadePickupPool->RemoveObject(*pickupIter);
	}
	RemoveAllInactiveObjects<GrenadePickup>(m_GrenadePickups);

	// reset starting grenade
	SpawnObject<GrenadePickup>(glm::dvec2(GRENADE_PICKUP_START_POSX, GRENADE_PICKUP_START_POSY),
								m_GrenadePickups, m_GrenadePickupPool);

	// reset tick flags
	m_CurrentTick = 0;
	m_CurrentTick = 0.0;
	m_AccumulatedTime = 0.0;
	m_LastSpawnIncreaseTick = m_CurrentTick;
	m_PlayerFootstepSoundLastTick = m_CurrentTick;

	ResetHUD();

	m_TextRenderer.AddText(OBJECTIVE_TEXT, 136.0f, 200.0f);
}

void Game::ResetHUD()
{
	m_HUDGrenadeCount.SetGrenadeCount(m_Player.m_GrenadeCount);
	m_TextRenderer.ClearAllText();
	UpdateScoreDisplay();
}

void Game::MainGameLoop(int worldMousePosX, int worldMousePosY)
{
	if (m_IsGamePlaying)
	{
		int newTimeMS = GetCurrentTimeInMilliseconds();
		double frameTime = (double)(newTimeMS - m_CurrentTimeMS) / 1000.0;

		m_AccumulatedTime += frameTime;

		if (!m_Player.IsDead() && !IsObjectiveComplete())
		{
			if (m_CurrentTotalEnemyCount < MAX_ENEMY_COUNT &&
				m_CurrentTick - m_LastSpawnIncreaseTick >= ENEMY_INCREASE_TIME_SECONDS * TICK_RATE)
			{
				m_LastSpawnIncreaseTick = m_CurrentTick;
				m_CurrentTotalEnemyCount += ENEMY_INCREASE_AMOUNT;
			}
		}

		while (m_AccumulatedTime >= m_SecondsPerTick / GAME_SPEED)
		{
			m_CurrentTick++;

			int currentTickInteger = (int)m_CurrentTick;

			UpdatePlayer(worldMousePosX, worldMousePosY);
			UpdateObjects<BulletProjectile>(m_Bullets, m_BulletPool);
			UpdateObjects<GrenadePickup>(m_GrenadePickups, m_GrenadePickupPool);
			UpdateObjects<Explosion>(m_Explosions, m_ExplosionPool);
			UpdateObjects<GrenadeProjectile>(m_Grenades, m_GrenadePool, this, &Game::ExplodeGrenade);
			UpdateEnemySpawns();
			UpdateObjects<EnemyCharacter>(m_Enemies, m_EnemyPool, this, &Game::UpdateEnemyTargetPoint);

			CheckForCollisions();

			// every half-second, remove enemies that have been pushed outside of the level 
			// due to cheap multi-body collision resolving
			// also remove enemies that have strayed too far from the player
			if (currentTickInteger % ((int)m_TickRate / 2) == 0)
			{
				CleanUpLostEnemies();
			}

			if (currentTickInteger % PICKUP_GRENADE_FREQ == 0)
			{
				if (m_GrenadePickups.size() >= MAX_PICKUP_GRENADE_COUNT)
				{
					m_GrenadePickups[0]->SetActive(false);
					m_GrenadePickupPool->RemoveObject(m_GrenadePickups[0]);
					RemoveAllInactiveObjects<GrenadePickup>(m_GrenadePickups);
				}
				SpawnObjectAtRandomLocation<GrenadePickup, CollisionCircle>(m_GrenadePickups, m_GrenadePickupPool);
			}

			// remove objective text
			if (currentTickInteger == OBJECTIVE_TEXT_DURATION)
			{
				m_TextRenderer.ClearAllText();
				UpdateScoreDisplay();
			}

			m_AccumulatedTime -= m_SecondsPerTick / GAME_SPEED;
		}

		m_CurrentTimeMS = newTimeMS;
	}
}

int Game::GetCurrentTimeInMilliseconds()
{
	return glutGet(GLUT_ELAPSED_TIME);
}

void Game::UpdatePlayer(int worldMousePosX, int worldMousePosY)
{
	m_Player.UpdateWithMouse((int)m_CurrentTick, worldMousePosX, worldMousePosY);

	if (m_Player.IsThrowingGrenade() && m_Player.m_GrenadeCount > 0)
	{
		m_Sounds[SOUND_GRENADE_THROW].play();
		m_Player.PlayThrowAnimation();
		SpawnGrenade(m_Player.GetCenter(), glm::dvec2(worldMousePosX, worldMousePosY));
		m_Player.m_GrenadeCount--;
		m_HUDGrenadeCount.SetGrenadeCount(m_Player.m_GrenadeCount);
	}
	else if (m_Player.IsShooting())
	{
		m_Sounds[SOUND_SHOTGUN].play();

		glm::dvec2 playerFacingDirection = m_Player.GetFacingDirection();
		glm::dvec2 bulletSpawnPosition = m_Player.GetCenter() + ((m_Player.GetWidth() / 4.0) * playerFacingDirection);
		// spawn straight bullet
		SpawnBullet(bulletSpawnPosition, playerFacingDirection);

		glm::dvec2 bulletDirection;
		const double spreadAngleDegrees = 5.0;
		double spreadAngleRadians = glm::radians(spreadAngleDegrees);
		double cosAngle = cos(spreadAngleRadians);
		double sinAngle = sin(spreadAngleRadians);
		bulletDirection.x = (cosAngle * playerFacingDirection.x) - (sinAngle * playerFacingDirection.y);
		bulletDirection.y = (sinAngle * playerFacingDirection.x) + (cosAngle * playerFacingDirection.y);
		// spawn bullet traveling slightly to the right
		SpawnBullet(bulletSpawnPosition, bulletDirection);

		spreadAngleRadians = glm::radians(-spreadAngleDegrees);
		cosAngle = cos(spreadAngleRadians);
		sinAngle = sin(spreadAngleRadians);
		bulletDirection.x = (cosAngle * playerFacingDirection.x) - (sinAngle * playerFacingDirection.y);
		bulletDirection.y = (sinAngle * playerFacingDirection.x) + (cosAngle * playerFacingDirection.y);
		// spawn bullet traveling slightly to the left
		SpawnBullet(bulletSpawnPosition, bulletDirection);

		m_Player.PlayShootingAnimation();
	}

	if (m_Player.IsMoving())
	{
		if (m_CurrentTick - m_PlayerFootstepSoundLastTick > SOUND_PLAYER_FOOTSTEP_FREQ)
		{
			m_PlayerFootstepSoundLastTick = m_CurrentTick;

			m_Sounds[SOUND_PLAYER_FOOTSTEP].play();
		}
	}
}

template <typename T> 
void Game::UpdateObjects(std::vector<T*>& activeObjects, ObjectPool<T>* objectPool, Game* context, void(Game::*additionalUpdate)(T*))
{
	// update every object and delete it if it is no longer active
	for (std::vector<T*>::iterator it = activeObjects.begin(); it != activeObjects.end(); ++it)
	{
		T* object = (*it);

		object->Update((int)m_CurrentTick);

		// if any additional updates were provided for this object
		if (context != nullptr && additionalUpdate != nullptr)
		{
			(context->*additionalUpdate)(object);
		}

		if (!object->IsActive())
		{
			objectPool->RemoveObject(object);
		}
	}

	RemoveAllInactiveObjects(activeObjects);
}

void Game::ExplodeGrenade(GrenadeProjectile* grenade)
{
	if (grenade->HasReachedTarget())
	{
		m_Sounds[SOUND_EXPLOSION].play();
		SpawnExplosion(grenade->GetCenter(), grenade->GetDamageValue());
	}
}

void Game::UpdateEnemySpawns()
{
	int enemiesToSpawn = m_CurrentTotalEnemyCount - m_Enemies.size();

	if (enemiesToSpawn > 0)
	{
		for (int i = 0; i < enemiesToSpawn; ++i)
		{
			SpawnObjectAtRandomLocation<EnemyCharacter, CollisionCircle>(m_Enemies, m_EnemyPool);
		}
	}
}

void Game::UpdateEnemyTargetPoint(EnemyCharacter* enemy)
{
	if (!m_Player.IsDead() && !IsObjectiveComplete())
	{
		enemy->SetTargetPoint(m_Player.GetPosition());
	}
}

// using the erase-remove idiom
// supplying predicate function as a C++11 lambda expression
template <typename T>
void Game::RemoveAllInactiveObjects(std::vector<T*>& activeObjects)
{
	activeObjects.erase(
		std::remove_if(
			activeObjects.begin(),
			activeObjects.end(),
			[](GameObject* o) -> bool
				{ return !o->IsActive(); }
		),
		activeObjects.end());
}

void Game::CleanUpLostEnemies()
{
	for (std::vector<EnemyCharacter*>::iterator enemyIter = m_Enemies.begin(); enemyIter != m_Enemies.end(); ++enemyIter)
	{
		EnemyCharacter* enemy = *enemyIter;
		double distanceFromPlayer = glm::length(enemy->GetPosition() - m_Player.GetPosition());

		if (distanceFromPlayer > MAX_DISTANCE_FROM_PLAYER ||
			!m_pGameLevel->IsWithinLevel(enemy->GetPosition(), enemy->GetSize()))
		{
			enemy->SetActive(false);
		}
	}
}

void Game::SpawnBullet(glm::dvec2 centerLocation, glm::dvec2 direction)
{
	BulletProjectile* bullet = m_BulletPool->GetNewObject();
	bullet->SetActive(true);
	m_Bullets.push_back(bullet);
	bullet->CenterOnPosition(centerLocation);
	bullet->SetDirection(direction);
}

void Game::SpawnGrenade(glm::dvec2 centerLocation, glm::dvec2 targetPoint)
{
	GrenadeProjectile* grenade = m_GrenadePool->GetNewObject();
	grenade->SetActive(true);
	m_Grenades.push_back(grenade);
	grenade->CenterOnPosition(centerLocation);
	grenade->SetTargetPoint(targetPoint);
}

void Game::SpawnExplosion(glm::dvec2 centerLocation, int damageValue)
{
	Explosion* explosion = m_ExplosionPool->GetNewObject();
	explosion->SetActive(true);
	m_Explosions.push_back(explosion);
	explosion->CenterOnPosition(centerLocation);
	explosion->SetDamageValue(damageValue);
}

template <typename ObjectWithCollider, typename ColliderType>
void Game::SpawnObjectAtRandomLocation(std::vector<ObjectWithCollider*>& activeObjects, ObjectPool<ObjectWithCollider>* objectPool)
{
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_real_distribution<double> randomPosX(0.0, m_GameLevelWidth);
	std::uniform_real_distribution<double> randomPosY(0.0, m_GameLevelHeight);

	ObjectWithCollider* object = objectPool->GetNewObject();
	object->SetActive(true);
	object->SetPosition(randomPosX(engine), randomPosY(engine));

	while (!IsValidSpawnPosition<ObjectWithCollider, ColliderType>(object))
	{
		object->SetPosition(randomPosX(engine), randomPosY(engine));
	}

	activeObjects.push_back(object);
}

template <typename T>
void Game::SpawnObject(glm::dvec2 location, std::vector<T*>& activeObjects, ObjectPool<T>* objectPool)
{
	T* object = objectPool->GetNewObject();
	activeObjects.push_back(object);
	object->SetPosition(location.x, location.y);
	object->SetActive(true);
}

void Game::SpawnEnemy(glm::dvec2 location)
{
	EnemyCharacter* enemy = m_EnemyPool->GetNewObject();
	m_Enemies.push_back(enemy);
	enemy->CenterOnPosition(location);
	enemy->SetActive(true);
}

void Game::CheckForCollisions()
{
	std::vector<CollisionAABB>* wallCollisionObjects = m_pGameLevel->GetWallCollisionObjects();
	CollisionCircle* playerCollisionObject = m_Player.GetCollisionObject();
	double penetrationDepth;
	for (std::vector<CollisionAABB>::iterator it = wallCollisionObjects->begin(); it != wallCollisionObjects->end(); ++it)
	{
		// player to wall
		CollisionAABB wall = *it;
		penetrationDepth = m_CollisionSystem.CheckCollision(playerCollisionObject, &wall);
		if (penetrationDepth != 0.0)
		{
			m_Player.CollideWithWall(wall.CalculateNormal(m_Player.GetCenter()), penetrationDepth);
		}

		// wall to enemy
		for (std::vector<EnemyCharacter*>::iterator enemyIter = m_Enemies.begin(); enemyIter != m_Enemies.end(); ++enemyIter)
		{
			EnemyCharacter* enemy = *enemyIter;
			penetrationDepth = m_CollisionSystem.CheckCollision(&wall, enemy->GetCollisionObject());
			if (penetrationDepth != 0.0)
			{
				enemy->CollideWithWall(wall.CalculateNormal(enemy->GetCenter()), penetrationDepth);
			}
		}
	}

	// player to grenade pickups
	for (std::vector<GrenadePickup*>::iterator pickupIter = m_GrenadePickups.begin(); pickupIter != m_GrenadePickups.end(); ++pickupIter)
	{
		GrenadePickup* grenadePickup = (*pickupIter);
		if (m_CollisionSystem.CheckCollision(playerCollisionObject, grenadePickup->GetCollisionObject()) != 0.0)
		{
			m_Sounds[SOUND_PICKUP_GRENADE].play();
			grenadePickup->ActivateEffect(m_Player);
			grenadePickup->SetActive(false);
			m_HUDGrenadeCount.SetGrenadeCount(m_Player.m_GrenadeCount);
		}
	}

	for (std::vector<BulletProjectile*>::iterator pBullet = m_Bullets.begin(); pBullet != m_Bullets.end(); ++pBullet)
	{
		CollisionCircle* bulletCollisionObject = (*pBullet)->GetCollisionObject();
		// bullet to enemy
		for (std::vector<EnemyCharacter*>::iterator enemyIter = m_Enemies.begin(); enemyIter != m_Enemies.end(); ++enemyIter)
		{
			EnemyCharacter* enemy = *enemyIter;
			if (!enemy->IsDead() && m_CollisionSystem.CheckCollision(enemy->GetCollisionObject(), bulletCollisionObject))
			{
				IDamageable* damagable = &(*enemy);
				(*pBullet)->HitObject(damagable);

				if (enemy->IsDead())
				{
					KillEnemy(enemy);
				}
			}
		}

		// bullet to wall
		for (std::vector<CollisionAABB>::iterator it = wallCollisionObjects->begin(); it != wallCollisionObjects->end(); ++it)
		{
			CollisionAABB wall = *it;
			if (m_CollisionSystem.CheckCollision(bulletCollisionObject, &wall))
			{
				(*pBullet)->SetActive(false);
			}
		}
	}

	for (std::vector<EnemyCharacter*>::iterator enemyIter1 = m_Enemies.begin(); enemyIter1 != m_Enemies.end(); ++enemyIter1)
	{
		EnemyCharacter* enemy1 = *enemyIter1;

		if (enemy1->IsDead()) continue;

		CollisionCircle* enemyCollisionObject1 = enemy1->GetCollisionObject();
		
		// player to enemy
		if (!m_Player.IsDead() && !GOD_MODE)
		{
			penetrationDepth = m_CollisionSystem.CheckCollision(enemyCollisionObject1, playerCollisionObject);
			if (penetrationDepth != 0.0)
			{
				m_Player.TakeDamage(1);
				m_Sounds[SOUND_PLAYER_DEATH].play();

				EndGame(false);
			}
		}

		// explosion to enemy
		for (std::vector<Explosion*>::iterator explosionIter = m_Explosions.begin(); explosionIter != m_Explosions.end(); ++explosionIter)
		{
			Explosion* explosion = *explosionIter;
			if (m_CollisionSystem.CheckCollision(enemyCollisionObject1, explosion->GetCollisionObject()) != 0.0)
			{
				enemy1->TakeDamage(explosion->GetDamageValue());

				if (enemy1->IsDead())
				{
					KillEnemy(enemy1);
				}
			}
		}

		// enemy to enemy
		for (std::vector<EnemyCharacter*>::iterator enemyIter2 = m_Enemies.begin(); enemyIter2 != m_Enemies.end(); ++enemyIter2)
		{
			EnemyCharacter* enemy2 = *enemyIter2;

			if (enemy1 == enemy2) continue;
			if (enemy2->IsDead()) continue;

			CollisionCircle* enemyCollisionObject2 = enemy2->GetCollisionObject();

			penetrationDepth = m_CollisionSystem.CheckCollision(enemyCollisionObject1, enemyCollisionObject2);
			if (penetrationDepth != 0.0)
			{
				glm::dvec2 originalPos = enemy1->GetPosition();
				enemy1->CollideWithWall(enemyCollisionObject2->CalculateNormal(enemy1->GetCenter()), penetrationDepth);

				if (!IsValidPosition<EnemyCharacter, CollisionCircle>(enemy1))
				{
					enemy1->SetPosition(originalPos.x, originalPos.y);

					originalPos = enemy2->GetPosition();

					enemy2->CollideWithWall(enemyCollisionObject1->CalculateNormal(enemy2->GetCenter()), penetrationDepth);

					// if there is no easy way to resolve this collision, 
					// ignore the collision by resetting all original positions
					if (!IsValidPosition<EnemyCharacter, CollisionCircle>(enemy2))
					{
						enemy2->SetPosition(originalPos.x, originalPos.y);
					}
				}
			}
		}
	}
}

void Game::KillEnemy(EnemyCharacter* enemy)
{
	int unusedSoundIndex;
	for (unusedSoundIndex = SOUND_ENEMY_DEATH0; unusedSoundIndex <= SOUND_ENEMY_DEATH4; ++unusedSoundIndex)
	{
		if (m_Sounds[unusedSoundIndex].getStatus() == 
				sf::SoundSource::Status::Stopped) break;
	}

	m_Sounds[unusedSoundIndex].play();
	enemy->PlayDeathAnimation();
	UpdateScore(m_Score + 1);
}

void Game::UpdateScore(int amount)
{
	m_Score = amount;

	if (!m_Player.IsDead() && !IsObjectiveComplete())
	{
		UpdateScoreDisplay();
	}

	if (IsObjectiveComplete())
	{
		EndGame(true);
	}
}

void Game::UpdateScoreDisplay()
{
	char scoreText[10];
	sprintf_s(scoreText, sizeof(scoreText), "Kills: %d", m_Score);
	m_TextRenderer.UpdateText(scoreText, 0.0f, 576.0f, 16.0f);
}

bool Game::IsObjectiveComplete()
{
	return m_Score >= OBJECTIVE_KILL_COUNT;
}

bool Game::IsWithinSpawnRange(glm::dvec2 position)
{
	double distanceFromPlayer = glm::length(position - m_Player.GetPosition());

	return distanceFromPlayer >= MIN_SPAWN_DISTANCE && distanceFromPlayer <= MAX_SPAWN_DISTANCE;
}

template <typename ColliderType>
bool Game::IsInsideWall(ColliderType* collider)
{
	std::vector<CollisionAABB>* wallCollisionObjects = m_pGameLevel->GetWallCollisionObjects();
	double penetrationDepth;
	for (std::vector<CollisionAABB>::iterator it = wallCollisionObjects->begin(); it != wallCollisionObjects->end(); ++it)
	{
		CollisionAABB wall = *it;

		penetrationDepth = m_CollisionSystem.CheckCollision(&wall, collider);
		if (penetrationDepth != 0.0)
		{
			return true;
		}
	}
	return false;
}

template <typename ObjectWithCollider, typename ColliderType>
bool Game::IsValidPosition(ObjectWithCollider* object)
{
	return !IsInsideWall<ColliderType>(object->GetCollisionObject()) &&
			m_pGameLevel->IsWithinLevel(object->GetPosition(), object->GetSize());
}

template <typename ObjectWithCollider, typename ColliderType>
bool Game::IsValidSpawnPosition(ObjectWithCollider* object)
{
	return IsWithinSpawnRange(object->GetPosition()) &&
			!IsInsideWall<ColliderType>(object->GetCollisionObject()) &&
			m_pGameLevel->IsWithinLevel(object->GetPosition(), object->GetSize());
}

void Game::EndGame(bool isWinner)
{
	if (isWinner)
	{
		for (std::vector<EnemyCharacter*>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); ++it)
		{
			(*it)->DisableTargetPoint();
		}

		DisplayWinningResults();
	}
	else
	{
		m_Player.PlayDeathAnimation();

		DisplayLosingResults();
	}

	m_Player.SetActive(false);
	m_Player.SetVelocity(0.0, 0.0);

	CleanUpLostEnemies();
}

void Game::DisplayWinningResults()
{
	std::string timeSurvivedString = GetTimeSurvivedString();

	m_TextRenderer.ClearAllText();
	m_TextRenderer.AddText("Objective Complete", 120, 100);
	m_TextRenderer.AddText("Final time:", 240, 200);
	m_TextRenderer.AddText(timeSurvivedString.c_str(), 220, 250);
	m_TextRenderer.AddText("Press R to play again.", 65, 450);
}

void Game::DisplayLosingResults()
{
	std::string timeSurvivedString = GetTimeSurvivedString();

	char scoreString[20];
	sprintf_s(scoreString, sizeof(scoreString), "Kill Count: %d\n", m_Score);

	m_TextRenderer.ClearAllText();
	m_TextRenderer.AddText("Time Survived: ", 200, 100);
	m_TextRenderer.AddText(timeSurvivedString.c_str(), 200, 150);
	m_TextRenderer.AddText(scoreString, 200, 400);
	m_TextRenderer.AddText("Press R to restart.", 105, 450);
}

std::string Game::GetTimeSurvivedString()
{
	int timeSurvivedInMS = GetCurrentTimeInMilliseconds() - m_StartTimeMS;

	int seconds = timeSurvivedInMS / 1000;
	int minutes = seconds / 60;
	int hours = minutes / 60;
	int milliseconds = timeSurvivedInMS - (seconds * 1000);
	seconds -= (minutes * 60);
	minutes -= (hours * 60);

	char timeSurvivedString[30];
	sprintf_s(timeSurvivedString, sizeof(timeSurvivedString), "%02d:%02d:%02d.%02d", hours, minutes, seconds, milliseconds);

	return std::string(timeSurvivedString);
}

bool Game::HandleKeyboardDown(unsigned char key, int x, int y)
{
	key = tolower(key);

	switch (key)
	{
		case 'r':
			if (m_Player.IsDead() || IsObjectiveComplete())
			{
				ResetGame();
				return true;
			}
			break;
	}

	if (m_Player.HandleKeyboardDown(key, x, y))
	{
		return true;
	}

	return false;
}

bool Game::HandleKeyboardUp(unsigned char key, int x, int y)
{
	if (m_Player.HandleKeyboardUp(key, x, y))
	{
		return true;
	}

	return false;
}

bool Game::HandleMouse(int button, int state, int x, int y)
{
	m_Crosshair.CenterOnScreenPosition((float)x, (float)y);

	if (m_Player.HandleMouse(button, state, x, y))
	{
		return true;
	}

	return false;
}

bool Game::HandleMovingMouse(int x, int y)
{
	m_Crosshair.CenterOnScreenPosition((float)x, (float)y);

	if (m_Player.HandleMovingMouse(x, y))
	{
		return true;
	}

	return false;
}

bool Game::HandlePassiveMouse(int x, int y)
{
	m_Crosshair.CenterOnScreenPosition((float)x, (float)y);

	if (m_Player.HandlePassiveMouse(x, y))
	{
		return true;
	}

	return false;
}

template <typename T> 
void Game::AddSpritesToList(std::vector<Sprite*>& spriteList, std::vector<T*>& objectList, bool(*condition)(T*))
{
	for (std::vector<T*>::iterator it = objectList.begin(); it != objectList.end(); ++it)
	{
		T* object = *it;
		if (condition == nullptr ||
			condition != nullptr && (*condition)(object))
		{
			spriteList.push_back(object->GetSprite());
		}
	}
}

std::vector<Sprite*> Game::GetSprites()
{
	std::vector<Sprite*> sprites;

	AddSpritesToList<BulletProjectile>(sprites, m_Bullets);
	AddSpritesToList<EnemyCharacter>(sprites, m_Enemies, [](EnemyCharacter* e) -> bool { return e->IsDead(); });
	sprites.push_back(m_Player.GetSprite());
	AddSpritesToList<EnemyCharacter>(sprites, m_Enemies, [](EnemyCharacter* e) -> bool { return !e->IsDead(); });
	AddSpritesToList<GrenadePickup>(sprites, m_GrenadePickups);
	AddSpritesToList<GrenadeProjectile>(sprites, m_Grenades);
	AddSpritesToList<Explosion>(sprites, m_Explosions);

	return sprites;
}

std::vector<IHUDElement*> Game::GetHUDElements()
{
	std::vector<IHUDElement*> elements;

	elements.push_back(&m_HUDGrenadeCount);
	elements.push_back(&m_TextRenderer);
	elements.push_back(&m_Crosshair);

	return elements;
}

GameLevel* Game::GetCurrentLevel()
{
	return m_pGameLevel;
}

glm::dvec2 Game::GetPlayerCenter()
{
	return m_Player.GetCenter();
}

std::vector<glm::vec2> Game::GetBulletPositions()
{
	std::vector<glm::vec2> bulletPositions;
	for (std::vector<BulletProjectile*>::iterator it = m_Bullets.begin(); it != m_Bullets.end(); ++it)
	{
		bulletPositions.push_back((*it)->GetCenter());
	}
	return bulletPositions;
}

std::vector<glm::vec3> Game::GetExplosionData()
{
	std::vector<glm::vec3> explosionData;
	for (std::vector<Explosion*>::iterator it = m_Explosions.begin(); it != m_Explosions.end(); ++it)
	{
		Explosion* explosion = *it;
		explosionData.push_back(glm::vec3(explosion->GetCenter(), explosion->GetLightFalloff()));
	}
	return explosionData;
}

std::vector<glm::vec4> Game::GetPickupData()
{
	std::vector<glm::vec4> pickupData;
	for (std::vector<GrenadePickup*>::iterator it = m_GrenadePickups.begin(); it != m_GrenadePickups.end(); ++it)
	{
		GrenadePickup* pickup = *it;
		pickupData.push_back(glm::vec4(pickup->GetCenter(), pickup->GetGlowColor()));
	}
	return pickupData;
}