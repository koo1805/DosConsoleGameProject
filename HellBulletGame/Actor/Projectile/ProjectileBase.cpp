#include "ProjectileBase.h"
#include <Engine/Engine.h>

using namespace Craft;
ProjectileBase::ProjectileBase(const std::string& image,
	const Vector2& position,
	Color color,
	ProjectileTeam team,
	ProjectileMoveType moveType,
	float moveSpeed, int damage,
	float directionX,
	float directionY
)
	: Actor(image, position, color),
	team(team),
	moveType(moveType),
	moveSpeed(moveSpeed),
	damage(damage),
	directionX(directionX),
	directionY(directionY),
	xPosition(static_cast<float>(position.x)),
	yPosition(static_cast<float>(position.y))
{}

void ProjectileBase::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	Move(deltaTime);

	CheckOutScreen();

	SetPosition(Vector2(static_cast<int>(xPosition), static_cast<int>(yPosition)));
}

void ProjectileBase::Move(float deltaTime)
{
	switch (moveType)
	{
	case ProjectileMoveType::Straight:

		xPosition += directionX * moveSpeed * deltaTime;
		yPosition += directionY * moveSpeed * deltaTime;

		break;

	case ProjectileMoveType::Homing:

		// Todo: 나중에 유도탄 구현
		break;
	}
}

void ProjectileBase::CheckOutScreen()
{
	if (xPosition + GetWidth() < 0.0f)
	{
		Destroy();
		return;
	}

	if (xPosition >= Engine::Get().GetPlayAreaWidth())
	{
		Destroy();
		return;
	}

	if (yPosition + GetHeight() < 0.0f)
	{
		Destroy();
		return;
	}

	if (yPosition >= Engine::Get().GetHeight())
	{
		Destroy();
		return;
	}
}

