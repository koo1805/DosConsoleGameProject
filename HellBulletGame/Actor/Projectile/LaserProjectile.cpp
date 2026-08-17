#include "LaserProjectile.h"
#include <Actor/Player.h>

LaserProjectile::LaserProjectile(const std::string& image, const Craft::Vector2& position, float duration, int damage)
	:Actor(
		image,
		position,
		Craft::Color::Red
	),
	damage(damage)
{
	timer.SetTargetTime(duration);
}

LaserProjectile::LaserProjectile(const Craft::Sprite & sprite, const Craft::Vector2 & position, float duration, int damage)
	: Actor(
		sprite,
		position,
		Craft::Color::Red
	),
	damage(damage)
{
	timer.SetTargetTime(duration);
}

void LaserProjectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	timer.Tick(deltaTime);

	if (timer.IsTimeOut())
	{
		Destroy();
	}
}

void LaserProjectile::OnCollision(const std::shared_ptr<Craft::Actor>&other)
{
	super::OnCollision(other);

	if (hasHitPlayer)
	{
		return;
	}


	if (!other->IsTypeOf<Player>())
	{
		return;
	}

	std::shared_ptr<Player> player = Craft::Cast<Player>(other);

	if (!player)
	{
		return;
	}

	// 아래에서 설명할 Player 함수
	player->TakeDamage(damage);

	hasHitPlayer = true;
}
