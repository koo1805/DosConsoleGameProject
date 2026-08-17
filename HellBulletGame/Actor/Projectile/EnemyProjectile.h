#pragma once

#include <Actor/Projectile/ProjectileBase.h>

class EnemyProjectile : public ProjectileBase
{
	// 커스텀 타입 설정
	TYPE_DECLARATIONS(EnemyProjectile, ProjectileBase)

public:
	EnemyProjectile(
		const Craft::Vector2& position,
		float directionX = 0.0f,
		float directionY = 1.0f,
		float moveSpeed = 15.0f,
		int damage = 1
	);
};

