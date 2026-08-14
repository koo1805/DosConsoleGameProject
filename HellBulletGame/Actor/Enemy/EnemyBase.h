#pragma once

#include <Actor/Actor.h>
#include <Actor/Enemy/EnemyStats.h>
#include <Render/Sprite.h>


class EnemyBase : public Craft::Actor
{
	TYPE_DECLARATIONS(EnemyBase, Actor)

	enum EnemyMoveState
	{
		Entering,
		Moving
	};

public:
	EnemyBase(
		const Craft::Sprite& sprite,
		const EnemyStats& stats,
		const Craft::Vector2& position
	);

	// Getter 함수
	inline int GetMaxHp() const { return stats.maxHp; }
	inline int GetScore() const { return stats.score; }

	void TakeDamage(int damage);

protected:
	// 함수 오버라이드
	virtual void Tick(float deltaTime) override;

	// 충돌 처리 함수 오버라이드
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

	// 이동 함수
	virtual void Move(float deltaTime);

	// 등장 패턴 함수
	void MoveDown(float deltaTime);
	void MoveDownToSide(float deltaTime);
	void MoveDiagonalLeft(float deltaTime);
	void MoveDiagonalRight(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);

	virtual void Fire();

protected:
	EnemyStats stats;

	EnemyMovePattern movePattern = EnemyMovePattern::Down;

	EnemyMoveState moveState = EnemyMoveState::Entering;

	int currentHp = 1;

	// 이동 처리를 위함 변수
	float xPosition = 0.0f;
	float yPosition = 0.0f;

	float sideStartY = 5.0f;
	float direction = 1.0f;

};