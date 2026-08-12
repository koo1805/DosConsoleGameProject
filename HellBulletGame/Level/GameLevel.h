#pragma once

#include <Level/Level.h>
#include <Util/Timer.h>

// 전방 선언
class GameManager;

class GameLevel : public Craft::Level
{
	// 게임 상태 열거형
	enum class GameState
	{
		Playing,
		GameOverWait
	};

public:
	GameLevel() = default;
	~GameLevel() = default;

private:
	// 함수 오버라이딩
	virtual void OnInitialized() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// UI를 구분할 경계선 함수
	void UIBorder();

	// 플레이어가 죽었을 때 실행될 콜백 함수
	void PlayerDied();

private:
	// 게임 관리자 클래스
	std::shared_ptr<GameManager> gameManager;

	// 게임 상태
	GameState gameState = GameState::Playing;

	// 게임 오버 대기 타이머
	Timer timer;

	// 게임 오버 대기 시간 (단위: 초)
	const float gameOverWaitTime = 2.0f;
};

