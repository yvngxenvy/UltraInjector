#include "Components/UserInterface.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	UserInterface window;
	window.Initialize();
	window.Destroy();
}