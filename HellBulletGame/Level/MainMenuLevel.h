#pragma once
#include <Level/Level.h>
#include <string>

// 메인 메뉴 아이템 구조체
struct MainMenuItem
{
	// 메뉴 선택 시 실행할 로직 저장을 위한 함수 포인터
	using OnSelected = void(*)();

	// 메뉴 텍스트
	std::string text;

	// 메뉴를 선택했을때 실행할 로직
	OnSelected onSelected = nullptr;

	// 생성자
	MainMenuItem(const std::string& text, OnSelected onSelected)
		: text(text), onSelected(onSelected)
	{ }

	~MainMenuItem() = default;
};

class MainMenuLevel : public Craft::Level
{
public:
	MainMenuLevel();
	~MainMenuLevel() = default;

private:
	// 함수 오버라이딩
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	// 타이틀 로드 함수
	void LoadTitle(const std::string& fileName);

private:
	// 현재 활성화된 메뉴 아이템 인덱스
	int currentIndex = 0;

	// 선택된 메뉴 아이템의 색상
	Craft::Color selectedColor = Craft::Color::Yellow;

	// 미선택된 메뉴 아이템의 색상
	Craft::Color unSelectedColor = Craft::Color::White;

	// 메뉴 아이템 배열
	std::vector<std::unique_ptr<MainMenuItem>> itemList;
};

