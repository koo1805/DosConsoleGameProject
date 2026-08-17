#include "WaveManager.h"
#include <Level/Level.h>
#include <Actor/Enemy/NormalEnemy.h>
#include <Actor/Enemy/FastEnemy.h>
#include <Actor/Enemy/EightwayEnemy.h>
#include <Actor/Enemy/BossEnemy.h>
#include <Actor/Manager/GameManager.h>

using namespace Craft;
WaveManager::WaveManager()
{
	BuildWave();
}

void WaveManager::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	waveTime += deltaTime;

	while (waveIndex < static_cast<int>(wave.size()) && wave[waveIndex].spawnTime <= waveTime)
	{
		SpawnWaveEnemy(wave[waveIndex]);
		++waveIndex;
	}
}

// 적 웨이브 구축
void WaveManager::BuildWave()
{
	wave.clear();

	waveTime = 0.0f;
	waveIndex = 0;

	AddHorizontalLine(0.0f, 20, 0, 5, 10, EnemyType::Normal, EnemyMovePattern::Down, EnemyFirePattern::Straight);
	AddVerticalLine(2.0f, 15, 0, 4, 1.0f, EnemyType::Normal, EnemyMovePattern::Down, EnemyFirePattern::AimPlayer);

	AddHorizontalLine(7.0f, 40, 0, 2, 40, EnemyType::Fast, EnemyMovePattern::DownToSide, EnemyFirePattern::Straight);
	AddHorizontalLine(9.0f, 40, 0, 2, 40, EnemyType::Fast, EnemyMovePattern::DownToSide, EnemyFirePattern::Straight);
	
	AddCross(12.0f, 20, 100, 0, 3, 2.0f, EnemyType::Normal, EnemyFirePattern::Straight);
	AddTargetPosition(16.0f, Craft::Vector2(10, 0), Craft::Vector2(60, 20), EnemyType::Eightway, EnemyFirePattern::EightWay);
	AddHorizontalLine(17.0f, 40, 0, 4, 20, EnemyType::Fast, EnemyMovePattern::DownToSide, EnemyFirePattern::AimPlayer);
	AddTargetPosition(22.0f, Craft::Vector2(52, -5), Craft::Vector2(52, 6), EnemyType::Boss, EnemyFirePattern::AimPlayer);
}

void WaveManager::SpawnWaveEnemy(const WaveSpawnInfo& spawnInfo)
{
	// 적 액터 생성
	std::shared_ptr<Craft::Level> owner = GetOwner();

	// 예외 처리
	if (!owner)
	{
		return;
	}

	switch (spawnInfo.enemyType)
	{
	case EnemyType::Normal:
	{
		owner->SpawnActor<NormalEnemy>(spawnInfo.position, spawnInfo.targetPosition, spawnInfo.movePattern, spawnInfo.firePattern);

		break;
	}
	
	case EnemyType::Fast:
	{
		owner->SpawnActor<FastEnemy>(spawnInfo.position, spawnInfo.targetPosition, spawnInfo.movePattern, spawnInfo.firePattern);

		break;
	}

	case EnemyType::Eightway:
	{
		owner->SpawnActor<EightwayEnemy>(spawnInfo.position, spawnInfo.targetPosition, spawnInfo.movePattern, spawnInfo.firePattern);

		break;
	}

	case EnemyType::Boss:
	{
		std::shared_ptr<BossEnemy> boss = owner->SpawnActor<BossEnemy>(spawnInfo.position, spawnInfo.targetPosition);

		std::shared_ptr<GameManager> gameManager = owner->GetActorOfType<GameManager>();

		if (gameManager)
		{
			gameManager->SetBoss(boss);
		}

		break;
	}
	}
}

// 수평 패턴
void WaveManager::AddHorizontalLine(float spawnTime, int startX, int y, int count, int spacing, EnemyType enemyType, EnemyMovePattern movePattern, EnemyFirePattern firePattern)
{
	for (int i = 0; i < count; ++i)
	{
		wave.emplace_back(spawnTime, Craft::Vector2(startX + spacing * i, y), enemyType, movePattern, firePattern);
	}
}

// 수직 패턴
void WaveManager::AddVerticalLine(float spawnTime, int x, int y, int count, float interval, EnemyType enemyType, EnemyMovePattern movePattern, EnemyFirePattern firePattern)
{
	for (int i = 0; i < count; ++i)
	{
		wave.emplace_back(spawnTime + interval * i, Craft::Vector2(x, y), enemyType, movePattern, firePattern);
	}
}

// 교차 패턴
void WaveManager::AddCross(float spawnTime, int leftX, int rightX, int y, int count, float interval, EnemyType enemyType, EnemyFirePattern firePattern)
{
	for (int i = 0; i < count; ++i)
	{
		wave.emplace_back(spawnTime + interval * i, Craft::Vector2(leftX, y), enemyType, EnemyMovePattern::DiagonalRight, firePattern);
		wave.emplace_back(spawnTime + interval * i, Craft::Vector2(rightX, y), enemyType, EnemyMovePattern::DiagonalLeft, firePattern);
	}
}

void WaveManager::AddTargetPosition(float spawnTime, const Craft::Vector2& spawnPosition, const Craft::Vector2& targetPosition, EnemyType enemyType, EnemyFirePattern firePattern)
{
	wave.emplace_back(spawnTime, spawnPosition, targetPosition, enemyType, EnemyMovePattern::TargetStop, firePattern);
}

void WaveManager::AddBoss(float spawnTime, const Craft::Vector2& targetPosition)
{
	// Todo: 보스 소환
}
