#include "Sprite.h"
#include <algorithm>

namespace Craft
{
	// {}로 한번에 그릴때
	Sprite::Sprite(std::initializer_list<std::string> lines) : lines(lines)
	{ }
	
	// 밖에서 push_back을 통해 문자열 추가 가능
	Sprite::Sprite(const std::vector<std::string>& lines) : lines(lines)
	{ }
	
	int Sprite::GetWidth() const
	{
		int maxWidth = 0;

		// 배열 내에 문자열을 읽어 들임
		for (const std::string& line : lines)
		{
			// 문자열을 비교해서 더 큰 값을 가져옴 -> 제일 큰 값이 너비가 됨
			maxWidth = std::max(maxWidth, static_cast<int>(line.length()));
		}

		return maxWidth;
	}
	
	int Sprite::GetHeight() const
	{
		// 줄 개수 = 높이
		return static_cast<int>(lines.size());
	}
}