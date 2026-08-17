#pragma once

#include <Actor/Actor.h>
#include <functional>

// 전방 선언
class BossEnemy;

class GameManager : public Craft::Actor
{
	// RTTI 등록
	TYPE_DECLARATIONS(GameManager, Actor)

public:
	// 플레이어가 죽을 때 실행할 함수 전달받기 위한 함수 포인터 타입 선언
	// 다른 객체에서 멤버 함수를 콜백으로 등록하도록 허용하기 위해 std::function 사용
	using OnPlayerDead = std::function<void()>;
	using OnBossDead = std::function<void()>;

public:
	GameManager() = default;
	~GameManager() = default;

	// 플레이어가 죽을 때 발생할 이벤트에 콜백 함수를 등록할 때 사용할 함수
	void RegisterOnPlayerDead(OnPlayerDead callback);
	void RegisterOnBossDead(OnBossDead callback);

	// 플레이어가 죽을 때 호출할 함수 - 플레이어 죽음 이벤트 발생
	void SetPlayerDead(const Craft::Vector2& deadPosition);
	void SetBossDead();

	// 점수 Getter / Setter
	inline int GetScore() const { return score; }
	inline void SetScore(int newScore) { score = newScore; }

	// 플레이어의 죽음 상태 플래그 Getter
	inline bool IsPlayerDead() const { return isPlayerDead; }

	// 플레이어가 죽은 위치
	inline Craft::Vector2 GetPlayerDeadPosition() const { return playerDeadPosition; }

	// 보스 설정
	inline void SetBoss(const std::shared_ptr<BossEnemy>& newBoss) { boss = newBoss; }
	inline std::shared_ptr<BossEnemy> GetBoss() const { return boss.lock(); }


private:
	// 점수
	int score = 0;

	// 플레이어의 죽음 여부 플래그
	bool isPlayerDead = false;

	bool isBossDead = false;

	// 플레이어가 죽은 위치를 저장할 변수
	Craft::Vector2 playerDeadPosition;

	// 플레이어 죽음 이벤트 콜백을 저장할 변수
	OnPlayerDead onPlayerDead = nullptr;

	OnBossDead onBossDead = nullptr;

	std::weak_ptr<BossEnemy> boss;
};