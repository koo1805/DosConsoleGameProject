#pragma once

#include <Core/Core.h>
#include <Math/Vector2.h>
#include <Math/Color.h>
#include <string>

namespace Craft
{
	class CRAFT_API HUDPanel
	{
	public:
		HUDPanel() = default;
		
		HUDPanel(int startX, int width, int height);

	public:
		// 경계선 구역 설정 함수
		void SetBounds(int startX, int width, int height);

		// 경계선 그리기 함수
		void DrawBorder() const;

		// HUD 내부 텍스트 그리는 함수
		void DrawText(
			const std::string& text,
			int localY,
			Color color = Color::White,
			int sortingOrder = 20
		) const;
		
		// localY = Vector2(startX + paddingX)
		void DrawText(
			const std::string& text,
			const Vector2& localPosition,
			Color color = Color::White,
			int sortingOrder = 20
		) const;

		// HUD 중간 분리선 그리는 함수
		void DrawSeparator(
			int localY,
			Color color = Color::White,
			int sortingOrder = 20
		);

		inline int GetStartX() const { return startX; }
		inline int GetContentX() const { return startX + paddingX; }
		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }

	private:
		// HUD 시작 위치
		int startX = 0;

		// HUD 너비 값
		int width = 0;

		// HUD 높이 값
		int height = 0;

		// HUD 내부 들여쓰기 값
		int paddingX = 2;
	};
}