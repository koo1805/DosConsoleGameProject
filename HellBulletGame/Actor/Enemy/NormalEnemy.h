#pragma once

#include <Actor/Enemy/EnemyBase.h>

class NormalEnemy : public EnemyBase
{
	TYPE_DECLARATIONS(NormalEnemy, EnemyBase)

public:
	NormalEnemy(const Craft::Vector2& position, EnemyMovePattern movePattern);

private:
	
};

