#pragma once

#include <Actor/Enemy/EnemyBase.h>
#include <Math/Vector2.h>

class EightwayEnemy : public EnemyBase
{
	TYPE_DECLARATIONS(EightwayEnemy, EnemyBase)

public:
	EightwayEnemy(const Craft::Vector2& position, const Craft::Vector2& targetPosition, EnemyMovePattern movePattern, EnemyFirePattern firePattern);
};

