#pragma once
#define WINDOWS_LEAN_AND_MEAN

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx9.h"
#include "../ImGui/imgui_impl_win32.h"

#include <functional>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#define DEFINE_WIDE_AND_ANSI_CHAR(VarName, Value) \
const wchar_t* const VarName##_W = L##Value; \
const char* const VarName##_A = ##Value;

/* Constants */

// TODO: Put this in a better place. I haven't learned c++ long enough to know where these constants are supposed to go
// Maybe namespace??
const wchar_t* const WINDOW_CLASS_NAME = L"UltraInjector";
DEFINE_WIDE_AND_ANSI_CHAR(WINDOW_TITLE, "Ultra Injector")

// As for these, i need to learn when if i should use constexpr because i am not sure yet
const POINTS WINDOW_SIZE = { 500, 330 };

class UserInterface
{
private:
	/* The imgui render function that will be called every frame */
	std::function<void()> RenderFn;

	/* The info of the currently created window class */
	WNDCLASSEXW WindowClassInfo;

	/* The reference to the user interface window  */
	HWND hWND;

	/* Whether or not the window/program is active (should be closed or not) */
	bool bActive;

	/* DirectX object used for creating devices */
	LPDIRECT3D9 D3D;

	/* The parameters for the "present" method in the device */
	D3DPRESENT_PARAMETERS D3DPresentParameters;

	/* The device used for rendering and handling DirectX content */
	LPDIRECT3DDEVICE9 D3DDevice;

	/* Whether the D3D device has been lost or not (can be due to user changes like resolution, graphics update, etc) */
	bool bDeviceLost;
public:
	/* Default constructor and destructor */
	
	UserInterface() = default;
	~UserInterface() = default;
private:
	/**
	 * @brief The windows process method that handles the messages from the user (WM_RESIZE, WM_EXIT, etc)
	 * @param hWnd - The handle to the window
	 * @param msg - The message that was sent to the window
	 * @param wParam - The first parameter of the message
	 * @param lParam - The second parameter of the message
	 * @return - The result of the message
	 */
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief Configures and creates the window based on the constants
	 * @param hInstance - The hInstance given by the operating system (one of the params the WinMain method)
	 */
	void ConfigureWindow(HINSTANCE hInstance);

	/* Cleans up the window (HWND) */
	void CleanUpWindow();

	/**
	 * @brief Configures the DirectX device
	 * @return - Whether the configuration was successful or not
	 */
	[[nodiscard]] bool ConfigureDevice();

	/* Cleans up the DirectX object and device */
	void CleanUpD3D();

	/* Configures ImGui so it is ready for rendering */
	void ConfigureImGui();

	/* Cleans up everything related to ImGui */
	void CleanUpImGui();
public:
	/**
	 * @brief Initializes the window, ImGui, and other necessary components
	 * @param hInstance - The hInstance given by the operating system (one of the params the WinMain method)
	 */
	void Initialize(HINSTANCE hInstance = nullptr);

	/**
	 * @brief Sets the render function that will be called every frame. This is the code for your ImGui code
	 * @param RenderFn - The function that will be called every frame
	 */
	void SetImGuiRenderExecution(std::function<void()> RenderFn);

	/* Cleans up the D3D and window */
	void Destroy();
};