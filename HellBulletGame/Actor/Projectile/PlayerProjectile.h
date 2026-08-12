#pragma once

#include <Actor/Actor.h>
#include <Util/Timer.h>

class PlayerProjectile : public Craft::Actor
{
	// 커스텀 타입 설정
	TYPE_DECLARATIONS(PlayerProjectile, Actor)

public:
	PlayerProjectile(const Craft::Vector2& position);

private:
	// 오버라이딩 함수
	virtual void Tick(float deltaTime);

private:
	Timer timer;

	// 속도 (단위 - 초)
	float moveSpeed = 30.0f;

	// 위치 갱신할 때 사용할 변수
	float yPosition = 0.0f;

	float xPosition = 0.0f;

	float curveTime = 0.5f;

};

