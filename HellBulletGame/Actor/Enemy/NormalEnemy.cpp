#include "NormalEnemy.h"
#include <Math/Vector2.h>

NormalEnemy::NormalEnemy(const Craft::Vector2& position, const Craft::Vector2& targetPosition, EnemyMovePattern movePattern, EnemyFirePattern firePattern)
	: EnemyBase(
		Craft::Sprite({			// 문자열 배열
				"<|T|>",
				"  V  "
			}),
	EnemyStats{ 
			1,					// 체력
			5.0f,				// 이동 속도
			2.0f,				// 발사 타이머
			10,					// 점수
			1,					// 데미지
			movePattern,		// 이동 방식
			firePattern,		// 발사 방식
			targetPosition		// 목표 위치
		},
	position
)
{
}