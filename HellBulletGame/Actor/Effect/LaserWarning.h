#pragma once

#include <Actor/Actor.h>
#include <Util/Timer.h>

class LaserWarning : public Craft::Actor
{
	TYPE_DECLARATIONS(LaserWarning, Actor)

public:
	LaserWarning(
		const std::string& image,
		const Craft::Vector2& position,
		float duration
	);

	LaserWarning(
		const Craft::Sprite& sprite,
		const Craft::Vector2& position,
		float duration
	);

protected:
	virtual void Tick(float deltaTime) override;

private:
	Craft::Timer timer;
};

