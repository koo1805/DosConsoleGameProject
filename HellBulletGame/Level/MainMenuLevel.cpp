#include "MainMenuLevel.h"
#include <Engine/Engine.h>
#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Level/GameLevel.h>
#include <cassert>
//#include <iostream>

using namespace Craft;

MainMenuLevel::MainMenuLevel()
{
	// 메뉴 아이템 생성
	itemList.emplace_back(
		std::make_unique<MainMenuItem>(
			"Game Start",
			[]()
			{
				// Todo: 게임 레벨로 이동
				Engine::Get().AddNewLevel<GameLevel>();
			}
		)
	);
	
	itemList.emplace_back(
		std::make_unique<MainMenuItem>(
			"Quit Game",
			[]()
			{
				Engine::Get().Quit();
			}
		)
	);
}

void MainMenuLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	// 배열의 원소 개수
	const int length = static_cast<int>(itemList.size());

	// 입력 처리
	if (Input::Get().GetKeyDown(VK_UP) || Input::Get().GetKeyDown('W'))
	{
		// 인덱스 돌리기 (-방향)
		currentIndex = (currentIndex - 1 + length) % length;
	}
	
	if (Input::Get().GetKeyDown(VK_DOWN) || Input::Get().GetKeyDown('S'))
	{
		// 인덱스 돌리기 (+방향)
		currentIndex = (currentIndex + 1) % length;
	}

	// 엔터 입력 처리 -> 현재 선택된 메뉴의 로직을 실행
	if (Input::Get().GetKeyDown(VK_RETURN) || Input::Get().GetKeyDown(VK_SPACE))
	{
		// Assert
		assert(currentIndex >= 0 && currentIndex < (int)itemList.size() && itemList[currentIndex]->onSelected);

		// 메뉴 아이템에 저장된 로직 실행
		itemList[currentIndex]->onSelected();
	}
}

void MainMenuLevel::Draw()
{
	Renderer::Get().Submit("Game", Vector2::Zero);

	// 메뉴 아이템 그리기
	const int count = static_cast<int>(itemList.size());
	for (int ix = 0; ix < count; ++ix)
	{
		// 선택/미선택된 아이템 색상 처리
		Color textColor = (ix == currentIndex) ? selectedColor : unSelectedColor;

		int width = static_cast<int>((itemList[ix]->text).size());

		// 아이템 그리기
		Renderer::Get().Submit(
			itemList[ix]->text,
			Vector2((Engine::Get().GetWidth() - width) / 2, (Engine::Get().GetHeight() / 2) + ix),
			textColor
		);
	}
}

void MainMenuLevel::LoadTitle(const std::string& fileName)
{
	// 최종 경로 조립
	std::string path = std::string("../Assets/Titles/") + fileName;

	// 파일 열기 (C-Style)
	FILE* file = nullptr;
	fopen_s(&file, path.c_str(), "rt");
	if (!file)
	{
		assert(false && "failed to open a title file.");
		return;
	}
	// 파일의 내용을 저장할 버퍼(데이터 저장공간) 확인
	// 파일 길이 확인 
	// -> 파일 위치를 제일 뒤로 이동 시킨 다음, 해당 위치 값 읽기
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);

	// 파일 제일 끝위치를 구한 다음에는 다시 처음으로 되돌리기
	rewind(file);

	// 앞에서 구한 위치를 사용해서 버퍼 생성
	char* buffer = new char[fileSize] {};

	// 데이터 읽기(파일 읽기)
	size_t readSize = fread(buffer, sizeof(char), fileSize, file);

	// 어서트
	assert(readSize > 0 && "No data is in the title file.");

	// Todo: 타이틀 파일 불러오기?

	fclose(file);
	file = nullptr;
}
