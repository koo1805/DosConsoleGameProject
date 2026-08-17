#include "GameLevel.h"
#include <Engine/Engine.h>
#include <Render/Renderer.h>
#include <Level/MainMenuLevel.h>
#include <Actor/Player.h>
#include <Actor/Enemy/Spawner/EnemySpawner.h>
#include <Actor/Enemy/BossEnemy.h>
#include <Actor/Manager/GameManager.h>
#include <Actor/Manager/WaveManager.h>

#include <string>

using namespace Craft;

void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	// HUD 초기화
	const int hudStartX = Engine::Get().GetWidth() - Engine::Get().GetHUDWidth();
	const int hudWidth = Engine::Get().GetWidth() - hudStartX;
	const int hudHeight = Engine::Get().GetHeight();

	hudPanel.SetBounds(hudStartX, hudWidth, hudHeight);

	// 플레이어 액터 추가
	player = SpawnActor<Player>();

	// 적 생성기 액터 추가
	//SpawnActor<EnemySpawner>();
	SpawnActor<WaveManager>();

	// 게임 관리자 생성 및 플레이어 죽음 이벤트에 함수 등록
	gameManager = SpawnActor<GameManager>();
	if (gameManager)
	{
		gameManager->RegisterOnPlayerDead(
			[this]()
			{
				PlayerDied();
			}
		);

		gameManager->RegisterOnBossDead(
			[this]()
			{
				BossDied();
			}
		);
	}
}

void GameLevel::Tick(float deltaTime)
{
	// 게임 오버 시 레벨 업데이트 중지
	if (gameState == GameState::GameOverWait || gameState == GameState::GameClear)
	{
		// 타이머 시간 업데이트
		timer.Tick(deltaTime);
		if (timer.IsTimeOut())
		{
			Engine::Get().AddNewLevel<MainMenuLevel>();
		}

		return;
	}

	Level::Tick(deltaTime);
}

void GameLevel::Draw()
{
	Level::Draw();

	DrawHUD();

	if (gameState == GameState::GameClear)
	{
		DrawGameClear();
	}
}

void GameLevel::DrawHUD()
{
	hudPanel.DrawBorder();

	hudPanel.DrawText("Hell Bullet", 2, Color::Red);
	hudPanel.DrawSeparator(5);

	std::shared_ptr<BossEnemy> boss = gameManager->GetBoss();
	if (boss)
	{
		const std::string bossHpText =
			"BOSS HP : " +
			std::to_string(boss->GetHp()) +
			" / " +
			std::to_string(boss->GetMaxHp());

		hudPanel.DrawText(
			bossHpText,
			8,
			Craft::Color::Red
		);
	}

	hudPanel.DrawSeparator(21);
	std::shared_ptr<Player> currentPlayer = player.lock();

	if (currentPlayer)
	{
		const std::string hpText =
			"HP : " +
			std::to_string(currentPlayer->GetHp()) +
			" / " +
			std::to_string(currentPlayer->GetMaxHp());

		hudPanel.DrawText(
			hpText,
			23,
			Color::Green
		);
	}

	hudPanel.DrawSeparator(25);
	if (gameManager)
	{
		const std::string scoreText = "SCORE : " + std::to_string(gameManager->GetScore());

		hudPanel.DrawText(scoreText, 27);
	}

	hudPanel.DrawSeparator(29);

	hudPanel.DrawText("CONTROL", 31);
	hudPanel.DrawText("MOVE : W / A / S / D", 34);
	hudPanel.DrawText("SHOT : SPACE |  MODE : R", 36);
}

void GameLevel::DrawGameClear()
{
	const int playWidth = Engine::Get().GetPlayAreaWidth();
	const int centerY = Engine::Get().GetHeight() / 2;

	const std::string line1 = "====================";
	const std::string line2 = "     GAME CLEAR     ";
	const std::string line3 = "====================";

	const int startX = (playWidth - static_cast<int>(line1.length())) / 2;

	Renderer::Get().Submit(
		line1,
		Vector2(startX, centerY - 1),
		Color::Green,
		100
	);

	Renderer::Get().Submit(
		line2,
		Vector2(startX, centerY),
		Color::Green,
		100
	);

	Renderer::Get().Submit(
		line3,
		Vector2(startX, centerY + 1),
		Color::Green,
		100
	);
}

void GameLevel::PlayerDied()
{
	// 게임 상태 변경
	gameState = GameState::GameOverWait;

	// 게임 오버 타이머 시간 설정
	timer.SetTargetTime(gameOverWaitTime);
}

void GameLevel::BossDied()
{
	gameState = GameState::GameClear;

	timer.SetTargetTime(gameOverWaitTime);
}
