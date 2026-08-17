#include "EightwayEnemy.h"

EightwayEnemy::EightwayEnemy(const Craft::Vector2& position, const Craft::Vector2& targetPosition, EnemyMovePattern movePattern, EnemyFirePattern firePattern)
	: EnemyBase(
		Craft::Sprite({
				" o-o ",
				"o:O:o",
				" o-o "
			}),
		EnemyStats(10, 10.0f, 1.8f, 50, 1, movePattern, firePattern, targetPosition),
		position
	)
{}
