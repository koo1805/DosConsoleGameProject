#include <Engine/Engine.h>
#include <Level/MainMenuLevel.h>

int main()
{
	Craft::Engine engine;
	engine.AddNewLevel<MainMenuLevel>();
	engine.Run();
}