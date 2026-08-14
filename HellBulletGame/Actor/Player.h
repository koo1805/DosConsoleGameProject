#pragma once

#include <Actor/Actor.h>
#include <Util/Timer.h>

class Player : public Craft::Actor
{
	// 발사 모드 (단발 / 연사)
	enum class FireMode
	{
		None = -1,
		Single,
		Automatic
	};

	// 커스텀 타입 등록
	TYPE_DECLARATIONS(Player, Actor)

public:
	Player();

private:
	// 함수 오버라이딩
	virtual void Tick(float deltaTime) override;

	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

	// 플레이어 이동 함수
	void Move(float xDirection, float yDirection, float deltaTime);

	// Projectile 발사 함수
	void Fire();

	// 총 연사 함수
	void AutomaticFire();

	// 발사 가능 여부 확인 함수
	inline bool CanShoot() const { return timer.IsTimeOut(); }

private:
	// 이동 처리에 필요한 함수
	float xPosition = 0.0f;
	float yPosition = 0.0f;

	// 이동 속도 변수
	float moveSpeed = 50.0f;

	// 발사 모드 변수
	FireMode fireMode = FireMode::None;

	// 타이머 변수
	Craft::Timer timer;

	// 연사 시 발사 간격(단위: 초)
	float fireInterval = 0.15f;
};