#include "HUDPanel.h"
#include <Render/Renderer.h>

namespace Craft
{
	HUDPanel::HUDPanel(int startX, int width, int height)
		: startX(startX), width(width), height(height)
	{ }

	void HUDPanel::SetBounds(int startX, int width, int height)
	{
		this->startX = startX;
		this->width = width;
		this->height = height;
	}

	void HUDPanel::DrawBorder() const
	{
		// 예외 처리
		if (width <= 0 || height <= 0)
		{
			return;
		}

		const int endX = startX + width - 1;
		const int topY = 0;
		const int bottomY = height - 1;

		// 왼쪽 세로 구분선
		for (int y = 0; y < height; ++y)
		{
			Renderer::Get().Submit(
				"|",
				Vector2(startX, y),
				Color::White,
				20
			);
		}
		
		// 오른쪽 세로 구분선
		for (int y = 0; y < height; ++y)
		{
			Renderer::Get().Submit(
				"|",
				Vector2(endX, y),
				Color::White,
				20
			);
		}

		// 위 | 아래 가로 구분선
		for (int x = startX; x <= endX; ++x)
		{
			Renderer::Get().Submit(
				"_",
				Vector2(x, topY),
				Color::White,
				20
			);

			Renderer::Get().Submit(
				"_",
				Vector2(x, bottomY),
				Color::White,
				20
			);
		}
	}

	void HUDPanel::DrawText(const std::string& text, int localY, Color color, int sortingOrder) const
	{
		DrawText(text, Vector2(paddingX, localY), color, sortingOrder);
	}

	void HUDPanel::DrawText(const std::string& text, const Vector2& localPosition, Color color, int sortingOrder) const
	{
		// 예외 처리
		if (text.empty())
		{
			return;
		}

		// HUD 내부에 그릴 텍스트 위치 설정
		const Vector2 screenPosition(startX + localPosition.x, localPosition.y);

		// HUD내부에 텍스트 그림
		Renderer::Get().Submit(text, screenPosition, color, sortingOrder);
	}

	// HUD 중간 분리선 그리는 함수
	void HUDPanel::DrawSeparator(int localY, Color color, int sortingOrder)
	{
		const int separatorStartX = startX + paddingX;
		const int separatorEndX = startX + width - paddingX - 1;

		for (int x = separatorStartX; x < separatorEndX; ++x)
		{
			Renderer::Get().Submit(
				"-",
				Vector2(x, localY),
				color,
				sortingOrder
			);
		}
	}
}