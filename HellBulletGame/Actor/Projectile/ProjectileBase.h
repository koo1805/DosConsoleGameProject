#pragma once

#include <Actor/Actor.h>

enum class ProjectileTeam
{
	Player,
	Enemy
};

enum class ProjectileMoveType
{
	Straight,
	Homing
};

class ProjectileBase : public Craft::Actor
{
	TYPE_DECLARATIONS(ProjectileBase, Actor)

public:
	ProjectileBase(
		const std::string& image,
		const Craft::Vector2& position,
		Craft::Color color,
		ProjectileTeam team,
		ProjectileMoveType moveType,
		float moveSpeed,
		int damage,
		float directionX,
		float directionY
	);

	inline int GetDamage() const
	{
		return damage;
	}

	inline ProjectileTeam GetTeam() const
	{
		return team;
	}

	inline ProjectileMoveType GetMoveType() const
	{
		return moveType;
	}

protected:
	virtual void Tick(float deltaTime) override;

	// 이동
	virtual void Move(float deltaTime);

	// 화면 밖 검사
	void CheckOutScreen();

protected:
	ProjectileTeam team = ProjectileTeam::Enemy;
	ProjectileMoveType moveType = ProjectileMoveType::Straight;

	float moveSpeed = 15.0f;

	int damage = 1;

	float directionX = 0.0f;
	float directionY = 1.0f;

	float xPosition = 0.0f;
	float yPosition = 0.0f;
};

