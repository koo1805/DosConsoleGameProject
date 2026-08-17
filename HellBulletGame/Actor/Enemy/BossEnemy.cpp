#include "BossEnemy.h"
#include <Engine/Engine.h>
#include <Level/Level.h>
#include <Actor/Manager/GameManager.h>
#include <Actor/Effect/LaserWarning.h>
#include <Actor/Projectile/ProjectileBase.h>
#include <Actor/Projectile/LaserProjectile.h>

BossEnemy::BossEnemy(const Craft::Vector2& position, const Craft::Vector2& targetPosition)
	: EnemyBase(
		Craft::Sprite({
			"   /=======\\  ",
			" /           \\",
			"|    < O >    |",
			" \\           /",
			"   \\=======/  "
		}),
		EnemyStats{
			60,
			5.0f,
			1.0f,
			100,
			1,
			EnemyMovePattern::TargetStop,
			EnemyFirePattern::AimPlayer,
			targetPosition
		},
		position
	)
{ }

void BossEnemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	UpdatePhase();

	if (bossState == BossState::Phase2)
	{
		UpdateSpecialPattern(deltaTime);
	}
}

void BossEnemy::Fire()
{
	if(bossState == BossState::Phase2)
	{
		return;
	}

	if (!timer.IsTimeOut())
	{
		return;
	}

	FireAimPlayer();

	timer.Reset();
}

void BossEnemy::OnCollision(const std::shared_ptr<Craft::Actor>&other)
{
	if (isInvincible)
	{
		if (other->IsTypeOf<ProjectileBase>())
		{
			std::shared_ptr<ProjectileBase> projectile = Craft::Cast<ProjectileBase>(other);

			if (projectile && projectile->GetTeam() == ProjectileTeam::Player)
			{
				projectile->Destroy();
			}
		}
		return;
	}

	EnemyBase::OnCollision(other);

	if (currentHp <= 0)
	{
		std::shared_ptr<Craft::Level> owner = GetOwner();

		if (owner)
		{
			std::shared_ptr<GameManager> gameManager = owner->GetActorOfType<GameManager>();

			if (gameManager)
			{
				gameManager->SetBossDead();
			}
		}
	}
}

void BossEnemy::UpdatePhase()
{
	if (hasEnteredPhase2)
	{
		return;
	}

	if (currentHp <= phase2Hp)
	{
		EnterPhase2();
	}
}

void BossEnemy::EnterPhase2()
{
	hasEnteredPhase2 = true;

	bossState = BossState::Phase2;

	isInvincible = true;

	color = Craft::Color::Red;

	specialState = BossSpecialState::HorizontalWarning;

	SpawnHorizontalWarning();

	specialTimer.SetTargetTime(warningTime);

	specialTimer.Reset();
}

void BossEnemy::ExitPhase2()
{
	specialState = BossSpecialState::None;

	bossState = BossState::Phase1;

	isInvincible = false;

	color = Craft::Color::White;

	timer.SetTargetTime(stats.fireInterval);

	timer.Reset();
}

void BossEnemy::SpawnHorizontalWarning()
{
	std::shared_ptr<Craft::Level> owner = GetOwner();

	if (!owner)
	{
		return;
	}

	const int playWidth = Craft::Engine::Get().GetPlayAreaWidth();

	const int screenHeight = Craft::Engine::Get().GetHeight();

	const int lineCount = 5;

	const int spacing = screenHeight / (lineCount + 1);

	const std::string warningLine(playWidth, '-');

	for (int i = 0; i < lineCount; ++i)
	{
		const int y = spacing * (i + 1);

		owner->SpawnActor<LaserWarning>(
			warningLine,
			Craft::Vector2(0, y),
			warningTime
		);
	}
}

