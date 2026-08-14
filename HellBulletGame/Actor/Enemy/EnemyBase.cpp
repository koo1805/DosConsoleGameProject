#include "EnemyBase.h"
#include <Level/Level.h>
#include <Render/Sprite.h>
#include <Math/Vector2.h>
#include <Util/Util.h>
#include <Actor/Projectile/PlayerProjectile.h>
#include <Actor/Effect/DestroyEffect.h>
#include <Actor/Manager/GameManager.h>

EnemyBase::EnemyBase(const Craft::Sprite& sprite, const EnemyStats& stats, const Craft::Vector2& position)
	: Actor(sprite), stats(stats), currentHp(stats.maxHp)
{
	SetPosition(position);

	xPosition = static_cast<float>(position.x);
	yPosition = static_cast<float>(position.y);
}

void EnemyBase::TakeDamage(int damage)
{
	currentHp -= damage;

	if (currentHp <= 0)
	{
		Destroy();
	}
}

void EnemyBase::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 이동 처리
	Move(deltaTime);

	// 발사 처리
	Fire();
}

void EnemyBase::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	// 충돌한 물체가 플레이어 탄약인지 확인
	// 커스텀 타입 활용
	if (other->IsTypeOf<PlayerProjectile>())
	{
		// 플레이어 탄약 제거
		other->Destroy();

		// 적 액터 제거
		Destroy();

		// 죽음 이펙트 구현 후 재생
		if (GetOwner())
		{
			GetOwner()->SpawnActor<DestroyEffect>(GetPosition());

			// 점수 획득 처리
			std::shared_ptr<GameManager> gameManager = GetOwner()->GetActorOfType<GameManager>();

			// 게임 관리자에 점수 획득 알림
			if (gameManager)
			{
				gameManager->SetScore(gameManager->GetScore() + stats.score);
			}
		}
	}
}

void EnemyBase::Move(float deltaTime)
{
	// 등장 패턴
	switch (stats.movePattern)
	{
	case EnemyMovePattern::Down:
		MoveDown(deltaTime);
		break;
	
	case EnemyMovePattern::DownToSide:
		MoveDownToSide(deltaTime);
		break;

	case EnemyMovePattern::DiagonalLeft:
		MoveDiagonalLeft(deltaTime);
		break;

	case EnemyMovePattern::DiagonalRight:
		MoveDiagonalRight(deltaTime);
		break;

	case EnemyMovePattern::Left:
		MoveLeft(deltaTime);
		break;

	case EnemyMovePattern::Right:
		MoveRight(deltaTime);
		break;
	}
	
	// 위치 설정
	SetPosition(Craft::Vector2(static_cast<int>(xPosition), static_cast<int>(yPosition)));
}

void EnemyBase::MoveDown(float deltaTime)
{
	yPosition += stats.moveSpeed * deltaTime;
}

void EnemyBase::MoveDownToSide(float deltaTime)
{
	// 목표 y위치까지 내려감
	if (moveState == EnemyMoveState::Entering)
	{
		yPosition += stats.moveSpeed * deltaTime;

		if (yPosition >= sideStartY)
		{
			moveState = EnemyMoveState::Moving;

			int random = Util::RandomRange(0, 1);
			direction = random == 0 ? -1.0f : 1.0f;
		}

		return;
	}

	xPosition += direction * stats.moveSpeed * deltaTime;
}

void EnemyBase::MoveDiagonalLeft(float deltaTime)
{
	xPosition -= stats.moveSpeed * deltaTime;
	yPosition += stats.moveSpeed * deltaTime;
}

void EnemyBase::MoveDiagonalRight(float deltaTime)
{
	xPosition += stats.moveSpeed * deltaTime;
	yPosition += stats.moveSpeed * deltaTime;
}

void EnemyBase::MoveLeft(float deltaTime)
{
	xPosition += stats.moveSpeed * deltaTime;
}

void EnemyBase::MoveRight(float deltaTime)
{
	xPosition -= stats.moveSpeed * deltaTime;
}

void EnemyBase::Fire()
{
}
