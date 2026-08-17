#include "FastEnemy.h"
#include <Math/Vector2.h>

FastEnemy::FastEnemy(const Craft::Vector2& position, const Craft::Vector2& targetPosition, EnemyMovePattern movePattern, EnemyFirePattern firePattern)
	: EnemyBase(
		Craft::Sprite({
				" o@o ",
				"<_O_>"
			}),
		EnemyStats{ 3, 12.0f, 1.2f, 20, 1, movePattern, firePattern, targetPosition },
		position
	)
{}
