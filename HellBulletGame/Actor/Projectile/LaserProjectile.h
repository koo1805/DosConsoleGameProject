#pragma once

#include <Actor/Actor.h>
#include <Util/Timer.h>

class LaserProjectile : public Craft::Actor
{
	TYPE_DECLARATIONS(LaserProjectile, Actor)

public:
	LaserProjectile(
		const std::string& image,
		const Craft::Vector2& position,
		float duration,
		int damage
	);
	
	LaserProjectile(
		const Craft::Sprite& sprite,
		const Craft::Vector2& position,
		float duration,
		int damage
	);

protected:
	virtual void Tick(float deltaTime) override;

	virtual void OnCollision(const std::shared_ptr<Craft::Actor>& other) override;

private:
	Craft::Timer timer;

	int damage = 1;

	bool hasHitPlayer = false;
};

