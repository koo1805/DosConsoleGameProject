#include "PlayerProjectile.h"

using namespace Craft;

PlayerProjectile::PlayerProjectile(const Craft::Vector2& position)
	: Actor("|", position, Color::Blue),
	yPosition(static_cast<float>(position.y)), xPosition(static_cast<float>(position.x))
{ 
	timer.SetTargetTime(curveTime);
}

void PlayerProjectile::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 아래에서 위로 이동 처리
	yPosition -= moveSpeed * deltaTime;
	//timer.Tick(deltaTime);
	//if (!timer.IsTimeOut())
	//{
	//	xPosition += moveSpeed * deltaTime;
	//}
	//else
	//{
	//	xPosition -= moveSpeed * deltaTime;
	//}
	// 좌표 검사(화면 벗어났는지 확인)
	if (yPosition < 0.0f)
	{
		// 삭제 처리
		Destroy();
		return;
	}

	// 위치 값 설정 및 갱신
	Vector2 newPosition = GetPosition();
	newPosition.y = static_cast<int>(yPosition);
	newPosition.x = static_cast<int>(xPosition);
	SetPosition(newPosition);
}
