#include "EnemyProjectile.h"

using namespace Craft;

EnemyProjectile::EnemyProjectile(const Vector2& position,
	float directionX,
	float directionY,
	float moveSpeed,
	int damage
)
	: ProjectileBase(
		"o",
		position,
		Color::Yellow,

		ProjectileTeam::Enemy,
		ProjectileMoveType::Straight,

		moveSpeed,
		damage,

		directionX,
		directionY
	)
{}
