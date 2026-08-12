#include "Actor.h"
#include <Engine/Engine.h>
#include <Render/Renderer.h>

namespace Craft
{
	Actor::Actor(const std::string& image,
		const Vector2& position,
		Color color)
		: image(image), position(position), color(color),
		width(static_cast<int>(image.length()))
	{

	}
	
	Actor::~Actor()
	{

	}
	
	void Actor::BeginPlay()
	{
		// 중복 호출 방지를 위해 설정	|	이벤트 처리했다고 설정
		hasBeganPlay = true;
	}
	
	void Actor::Tick(float deltaTime)
	{

	}
	
	void Actor::Draw()
	{
		// 비활성 상태이면 종료.
		if (!IsActive())
		{
			return;
		}

		// 렌더러에 필요한 데이터 제출.
		Renderer::Get().Submit(image, position, color, sortingOrder);
	}

	void Actor::OnCollision(const std::shared_ptr<Actor>& other)
	{

	}

	void Actor::ChangeImage(const std::string& newImage)
	{
		// 이미지 길이 설정
		width = static_cast<int>(newImage.size());

		// 새로운 글자 값 설정
		image = newImage;
	}
	
	void Actor::Destroy()
	{
		// 액터 삭제 예약 설정
		// 다음 프레임에 액터가 레벨에서 제거됨
		hasExpired = true;
	}
	
	void Actor::QuitGame()
	{
		// 엔진 종료 요청
		Engine::Get().Quit();
	}
	void Actor::SetPosition(const Vector2& newPosition)
	{
		// 변경하려는 위치 값이 기존 값과 동일하면 종료.
		if (position == newPosition)
		{
			return;
		}

		position = newPosition;
	}
}