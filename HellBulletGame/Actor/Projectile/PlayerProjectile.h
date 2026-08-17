#pragma once

#include <Actor/Projectile/ProjectileBase.h>

class PlayerProjectile : public ProjectileBase
{
	TYPE_DECLARATIONS(PlayerProjectile, ProjectileBase)

public:
	PlayerProjectile(const Craft::Vector2& position, int damage);
};