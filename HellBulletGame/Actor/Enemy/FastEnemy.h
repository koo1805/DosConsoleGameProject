#pragma once

#include <Actor/Enemy/EnemyBase.h>

class FastEnemy : public EnemyBase
{
	TYPE_DECLARATIONS(FastEnemy, EnemyBase)

public:
	FastEnemy(const Craft::Vector2& position, const Craft::Vector2& targetPosition, EnemyMovePattern movePattern, EnemyFirePattern firePattern);
};

