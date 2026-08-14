#pragma once

#include <Actor/Actor.h>
#include <Util/Timer.h>

class EnemySpawner : public Craft::Actor
{
	TYPE_DECLARATIONS(EnemySpawner, Actor)

public:
	EnemySpawner();

protected:
	virtual void Tick(float deltaTime) override;

private:
	// 적 생성 함수
	void SpawnEnemy();

private:
	// 타이머
	Craft::Timer timer;
};