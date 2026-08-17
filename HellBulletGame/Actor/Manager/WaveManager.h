#pragma once

#include <Actor/Actor.h>
#include <Actor/Enemy/EnemyStats.h>
#include <Math/Vector2.h>

#include <vector>

enum class EnemyType
{
	Normal,
	Fast,
	Eightway,
	Boss
};

struct WaveSpawnInfo
{
	// 일반 생성자
	WaveSpawnInfo(
		float spawnTime,
		const Craft::Vector2& position,
		EnemyType enemyType,
		EnemyMovePattern movePattern,
		EnemyFirePattern firePattern
	)
		: spawnTime(spawnTime),
		position(position),
		enemyType(enemyType),
		movePattern(movePattern),
		firePattern(firePattern)
	{}

	// 목적지 필요 생성자
	WaveSpawnInfo(
		float spawnTime,
		const Craft::Vector2& position,
		const Craft::Vector2& targetPosition,
		EnemyType enemyType,
		EnemyMovePattern movePattern,
		EnemyFirePattern firePattern
	)
		: spawnTime(spawnTime),
		position(position),
		targetPosition(targetPosition),
		enemyType(enemyType),
		movePattern(movePattern),
		firePattern(firePattern)
	{}

	float spawnTime = 0.0f;
	// 적 생성 위치
	Craft::Vector2 position;

	// 특정 위치로 이동할 때 사용하는 목적지
	Craft::Vector2 targetPosition = Craft::Vector2(0, 0);

	// Enemy 타입
	EnemyType enemyType;

	EnemyMovePattern movePattern = EnemyMovePattern::Down;

	EnemyFirePattern firePattern = EnemyFirePattern::Straight;
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
		EnemyType enemyType,
		EnemyMovePattern movePattern,
		EnemyFirePattern firePattern
	);

	// 세로로 몹을 생성하는 함수
	void AddVerticalLine(
		float spawnTime,
		int x,
		int y,
		int count,
		float interval,
		EnemyType enemyType,
		EnemyMovePattern movePattern,
		EnemyFirePattern firePattern
	);

	// 교차 패턴
	void AddCross(
		float spawnTime,
		int leftX,
		int rightX,
		int y,
		int count,
		float interval,
		EnemyType enemyType,
		EnemyFirePattern firePattern
	);

	// 목적지 이동 패턴
	void AddTargetPosition(
		float spawnTime,
		const Craft::Vector2& spawnPosition,
		const Craft::Vector2& targetPosition,
		EnemyType enemyType,
		EnemyFirePattern firePattern
	);

	// 보스 추가
	void AddBoss(
		float spawnTime,
		const Craft::Vector2& targetPosition
	);
private:
	// 웨이브를 저장할 변수
	std::vector<WaveSpawnInfo> wave;

	// 웨이브 시간
	float waveTime = 0.0f;

	// 웨이브 단계
	int waveIndex = 0;
};

