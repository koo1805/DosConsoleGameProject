#pragma once

#include <Core/Core.h>

namespace Craft
{
	// 입력 관련 기능 제공
	class CRAFT_API Input
	{
		//Engine 클래스에서만 특정 함수에 접근 가능하도록 함
		friend class Engine;

		// 키 입력 상태 구조체
		struct KeyState
		{
			// 현재 프레임에 키가 눌렸는지 여부 플래그
			bool isKeyDown = false;

			// 이전 프레임에 키가 눌렸는지 여부 플래그
			bool wasKeyDown = false;
		};

	public:
		Input();
		~Input() = default;

		// 키 눌림/해제 여부 확인 함수
		// 이전 프레임에는 키가 안눌렸다가 현재 프레임에 눌렸을때 1번만 호출
		// 이전 프레임에 안 눌렸다가 이번 프레임에 눌리면 true 반환
		bool GetKeyDown(int keyCode) const;
		
		// 이전 프레임에는 키가 눌렸다가 현재 프레임에 해제되면 1번만 호출
		// 이전 프레임에 눌렸다가 이번 프레임에 안 눌리면 true 반환
		bool GetKeyUp(int keyCode) const;
		
		// 현재 프레임에 입력이 눌리면 반복해서 true를 반환하는 함수
		bool GetKey(int keyCode) const;

		// (싱글톤)외부에서 접근이 가능하도록 해주는 함수
		static Input& Get();

	private:
		// 현재 프레임에 특정 키 입력이 발생했는지를 처리하는 함수
		void ProcessInput();

		// 이전 프레임의 키 눌림 상태를 저장하는 함수
		void SavePreviousStates();

	private:
		// 가상 키의 수 (=처리할 키의 수)
		const int keyCount = 256;

		// 키 입력 상태 관리 배열
		KeyState keyStates[256] = { };

		// (싱글톤)전역 접근이 가능하도록 변수 추가
		static Input* instance;
	};
}