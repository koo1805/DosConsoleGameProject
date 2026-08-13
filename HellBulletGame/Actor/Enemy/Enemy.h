#pragma once

#include <Actor/Actor.h>
#include <Util/Timer.h>
#include <Render/Sprite.h>

class Enemy : public Craft::Actor
{
	// 커스텀 타입 설정
	TYPE_DECLARATIONS(Enemy, Actor)

		// 이동 방향 열거형
		enum class MoveDirection
	{
		None = -1,
		Left,
		Right,
		Down,
		DownR,
		DownL,
		DownUp,
		V
	};

public:
	Enemy(const std::string& image, int yPosition = 5);

	Enemy(const Craft::Sprite& sprite, int yPosition = 5);


private:
	// 이벤트 함수 오버라이드
	virtual void Tick(float deltaTime) override;

	// 충돌 처리 함수 오버라이드
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

private:
	// 이동 방향 열거형 변수
	MoveDirection direction = MoveDirection::None;

	// 이동 처리를 위함 변수
	float xPosition = 0.0f;
	float yPosition = 0.0f;
	float moveSpeed = 5.0f;

	float stopDownPosition = 5.0f;

	// 발사 타이머
	Timer timer;
};