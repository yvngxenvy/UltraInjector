#include "UserInterface.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall UserInterface::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

void UserInterface::ConfigureWindow(HINSTANCE hInstance)
{
    WindowClassInfo = WNDCLASSEXW(sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0L, 0L, hInstance, nullptr, nullptr, nullptr, nullptr, WINDOW_CLASS_NAME, nullptr);
    RegisterClassExW(&WindowClassInfo);
    hWND = CreateWindowW(WindowClassInfo.lpszClassName, WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WindowSize.x, WindowSize.y, nullptr, nullptr, WindowClassInfo.hInstance, nullptr);

    ShowWindow(hWND, SW_SHOWDEFAULT);
	UpdateWindow(hWND);
}

void UserInterface::CleanUpWindow()
{
    if (hWND) {
        DestroyWindow(hWND);
		UnregisterClassW(WindowClassInfo.lpszClassName, WindowClassInfo.hInstance);
    }
}

bool UserInterface::ConfigureDevice()
{
    // Create the D3DDevice
	D3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (D3D == nullptr) {
        return false;
    }

    ZeroMemory(&D3DPresentParameters, sizeof(D3DPRESENT_PARAMETERS));
    D3DPresentParameters.Windowed = TRUE;
    D3DPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    D3DPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    D3DPresentParameters.EnableAutoDepthStencil = TRUE;
    D3DPresentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    D3DPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if (D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWND, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3DPresentParameters, &D3DDevice) < 0) {
        return false;
    }

    return true;
}

void UserInterface::CleanUpD3D()
{
	if (D3DDevice) {
		D3DDevice->Release();
		D3DDevice = nullptr;
	}
	if (D3D) {
		D3D->Release();
		D3D = nullptr;
	}
}

void UserInterface::ConfigureImGui()
{
    // Basic initialization
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// ImGui style
	ImGui::StyleColorsDark();

    // DirectX and windows init
	ImGui_ImplWin32_Init(hWND);
	ImGui_ImplDX9_Init(D3DDevice);
}

void UserInterface::CleanUpImGui()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void UserInterface::Initialize(HINSTANCE hInstance)
{
	ConfigureWindow(hInstance);
    if (ConfigureDevice() == false) {
		CleanUpWindow();
		return;
    }
    ConfigureImGui();

    // Main message loop
    bActive = true;
    bDeviceLost = false;
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
    while (bActive) {
		// Receive and handle messages
		if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			bActive = msg.message != WM_QUIT;
			continue;
		}

        // Check if device has been lost
        if (bDeviceLost) {
            HRESULT hr = D3DDevice->TestCooperativeLevel();
            if (hr == D3DERR_DEVICELOST)
            {
                Sleep(10);
                continue;
            }

            if (hr == D3DERR_DEVICENOTRESET) {
                ImGui_ImplDX9_InvalidateDeviceObjects();
                HRESULT hr = D3DDevice->Reset(&D3DPresentParameters);
                if (hr == D3DERR_INVALIDCALL) {
                    bActive = false;
                    continue;
                }

                ImGui_ImplDX9_CreateDeviceObjects();
            }

			bDeviceLost = false;
        }

        // ImGui preprocess frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // ImGui window code
        ImGui::ShowDemoWindow();

        // Rendering
		D3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		D3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		D3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(114, 144, 154, 255), 1.0f, 0);
        if (D3DDevice->BeginScene() >= 0) {
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			D3DDevice->EndScene();
        }

		// Present the rendered content
		HRESULT result = D3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
		if (result == D3DERR_DEVICELOST) {
			bDeviceLost = true;
		}
    }
}

void UserInterface::Destroy()
{
    CleanUpImGui();
    CleanUpD3D();
	CleanUpWindow();
}
