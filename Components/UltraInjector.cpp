#include "UltraInjector.hpp"

void UltraInjector::Initialize()
{
	static Injector injector;
	injector.Initialize();

	UserInterface window;
	window.SetImGuiRenderExecution([&]() {
		ImGui::Text("Hello, world!");
	});
	window.Initialize(); // This is a method which will run until the program exits

	// By the time this method calls, the program exited itself or the user has exited the program
	window.Destroy();
}
