#include "WaveManager.h"
#include <Level/Level.h>
#include <Actor/Enemy/NormalEnemy.h>

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

void WaveManager::BuildWave()
{
	wave.clear();

	waveTime = 0.0f;
	waveIndex = 0;

	AddHorizontalLine(0.0f, 20, 0, 5, 10, EnemyMovePattern::Down);
	AddVerticalLine(2.0f, 15, 0, 5, 1.0f, EnemyMovePattern::Down);
	AddCross(8.0f, 10, 80, 0, 3, 2.0f);
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

	owner->SpawnActor<NormalEnemy>(spawnInfo.position, spawnInfo.movePattern);
}

void WaveManager::AddHorizontalLine(float spawnTime, int startX, int y, int count, int spacing, EnemyMovePattern movePattern)
{
	for (int i = 0; i < count; ++i)
	{
		wave.push_back({ spawnTime, Craft::Vector2(startX + spacing * i, y), movePattern });
	}
}

void WaveManager::AddVerticalLine(float spawnTime, int x, int y, int count, float interval, EnemyMovePattern movePattern)
{
	for (int i = 0; i < count; ++i)
	{
		wave.push_back({ spawnTime + interval * i, Craft::Vector2(x, y), movePattern });
	}
}

void WaveManager::AddCross(float spawnTime, int leftX, int rightX, int y, int count, float interval)
{
	for (int i = 0; i < count; ++i)
	{
		wave.push_back({ spawnTime + interval * i, Craft::Vector2(leftX, y), EnemyMovePattern::DiagonalRight });
		wave.push_back({ spawnTime + interval * i, Craft::Vector2(rightX, y), EnemyMovePattern::DiagonalLeft });
	}
}
