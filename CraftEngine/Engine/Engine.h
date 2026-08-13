#pragma once

#include <Core/Core.h>
#include <memory>		// 스마트 포인터 사용을 위해

// CraftEngine 프로젝트 안의  클래스는 Craft 네임 스페이스 사용.
namespace Craft
{
	// 전방 선언 (컴파일 시간 단축)
	class Level;
	class Input;
	class Renderer;
	class CollisionSystem;

	// 메인 엔진 클래스
	// 엔진 루프를 제공
	// 게임 엔진의 핵심 기능 제공
	class CRAFT_API Engine
	{
		// 엔진 설정 (데이터)
		struct Setting
		{
			// 목표 프레임 수 (초당 프레임)
			float framerate = 0.0f;

			// 사용할 콘솔 화면 너비
			int width = 0;

			// 사용할 콘솔 화면 높이
			int height = 0;
			
			// 사용할 콘솔 화면 내 HUD 너비
			int hudWidth = 0;
		};

	public:
		Engine();
		virtual ~Engine();

		// 엔진( 게임 루프 ) 실행 함수
		void Run();

		// 엔진 종료 함수
		void Quit();

		// 레벨 추가 요청 함수
		// 1. std::is_base_of 하는일이 무엇인지
		// 2. std::enable_if_t 하는일이 무엇인지
		// 3. typename = std::enable_if_t<std::is_base_of<Level, T>::value>>
		template<typename T,
			typename = std::enable_if_t < std::is_base_of<Level, T>::value>>
		void AddNewLevel()
		{
			// 추가 요청 레벨 객체 생성
			nextLevel = std::make_shared<T>();
		}

		// 전역(싱글톤) 접근 함수
		static Engine& Get();

		// Getter
		inline int GetWidth() const { return setting.width; }
		inline int GetHeight() const { return setting.height; }
		inline int GetHUDWidth() const { return setting.hudWidth; }

		inline int GetPlayAreaWidth() const { return setting.width - setting.hudWidth; }

	protected:
		// 입력 처리 함수 (입력 폴링)
		void ProcessInput();

		// [ 초기화 함수 ]
		//	 > 레벨 초기화 함수
		void OnInitialized();

		// 게임 플레이 이벤트 함수

		// - 게임 플레이 초기화 함수
		//	 > 액터 초기화 함수
		void BeginPlay();

		// 게임 플레이 업데이트 함수
		void Tick(float deltaTime);

		// 화면에 그리는 함수
		void Draw();

		// 충돌 처리 함수
		void ProcessCollision();

		// 이전 입력을 저장하는 함수
		void SavePreviousInputStates();

		// 엔진 종료 시 정리 함수
		void Shutdown();

		// 엔진 설정 로드 함수				->	C 스타일 입출력/문자열 자르기 기능
		void LoadEngineSetting();

		// 엔진 설정 로드 함수				->	C++ 스타일 입출력/문자열 자르기 기능
		void LoadEngineSetting02();

	protected:
		// 엔진 종료 요청 여부 플래그
		bool isQuit = false;

		// 엔진 설정 함수
		Setting setting;

		// 전역 접근이 가능하도록 변수 선언		| 싱글톤을 위한 전역 객체
		static Engine* instance;

		//메인 레벨
		std::shared_ptr<Level> mainLevel;

		//추가 요청된 레벨
		std::shared_ptr<Level> nextLevel;

		// 입력 시스템
		std::unique_ptr<Input> input;

		// 렌더러
		std::unique_ptr<Renderer> renderer;

		// 충돌 시스템 객체
		std::unique_ptr<CollisionSystem> collisionSystem;
	};
}