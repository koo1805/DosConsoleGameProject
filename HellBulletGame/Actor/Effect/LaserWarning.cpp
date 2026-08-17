#include "LaserWarning.h"

LaserWarning::LaserWarning(const std::string& image, const Craft::Vector2& position, float duration)
	: Actor(
		image,
		position,
		Craft::Color::Yellow
	)
{
	timer.SetTargetTime(duration);
}

LaserWarning::LaserWarning(const Craft::Sprite& sprite, const Craft::Vector2& position, float duration)
	: Actor(
		sprite,
		position,
		Craft::Color::Yellow
	)
{
	timer.SetTargetTime(duration);
}

void LaserWarning::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	timer.Tick(deltaTime);

	if (timer.IsTimeOut())
	{
		Destroy();
	}
}
