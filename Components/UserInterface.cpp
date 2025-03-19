#include "UserInterface.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall UserInterface::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static POINT initalPoint = {0, 0};

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg) {
        // TODO: improve movement system
    case WM_LBUTTONDOWN: {
        
        initalPoint = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        break;
    }
    case WM_MOUSEMOVE: {
        if (wParam == MK_LBUTTON) {
            POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            RECT rect = {};

            GetWindowRect(hWnd, &rect);

			rect.left += point.x - initalPoint.x;
			rect.top += point.y - initalPoint.y;

            if (initalPoint.x >= 0 && initalPoint.x <= WINDOW_SIZE.x && initalPoint.y >= 0 && initalPoint.y <= 26) {
				SetWindowPos(hWnd, nullptr, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            }
        }
        break;
    }
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
    hWND = CreateWindowW(WindowClassInfo.lpszClassName, WINDOW_TITLE_W, WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_SIZE.x, WINDOW_SIZE.y, nullptr, nullptr, WindowClassInfo.hInstance, nullptr);

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
    io.IniFilename = nullptr;

	// ImGui style
    // Font
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Segoeuib.ttf", 17.5f);
    
    // Colors
	ImGuiStyle* style = &ImGui::GetStyle();
    style->WindowPadding = ImVec2(15, 15);
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;
    style->FrameBorderSize = 0.f;
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);

    ImVec4* colors = style->Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.60f, 0.26f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.60f, 0.26f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.31f, 0.16f, 0.48f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.60f, 0.26f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.54f, 0.24f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.26f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.60f, 0.26f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.60f, 0.26f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.06f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.60f, 0.26f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.60f, 0.26f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.60f, 0.26f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.10f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.41f, 0.10f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.26f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.26f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.60f, 0.26f, 0.98f, 0.95f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.60f, 0.26f, 0.98f, 0.80f);
    colors[ImGuiCol_Tab] = ImVec4(0.37f, 0.18f, 0.58f, 0.86f);
    colors[ImGuiCol_TabSelected] = ImVec4(0.42f, 0.20f, 0.68f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.60f, 0.26f, 0.98f, 1.00f);
    colors[ImGuiCol_TabDimmed] = ImVec4(0.11f, 0.07f, 0.15f, 0.97f);
    colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.27f, 0.14f, 0.42f, 1.00f);
    colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextLink] = ImVec4(0.60f, 0.26f, 0.98f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.53f, 0.53f, 0.53f, 0.45f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavCursor] = ImVec4(0.60f, 0.26f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

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
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(WINDOW_SIZE.x, WINDOW_SIZE.y), ImGuiCond_Always);
        ImGui::Begin(WINDOW_TITLE_A, &bActive, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        RenderFn();
        ImGui::End();

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

void UserInterface::SetImGuiRenderExecution(std::function<void()> RenderFn)
{
	this->RenderFn = RenderFn;
}

void UserInterface::Destroy()
{
    CleanUpImGui();
    CleanUpD3D();
	CleanUpWindow();
}
