#pragma once

#include <Actor/Enemy/EnemyBase.h>
#include <Util/Timer.h>

class BossEnemy : public EnemyBase
{
	TYPE_DECLARATIONS(BossEnemy, EnemyBase)

public:
	BossEnemy(
		const Craft::Vector2& position,
		const Craft::Vector2& targetPosition
	);

private:
	enum class BossSpecialState
	{
		None,

		HorizontalWarning,
		HorizontalLaser,

		VerticalWarning,
		VerticalLaser,

		GridWarning,
		GridLaser
	};

	enum class BossState
	{
		Entering,
		Phase1,
		Phase2
	};

protected:
	virtual void Tick(float deltaTime) override;

	virtual void Fire() override;

	virtual void OnCollision(const std::shared_ptr<Craft::Actor>& other) override;

protected:
	void UpdatePhase();

	void EnterPhase2();

	void ExitPhase2();

	void SpawnHorizontalWarning();
	void SpawnVerticalWarning();
	void SpawnGridWarning();

	void FireHorizontalLaser();
	void FireVerticalLaser();
	void FireGridLaser();

	void UpdateSpecialPattern(float deltaTime);

	//void FireBossSpecial();

protected:
	BossState bossState = BossState::Entering;

	BossSpecialState specialState = BossSpecialState::None;

	// 무적
	bool isInvincible = false;

	bool hasEnteredPhase2 = false;

	int phase2Hp = 30;

	Craft::Timer specialTimer;

	float warningTime = 1.0f;

	float laserTime = 0.6f;

	float specialDuration = 5.0f;

	float specialAngle = 0.0f;
};

