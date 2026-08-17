#pragma once

#include <Math/Vector2.h>

enum class EnemyMovePattern
{
	Down,
	DownToSide,
	DiagonalLeft,
	DiagonalRight,
	Left,
	Right,
	TargetStop,
	Zigzag
};

enum class EnemyFirePattern
{
	None,
	Straight,
	AimPlayer,
	Burst,
	EightWay
};

struct EnemyStats
{
	int maxHp = 1;
	float moveSpeed = 5.0f;
	float fireInterval = 1.0f;
	int score = 10;
	int damage = 1;
	EnemyMovePattern movePattern = EnemyMovePattern::Down;
	EnemyFirePattern firePattern = EnemyFirePattern::Straight;
	Craft::Vector2 targetPosition;
};