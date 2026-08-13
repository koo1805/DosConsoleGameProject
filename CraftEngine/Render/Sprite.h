#pragma once

#include <Core/Core.h>
#include <vector>
#include <string>
#include <initializer_list>

namespace Craft
{
	class CRAFT_API Sprite
	{
	public:
		Sprite() = default;

		Sprite(std::initializer_list<std::string> lines);
		Sprite(const std::vector<std::string>& lines);

		// 배열내의 문자열의 너비 구하는 함수
		int GetWidth() const;

		// 문자그림의 높이 구하는 함수
		int GetHeight() const;

		// 비어있는지 확인하는 함수
		inline bool IsEmpty() const { return lines.empty(); }

		// 문자열을 가져오는 함수
		inline const std::vector<std::string>& GetLines() const { return lines; }

	private:
		std::vector<std::string> lines;
	};

}