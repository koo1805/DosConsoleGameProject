#include "NormalEnemy.h"
#include <Math/Vector2.h>
#include <Engine/Engine.h>

NormalEnemy::NormalEnemy(const Craft::Vector2& position, EnemyMovePattern movePattern) : EnemyBase(
	Craft::Sprite({
			"|Y|",
			" V "
		}),
	EnemyStats{ 3, 5.0f, 1.0f, 10, 1, movePattern},
	position
)
{
}