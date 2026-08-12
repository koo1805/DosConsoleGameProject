#include "Level.h"

namespace Craft
{
	Level::Level()
	{

	}
	
	Level::~Level()
	{

	}
	
	void Level::OnInitialized()
	{
		//레벨 초기화 처리 플래그 설정	>  초기화 됐다고 설정
		hasInitialized = true;
	}
	
	void Level::BeginPlay()
	{
		// 액터 초기화시 1번만 호출되는 이벤트
		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			// 검증 - 이미 BeginPlay가 호출된 액터는 건너뛰기
			if (actor->HasBeganPlay())
			{
				continue;
			}

			// BeginPlay 이벤트 호출
			actor->BeginPlay();
		}
	}
	
	void Level::Tick(float deltaTime)
	{
		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			// 비활성화된 액터는 건너뛰기
			if (!actor->IsActive())
			{
				continue;
			}

			// Tick 이벤트 호출
			actor->Tick(deltaTime);
		}
	}
	
	void Level::Draw()
	{
		for (const std::shared_ptr<Actor>& actor : actorList)
		{
			// 비활성화된 액터는 건너뛰기
			if (!actor->IsActive())
			{
				continue;
			}

			// Draw 이벤트 호출
			actor->Draw();
		}
	}

	void Level::ProcessAddAndDestroyActors()
	{
		// 액터 제거 처리
		// 이터레이터 기반 루프
		for (auto iterator = actorList.begin(); iterator != actorList.end();)
		{
			// 제거 요청 여부 확인
			if ((*iterator)->HasExpired())
			{
				iterator = actorList.erase(iterator);
				continue;
			}

			// 다음 순번을 처리하기 위해 이터레이터(반복자, 포인터) 증가 처리
			++iterator;
		}

		// 액터 추가 처리

		// 추가 요청된 목록이 있는지 확인
		if (addRequestedActorList.empty())
		{
			return;
		}

		// 추가 요청 처리
		for (const auto& actor : addRequestedActorList)
		{
			actorList.emplace_back(actor);
		}

		//정리
		addRequestedActorList.clear();
	}
	void Level::SavePreviousActorStates()
	{
		// 액터 순회하면서 이전 상태 저장 처리
		for (const auto& actor : actorList)
		{
			// 액터가 활성화 되지 않았으면 무시
			if (!actor->IsActive())
			{
				continue;
			}

			// 상태 저장
			actor->SavePreviousState();
		}
	}
}