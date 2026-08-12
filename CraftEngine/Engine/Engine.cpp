#include "Engine.h"
#include <Level/Level.h>
#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Physics/CollisionSystem.h>

#include <Windows.h>
#include <iostream>
#include <cassert>

#include <fstream>
#include <sstream>

namespace Craft
{
	// 전역 변수 초기화
	Engine* Engine::instance = nullptr;

	Engine::Engine()
	{
		//assert
		assert(!instance && "instance is not null");

		// instance 초기화
		instance = this;

		// 엔진 설정 로드
		LoadEngineSetting();

		// 입력 객체 생성
		input = std::make_unique<Input>();

		// 렌더러 객체 생성.
		renderer = std::make_unique<Renderer>(Vector2(setting.width, setting.height));

		// 콜리전 시스템 객체 생성
		collisionSystem = std::make_unique<CollisionSystem>();
	}
	
	Engine::~Engine()
	{
		instance = nullptr;
	}
	
	void Engine::Run()
	{
		// 윈도우즈가 제공하는 고해상도 타이머(하드웨어 타이머)

		// QueryPerformanceFrequency : 타이머의 해상도
		// 예시 : 밀리세컨드 - 1/1000초 -> 해상도 1000
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		// 현재 시간 확인
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		// 프레임 시간 계산을 위한 변수
		int64_t current = counter.QuadPart;
		int64_t previous = current;

		// 고정 프레임으로 만들기 위한 값
		float oneFrameTime = 1.0f / setting.framerate;

		// 엔진 루프
		while (true)
		{
			// 종료 조건 처리.
			if (isQuit)
			{
				break;
			}

			// 프레임 처리

			// 입력 처리
			ProcessInput();

			// 프레임 시간 계산
			// 1. 현재 시간 읽기
			QueryPerformanceCounter(&counter);

			// 현재 시간 저장
			current = counter.QuadPart;

			// 2. (현재 시간 - 이전 시간) / 시간 단위(해상도).
			//    -> 초단위로 변환.
			//    예) 밀리세컨드(1/1000초). 200밀리세컨드 -> 초?
			float deltaTime = static_cast<float>(current - previous) / static_cast<float>(frequency.QuadPart);

			// 고정 프레임 처리
			// 프레임 사이에 걸린 시간이 목표 시간보다 더 많이 지났으면
			// 프레임 처리
			if (deltaTime >= oneFrameTime)
			{
				// 레벨 초기화 이벤트 함수				| 게임 이벤트 함수 호출.
				OnInitialized();

				// 레벨의 액터 초기화 이벤트 함수		| 게임 이벤트의 초기화 함수(1번만 호출)
				BeginPlay();

				// 레벨의 액터 업데이트 함수			| 게임 업데이트
				Tick(deltaTime);

				// 충돌 처리
				ProcessCollision();

				// 업데이트된 결과를 화면에 그리는 함수	| 화면 그리기
				Draw();

				// 이 위까지 호출이 완료되면 프레임 처리 완료됨

				// 레벨 전환 처리
				if (nextLevel)
				{
					//기존 레벨 정리
					if (mainLevel)
					{
						mainLevel.reset();
					}

					// 이전 프레임에 전환 요청된 레벨을 메인 레벨로 설정
					mainLevel = nextLevel;

					// 포인터 정리
					nextLevel.reset();
				}

				// 추가/제거 요청된 액터 정리
				if (mainLevel)
				{
					mainLevel->ProcessAddAndDestroyActors();

					// 액터의 이전 상태 저장
					mainLevel->SavePreviousActorStates();
				}

				// 처리된 입력을 이전 프레임 입력으로 저장
				SavePreviousInputStates();

				//현재 시간을 이전 시간으로 저장		| 이전 프레임 시간 기록
				previous = current;
			}
		}

		//// 종료 처리 함수 호출
		Shutdown();
	}
	
	void Engine::Quit()
	{
		// 엔진 종료 플래그 설정.
		isQuit = true;
	}

	Engine& Engine::Get()
	{
		// assert
		// 필수로 통과해야하는 조건이 있을 때 사용
		// 디버그 모드에서만 동작
		assert(instance && "instance is null");

		// 전역 객체 반환
		return *instance;
	}
	
	void Engine::ProcessInput()
	{
		assert(input && "input should not be null here");
		if (!input)
		{
			return;
		}

		input->ProcessInput();
	}
	
	void Engine::OnInitialized()
	{
		// 레벨 초기화 처리.
		// 예외 처리.
		if (!mainLevel || mainLevel->HasInitialized())
		{
			return;
		}

		// 초기화 이벤트 호출
		mainLevel->OnInitialized();
	}
	
	void Engine::BeginPlay()
	{
		if (!mainLevel)
		{
			return;
		}

		// 레벨에 이벤트 전달
		mainLevel->BeginPlay();
	}
	
	void Engine::Tick(float deltaTime)
	{
		// 레벨에 이벤트 전달
		if (!mainLevel)
		{
			return;
		}

		mainLevel->Tick(deltaTime);
	}
	
	void Engine::Draw()
	{
		// 레벨에 이벤트 전달
		if (!mainLevel)
		{
			return;
		}
		mainLevel->Draw();

		// 렌더러에 Draw 이벤트 호출.
		if (!renderer)
		{
			return;
		}

		renderer->Draw();
	}

