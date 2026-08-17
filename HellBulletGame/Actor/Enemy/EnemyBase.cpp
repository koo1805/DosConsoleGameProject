#include "EnemyBase.h"
#include <Engine/Engine.h>
#include <Level/Level.h>
#include <Render/Sprite.h>
#include <Math/Vector2.h>
#include <Util/Util.h>
#include <Actor/Player.h>
#include <Actor/Projectile/ProjectileBase.h>
#include <Actor/Projectile/EnemyProjectile.h>
#include <Actor/Effect/DestroyEffect.h>
#include <Actor/Manager/GameManager.h>

#include <cmath>

using namespace Craft;
EnemyBase::EnemyBase(const Craft::Sprite& sprite, const EnemyStats& stats, const Craft::Vector2& position)
	: Actor(sprite, position),
	stats(stats),
	currentHp(stats.maxHp),
	xPosition(static_cast<float>(position.x)),
	yPosition(static_cast<float>(position.y))
{
	timer.SetTargetTime(stats.fireInterval);
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

	// 플레이 영역 밖 렌더링 처리
	CheckOutScreen();

	// 발사 타이머 처리
	timer.Tick(deltaTime);

	// 발사 처리
	Fire();
}

void EnemyBase::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	// 충돌한 물체가 플레이어 탄약인지 확인
	// 커스텀 타입 활용
	if (other->IsTypeOf<ProjectileBase>())
	{
		std::shared_ptr<ProjectileBase> projectile = Craft::Cast<ProjectileBase>(other);

		if (!projectile) 
		{
			return;
		}

		if(projectile->GetTeam() != ProjectileTeam::Player)
		{
			return;
		}

		const int damage = projectile->GetDamage();

		// 플레이어 탄약 제거
		projectile->Destroy();

		TakeDamage(damage);

		if (currentHp <= 0)
		{
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
	
	case EnemyMovePattern::TargetStop:
		MoveTargetStop(deltaTime);
		break;
	
	case EnemyMovePattern::Zigzag:
		MoveZigzag(deltaTime);
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
	xPosition -= stats.moveSpeed * deltaTime;
}

void EnemyBase::MoveRight(float deltaTime)
{
	xPosition += stats.moveSpeed * deltaTime;
}

void EnemyBase::MoveTargetStop(float deltaTime)
{
	const float targetX = static_cast<float>(stats.targetPosition.x);

	const float targetY = static_cast<float>(stats.targetPosition.y);

	const float deltaX = targetX - xPosition;
	const float deltaY = targetY - yPosition;

	const float distance = std::sqrt((deltaX * deltaX) + (deltaY * deltaY));

	// 목적지 도착시
	if (distance <= 0.5f)
	{
		xPosition = targetX;
		yPosition = targetY;

		return;
	}

	// 이번 프레임 이동 거리
	const float moveDistance = stats.moveSpeed * deltaTime;

	// 목적지를 넘어갈때
	if (moveDistance >= distance)
	{
		xPosition = targetX;
		yPosition = targetY;

		return;
	}

	// 목표 방향으로 정규화
	const float directionX = deltaX / distance;

	const float directionY = deltaY / distance;


	xPosition += directionX * moveDistance;

	yPosition += directionY * moveDistance;
}

void EnemyBase::MoveZigzag(float deltaTime)
{

}

void EnemyBase::CheckOutScreen()
{
	if (xPosition + GetWidth() < 0)
	{
		Destroy();
		return;
	}

	if (xPosition >= Engine::Get().GetPlayAreaWidth())
	{
		Destroy();
		return;
	}

	if (yPosition > Engine::Get().GetHeight())
	{
		Destroy();
		return;
	}
}

void EnemyBase::Fire()
{
	// 발사하지 않는 Enemy
	if (stats.firePattern == EnemyFirePattern::None)
	{
		return;
	}

	// 발사 시간 예외 처리
	if (!timer.IsTimeOut())
	{
		return;
	}

	switch (stats.firePattern)
	{
	case EnemyFirePattern::None:
		break;

	case EnemyFirePattern::Straight:
		FireStraight();
		break;

	case EnemyFirePattern::EightWay:
		FireEightWay();
		break;

	case EnemyFirePattern::AimPlayer:
		FireAimPlayer();
		break;

	case EnemyFirePattern::Burst:
		FireBurst();
		break;
	}


	// 다음 발사까지 다시 시간 측정
	timer.Reset();
}

void EnemyBase::FireStraight()
{
	SpawnProjectile(0.0f, 1.0f);
}

void EnemyBase::FireEightWay()
{
	const float diagonal = 0.707f;
	// 위
	SpawnProjectile(0.0f, -1.0f);
	// 아래
	SpawnProjectile(0.0f, 1.0f);
	// 왼쪽
	SpawnProjectile(-1.0f, 0.0f);
	// 오른쪽
	SpawnProjectile(1.0f, 0.0f);
	// 왼족 위
	SpawnProjectile(-diagonal, -diagonal);
	// 오른쪽 위
	SpawnProjectile(diagonal, -diagonal);
	// 왼족 아래
	SpawnProjectile(-diagonal, diagonal);
	// 오른쪽 아래
	SpawnProjectile(diagonal, diagonal);
}

void EnemyBase::FireAimPlayer()
{
	std::shared_ptr<Craft::Level> owner = GetOwner();

	if (!owner)
	{
		return;
	}

	std::shared_ptr<Player> player = owner->GetActorOfType<Player>();

	if(!player)
	{
		return;
	}

	// Enemy의 발사 위치
	const Craft::Vector2 firePosition = GetFirePosition();

	// Player의 중심 위치
	const Craft::Vector2 playerPosition(
		player->GetPosition().x + (player->GetWidth() / 2),
		player->GetPosition().y + (player->GetHeight() / 2)
	);

	// Enemy → Player 방향 벡터
	float directionX = static_cast<float>(playerPosition.x - firePosition.x);

	float directionY = static_cast<float>(playerPosition.y - firePosition.y);

	// 방향 벡터 길이
	const float length = std::sqrt((directionX * directionX) + (directionY * directionY));

	// Enemy와 Player 위치가 완전히 같은 경우 방지
	if (length <= 0.0001f)
	{
		return;
	}

	// 정규화
	directionX /= length;
	directionY /= length;

	// Player 방향으로 발사
	SpawnProjectile(directionX, directionY);
}

void EnemyBase::FireBurst()
{}

Craft::Vector2 EnemyBase::GetFirePosition() const
{
	return Craft::Vector2((GetPosition().x + GetWidth() / 2), GetPosition().y + GetHeight());
}

void EnemyBase::SpawnProjectile(float directionX, float directionY)
{
	std::shared_ptr<Craft::Level> owner = GetOwner();

	if (!owner)
	{
		return;
	}

	owner->SpawnActor<EnemyProjectile>(
		GetFirePosition(),
		directionX,
		directionY,
		15.0f,
		stats.damage
	);
}
