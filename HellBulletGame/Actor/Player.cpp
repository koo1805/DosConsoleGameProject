#include "Player.h"
#include <Engine/Engine.h>
#include <Input/Input.h>
#include <Level/GameLevel.h>
#include <Actor/Projectile/PlayerProjectile.h>
#include <Actor/Projectile/EnemyProjectile.h>
#include <Actor/Effect/DestroyEffect.h>
#include <Actor/Manager/GameManager.h>

using namespace Craft;
Player::Player() : Actor("P", Vector2::Zero, Color::Green), fireMode(FireMode::Single)
{
	// 생성 위치 설정
	int x = (Engine::Get().GetWidth() / 2) - (width / 2);
	int y = (Engine::Get().GetHeight() - 2);
	SetPosition(Vector2(x, y));

	// x위치 저장
	xPosition = static_cast<float>(x);
	yPosition = static_cast<float>(y);

	// 연사 타이머 시간 설정
	timer.SetTargetTime(fireInterval);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 프레임 관련 문자열.
	const int size = 256;
	char fpsString[size] = {};
	sprintf_s(
		fpsString,
		size,
		"dt: %f | fps: %.1f",
		deltaTime,
		(1.0f / deltaTime)
	);

	// 콘솔 창 이름에 값 설정.
	SetConsoleTitleA(fpsString);

	// ESC 키 종료 처리
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}

	// 방향키 입력에 따른 이동 방향 설정
	// 오른쪽 | 아래 방향: 1 
	// 왼쪽 | 위 방향: -1
	float xDirection = 0.0f;
	float yDirection = 0.0f;
	if (Input::Get().GetKey(VK_RIGHT) || Input::Get().GetKey('D'))
	{
		xDirection = 1.0f;
	}
	if (Input::Get().GetKey(VK_LEFT) || Input::Get().GetKey('A'))
	{
		xDirection = -1.0f;
	}
	if (Input::Get().GetKey(VK_DOWN) || Input::Get().GetKey('S'))
	{
		yDirection = 1.0f;
	}
	if (Input::Get().GetKey(VK_UP) || Input::Get().GetKey('W'))
	{
		yDirection = -1.0f;
	}

	// 이동 함수 호출
	Move(xDirection, yDirection, deltaTime);

	// 발사 타이머 업데이트
	timer.Tick(deltaTime);

	if (fireMode == FireMode::Single)
	{
		// 탄약 발사 처리
		if (Input::Get().GetKeyDown(VK_SPACE))
		{
			Fire();
		}
	}
	else if (fireMode == FireMode::Automatic)
	{
		// 연사 모드 처리
		if (Input::Get().GetKey(VK_SPACE))
		{
			// 연사 발사 함수 호출
			AutomaticFire();
		}
	}

	// 발사 모드 전환 처리
	if (Input::Get().GetKeyDown('R'))
	{
		if (fireMode == FireMode::Single)
		{
			fireMode = FireMode::Automatic;
		}
		else if (fireMode == FireMode::Automatic)
		{
			fireMode = FireMode::Single;
		}
	}
}

void Player::OnCollision(const std::shared_ptr<Actor>& other)
{
	super::OnCollision(other);

	// 충돌한 액터가 적 탄약인지 확인
	if (other->IsTypeOf<EnemyProjectile>())
	{
		// 플레이어 제거
		Destroy();

		// 적 탄약 제거
		other->Destroy();

		// 파괴 이펙트 생성
		if (GetOwner())
		{
			GetOwner()->SpawnActor<DestroyEffect>(GetPosition());

			// 게임 관리자에 플레이어 죽음 알림
			std::shared_ptr<GameManager> gameManager = GetOwner()->GetActorOfType<GameManager>();
			if (gameManager)
			{
				gameManager->SetPlayerDead(position);
			}
		}

	}
}

void Player::Move(float xDirection, float yDirection, float deltaTime)
{
	// x위치 업데이트
	// 이동 처리 -> 이동 방향과 빠르기를 적용해서 새로운 위치를 구하는 것
	// 이동 방향(direction) / 빠르기(moveSpeed) | 시간
	// 동속도 운동: 이동 거리 = 기존의 위치 + 이동 방향 x 빠르기 x 시간
	xPosition += xDirection * moveSpeed * deltaTime;
	yPosition += yDirection * moveSpeed * deltaTime;

	// 화면 왼쪽 벗어나지 않도록 처리
	if (xPosition < 0)
	{
		xPosition = 0.0f;
	}

	// 화면 오른쪽 벗어나지 않도록 처리
	if (xPosition + width >= Engine::Get().GetWidth() -28)
	{
		xPosition = static_cast<float>(Engine::Get().GetWidth() - width - 28);
	}

	// 화면 위쪽 벗어나지 않도록 처리
	if (yPosition < 0)
	{
		yPosition = 0.0f;
	}

	// 화면 아래쪽 벗어나지 않도록 처리
	if (yPosition + height >= Engine::Get().GetHeight())
	{
		yPosition = static_cast<float>(Engine::Get().GetHeight() - height);
	}

	// 위치 업데이트
	Vector2 newPosition = GetPosition();
	// float 값을 int로 형변환할 때 소숫점 값은 버림 처리된다는 점 주의
	newPosition.x = static_cast<int>(xPosition);
	newPosition.y = static_cast<int>(yPosition);
	SetPosition(newPosition);
}

void Player::Fire()
{
	// 탄약 생성 위치 구하기.
	// 플레이어의 가운데 위치
	Vector2 projectilePosition(
		GetPosition().x + (width / 2),
		GetPosition().y
	);

	// 탄약 생성.
	std::shared_ptr<Level> owner = GetOwner();
	if (owner)
	{
		owner->SpawnActor<PlayerProjectile>(projectilePosition);
	}
}

void Player::AutomaticFire()
{
	// 발사 가능 여부 확인.
	if (!CanShoot())
	{
		return;
	}

	// 발사 처리.
	Fire();

	// 경과 시간 초기화.
	timer.Reset();
}
