#include "PlayerProjectile.h"

using namespace Craft;

PlayerProjectile::PlayerProjectile(const Craft::Vector2& position, int damage)
	: ProjectileBase(
		"|",
		position,
		Color::Blue,
		ProjectileTeam::Player,
		ProjectileMoveType::Straight,
		30.0f,
		damage,
		0.0f,
		-1.0f
	)
{}
