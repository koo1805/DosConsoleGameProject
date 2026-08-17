#pragma once

struct PlayerStats
{
	PlayerStats(
		int maxHp = 30,
		int attack = 1
	)
		: maxHp(maxHp),
		attack(attack)
	{}

	int maxHp = 30;
	int attack = 1;
};