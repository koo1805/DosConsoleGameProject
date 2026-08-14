#pragma once

#include <Actor/Enemy/EnemyBase.h>
#include <Math/Vector2.h>

class NormalEnemy : public EnemyBase
{
	TYPE_DECLARATIONS(NormalEnemy, EnemyBase)

public:
	NormalEnemy(const Craft::Vector2& position, EnemyMovePattern movePattern);
};