void BossEnemy::SpawnVerticalWarning()
{
	std::shared_ptr<Craft::Level> owner = GetOwner();

	if (!owner)
	{
		return;
	}

	const int playWidth = Craft::Engine::Get().GetPlayAreaWidth();

	const int screenHeight = Craft::Engine::Get().GetHeight();

	const int lineCount = 5;

	const int spacing = playWidth / (lineCount + 1);

	std::vector<std::string> lineImage;

	for (int y = 0; y < screenHeight; ++y)
	{
		lineImage.emplace_back("|");
	}

	const Craft::Sprite warningSprite(lineImage);

	for (int i = 0; i < lineCount; ++i)
	{
		const int x = spacing * (i + 1);

		owner->SpawnActor<LaserWarning>(
			warningSprite,
			Craft::Vector2(x, 0),
			warningTime
		);
	}
}

void BossEnemy::SpawnGridWarning()
{
	SpawnHorizontalWarning();
	SpawnVerticalWarning();
}

void BossEnemy::FireHorizontalLaser()
{
	std::shared_ptr<Craft::Level> owner = GetOwner();

	if (!owner)
	{
		return;
	}

	const int playWidth = Craft::Engine::Get().GetPlayAreaWidth();

	const int screenHeight = Craft::Engine::Get().GetHeight();

	const int lineCount = 5;

	const int spacing = screenHeight / (lineCount + 1);

	const std::string laserLine(playWidth, '#');

	for (int i = 0; i < lineCount; ++i)
	{
		const int y = spacing * (i + 1);

		owner->SpawnActor<LaserProjectile>(
			laserLine,
			Craft::Vector2(0, y),
			laserTime,
			stats.damage
		);
	}
}

void BossEnemy::FireVerticalLaser()
{
	std::shared_ptr<Craft::Level> owner = GetOwner();

	if (!owner)
	{
		return;
	}

	const int playWidth = Craft::Engine::Get().GetPlayAreaWidth();

	const int screenHeight = Craft::Engine::Get().GetHeight();

	const int lineCount = 5;

	const int spacing = playWidth / (lineCount + 1);

	std::vector<std::string> lineImage;

	for (int y = 0; y < screenHeight; ++y)
	{
		lineImage.emplace_back("#");
	}

	const Craft::Sprite laserSprite(lineImage);

	for (int i = 0; i < lineCount; ++i)
	{
		const int x = spacing * (i + 1);

		owner->SpawnActor<LaserProjectile>(
			laserSprite,
			Craft::Vector2(x, 0),
			laserTime,
			stats.damage
		);
	}
}

void BossEnemy::FireGridLaser()
{
	FireHorizontalLaser();
	FireVerticalLaser();
}

void BossEnemy::UpdateSpecialPattern(float deltaTime)
{
	specialTimer.Tick(deltaTime);

	if (!specialTimer.IsTimeOut())
	{
		return;
	}


	switch (specialState)
	{
	case BossSpecialState::None:
	{
		break;
	}

	// 1. 가로
	case BossSpecialState::HorizontalWarning:
	{
		FireHorizontalLaser();

		specialState = BossSpecialState::HorizontalLaser;

		specialTimer.SetTargetTime(laserTime);

		specialTimer.Reset();

		break;
	}


	case BossSpecialState::HorizontalLaser:
	{
		SpawnVerticalWarning();

		specialState = BossSpecialState::VerticalWarning;

		specialTimer.SetTargetTime(warningTime);

		specialTimer.Reset();

		break;
	}

	// 2. 세로
	case BossSpecialState::VerticalWarning:
	{
		FireVerticalLaser();

		specialState =
			BossSpecialState::VerticalLaser;

		specialTimer.SetTargetTime(laserTime);

		specialTimer.Reset();

		break;
	}

	case BossSpecialState::VerticalLaser:
	{
		SpawnGridWarning();

		specialState = BossSpecialState::GridWarning;

		specialTimer.SetTargetTime(warningTime);

		specialTimer.Reset();

		break;
	}

	case BossSpecialState::GridWarning:
	{
		// 가로 + 세로 동시 발사
		FireGridLaser();

		specialState = BossSpecialState::GridLaser;

		specialTimer.SetTargetTime(laserTime);
		specialTimer.Reset();

		break;
	}

	case BossSpecialState::GridLaser:
	{
		ExitPhase2();

		break;
	}
	}
}
