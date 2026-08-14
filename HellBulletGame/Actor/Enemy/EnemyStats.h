#pragma once

enum EnemyMovePattern
{
	Down,
	DownToSide,
	DiagonalLeft,
	DiagonalRight,
	Left,
	Right
};

struct EnemyStats
{
	int maxHp = 1;
	float moveSpeed = 5.0f;
	float fireIntrval = 1.0f;
	int score = 10;
	int damage = 1;
	EnemyMovePattern movePattern = EnemyMovePattern::Down;
};