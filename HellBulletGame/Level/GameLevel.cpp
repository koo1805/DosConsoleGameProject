#include "GameLevel.h"
#include <Engine/Engine.h>
#include <Render/Renderer.h>
#include <Actor/Player.h>
#include <Actor/Enemy/Spawner/EnemySpawner.h>
#include <Actor/Manager/GameManager.h>

using namespace Craft;

void GameLevel::OnInitialized()
{
	Level::OnInitialized();

	// 플레이어 액터 추가
	SpawnActor<Player>();

	// 적 생성기 액터 추가
	//SpawnActor<EnemySpawner>();

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
	}
}

void GameLevel::Tick(float deltaTime)
{
	// 게임 오버 시 레벨 업데이트 중지
	if (gameState == GameState::GameOverWait)
	{
		// 타이머 시간 업데이트
		timer.Tick(deltaTime);
		if (timer.IsTimeOut())
		{
			Engine::Get().Quit();
		}

		return;
	}

	Level::Tick(deltaTime);
}

void GameLevel::Draw()
{
	Level::Draw();

	UIBorder();
}

void GameLevel::UIBorder()
{
	for (int ix = 0; ix < Engine::Get().GetHeight(); ix++)
	{
		Renderer::Get().Submit("#", Vector2(120, ix));
	}
}

void GameLevel::PlayerDied()
{
	// 게임 상태 변경
	gameState = GameState::GameOverWait;

	// 게임 오버 타이머 시간 설정
	timer.SetTargetTime(gameOverWaitTime);
}