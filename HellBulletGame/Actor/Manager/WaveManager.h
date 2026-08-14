#pragma once

#include <Actor/Actor.h>
#include <Actor/Enemy/EnemyStats.h>
#include <Math/Vector2.h>

#include <vector>

struct WaveSpawnInfo
{
	float spawnTime = 0.0f;
	Craft::Vector2 position;
	EnemyMovePattern movePattern = EnemyMovePattern::Down;
};

class WaveManager : public Craft::Actor
{
	TYPE_DECLARATIONS(WaveManager, Actor)

public:
	WaveManager();

protected:
	virtual void Tick(float deltaTime) override;

private:

	void BuildWave();
	void SpawnWaveEnemy(const WaveSpawnInfo& spawnInfo);

	// 가로로 몹을 생성하는 함수
	void AddHorizontalLine(
		float spawnTime,
		int startX,
		int y,
		int count,
		int spacing,
		EnemyMovePattern movePattern
	);

	// 세로로 몹을 생성하는 함수
	void AddVerticalLine(
		float spawnTime,
		int x,
		int y,
		int count,
		float interval,
		EnemyMovePattern movePattern
	);

	// 교차 패턴
	void AddCross(
		float spawnTime,
		int leftX,
		int rightX,
		int y,
		int count,
		float interval
	);

private:
	// 웨이브를 저장할 변수
	std::vector<WaveSpawnInfo> wave;

	// 웨이브 시간
	float waveTime = 0.0f;

	// 웨이브 단계
	int waveIndex = 0;
};