	void Engine::ProcessCollision()
	{
		// 예외 처리
		if (!mainLevel || !collisionSystem)
		{
			return;
		}

		// 레벨의 액터 목록을 충돌 시스템에 전달해 처리 진행 - 충돌 처리
		// 의존성 주입 (Defendency Injection)
		collisionSystem->ProcessCollision(mainLevel->actorList);
	}
	
	void Engine::SavePreviousInputStates()
	{
		assert(input && "input should not null here");
		if (!input)
		{
			return;
		}

		input->SavePreviousStates();
	}
	
	void Engine::Shutdown()
	{

	}

	// C 스타일 입출력/문자열 자르기 기능
	void Engine::LoadEngineSetting()
	{
		// 파일 열기 (개행 문자 처리를 쉽게 텍스트 모드로 열기)
		FILE* file = nullptr;
		fopen_s(&file, "../Config/Setting.txt", "rt");

		// 예외처리
		if (!file)
		{
			std::cout << "Failed to open engine setting file.\n";

			// 디버그 모드에서 강제 중단 시키는 기능
			__debugbreak();
			return;
		}

		// 데이터 읽어오기
		const int bufferSize = 2048;
		char buffer[bufferSize] = {};

		size_t readSize
			= fread(buffer, sizeof(char), bufferSize, file);

		// 값 저장을 위해 서식 해석 (파싱-Parsing)
		// 문자열 자르기(Split)
		char* context = nullptr;
		char* token = nullptr;
		// 파일에서 읽은 전체 문자열을 개행(\n)문자 기준으로 처음 자르기
		token = strtok_s(buffer, "\n", &context);

		// 반복해서 자르기
		while (token)
		{
			// 공백 전까지 읽은 문자열을 저장할 변수
			char key[15] = {};

			// 포맷을 지정한 문자열 읽기
			// 공백 문자를 만나면 그 전까지 읽어서 저장
			sscanf_s(token, "%s", key, 15);

			// 키 값을 비교해서 값 설정
			if (strcmp(key, "framerate") == 0)
			{
				sscanf_s(token, "framerate = %f", &setting.framerate);
			}
			else if (strcmp(key, "width") == 0)
			{
				sscanf_s(token, "width = %d", &setting.width);
			}
			else if (strcmp(key, "height") == 0)
			{
				sscanf_s(token, "height = %d", &setting.height);
			}

			// 나머지 문자열 자르기(개행 문자 기준으로)
			token = strtok_s(nullptr, "\n", &context);
		}

		// 파일 닫기
		fclose(file);
		file = nullptr;
	}
	
	// C++ 스타일 입출력/문자열 자르기 기능
	void Engine::LoadEngineSetting02()
	{
		// 엔진 설정 파일 열기
		std::ifstream fileCpp("../Config/Setting.txt");

		// 정상적으로 열렸는지 확인
		assert(fileCpp.is_open());

		// 라인(Line) 별로 읽기
		std::string line;
		while (std::getline(fileCpp,line))
		{
			// 빈 줄 및 주석 건너뛰기
			if (line.empty() || line[0] == '#')
			{
				continue;
			}

			// key = value 포맷 파싱
			const size_t equalPosition = line.find('=');

			// 라인 문자열에 = 문자가 있는지 확인
			assert(equalPosition != std::string::npos);

			// 좌우 공백 제거용 람다
			auto trim = [](std::string& s)
				{
					// 공백 문자 집합 (\r\n -> CRLF) \n(LF)
					// ' ': 스페이스
					// \t: 탭
					// \r: 윈도우 개행문자 일자
					// \n: 개행문자
					const char* whiteSpace = "\t\r\n";

					// 문자열의 앞에서부터 공백이 아닌 첫 문자 위치 검색
					const size_t begin = s.find_first_not_of(whiteSpace);

					//공백이 아닌 문자를 못찾은 경우에는 빈 문자열로 설정 후 반환
					if (begin == std::string::npos)
					{
						s.clear();
						return;
					}

					// 문자열의 뒤에서부터 공백이 아닌 마지막 문자 위치 검색
					const size_t end = s.find_last_not_of(whiteSpace);

					// " abc " . begin = 1 / end = 3 / (end - begin) = 2
					s = s.substr(begin, end - begin + 1);
				};

			// key 파싱
			std::string key = line.substr(0, equalPosition);

			// value 파싱
			std::string value = line.substr(equalPosition + 1);

			// key/value에서 공백 제거
			trim(key);
			trim(value);

			// key와 value가 제대로 설정됐는지 확인
			assert(!key.empty() && !value.empty());

			// 속성 읽기

			// framerate
			if (key == "framerate")
			{
				setting.framerate = static_cast<float>(atof(value.c_str()));
				assert(setting.framerate > 0.0f);
				continue;
			}

			// width
			if (key == "width")
			{
				setting.width = static_cast<int>(atof(value.c_str()));
				assert(setting.width > 0);
				continue;
			}

			//framerate
			if (key == "height")
			{
				setting.height = static_cast<int>(atof(value.c_str()));
				assert(setting.height > 0);
			}

		}

		// 처리가 완료되면 파일 닫기
		fileCpp.close();
	}
}