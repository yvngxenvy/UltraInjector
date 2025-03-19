#include "UltraInjector.hpp"
#include "../Utils.hpp"

void UltraInjector::DisplayMainUserInterface()
{
	if (ImGui::Button("Select Process")) {
		CurrentDisplay = UserInterfaceDisplay::UID_SelectProcess;
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::InputTextWithHint("##ProcessName", "Process name will go here...", &SelectedProcess.ProcessName, ImGuiInputTextFlags_ReadOnly);

	if (ImGui::Button("Select DLL")) {
		CurrentDisplay = UserInterfaceDisplay::UID_SelectDLL;
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
	ImGui::InputTextWithHint("##DLLName", "DLL name will go here...", &SelectedDLLProfile.Name, ImGuiInputTextFlags_ReadOnly);

	if (SelectedProcess.ProcessID != 0) {
		ImGui::Separator();
		bool bIsRunning = SelectedProcess.IsRunning();
        ImGui::Text("Process Active:");
		ImGui::SameLine();
		ImGui::TextColored(bIsRunning ? ImVec4(0.0f, 1.f, 0.f, 1.0f) : ImVec4(1.f, 0.f, 0.f, 1.f), bIsRunning ? "True" : "False");

		if (bIsRunning) {
			ImGui::SameLine();

			bool bHWND = SelectedProcess.GetHWND();
			ImGui::Text("Window Active:");
			ImGui::SameLine();
			ImGui::TextColored(bHWND ? ImVec4(0.0f, 1.f, 0.f, 1.0f) : ImVec4(1.f, 0.f, 0.f, 1.f), bHWND ? "True" : "False");

			ImGui::Text("Process ID:");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "%d", SelectedProcess.ProcessID);

			if (bHWND) {
				if (SelectedProcess.WindowTitle.empty() == false) {
					ImGui::Text("Window Title:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "%s", SelectedProcess.WindowTitle.c_str());
				}
			}
		}
		else {
			SelectedProcess = Injector.RelocateProcess(SelectedProcess);
		}
		ImGui::Separator();
		if (ImGui::BeginChild("##InjectButtons", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 20.f})) {
			if (ImGui::Button("Inject", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2 - 5.f })) {
				Injector.InjectDLL(SelectedProcess, SelectedDLLProfile.Path);
			}
			if (ImGui::Button("Uninject", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y })) {
				Injector.UninjectDLL(SelectedProcess, SelectedDLLProfile.Path);
			}
			ImGui::EndChild();
		}
	}

	// Display fps stats at the bottom
	std::string fps = std::to_string(static_cast<int32_t>(round(ImGui::GetIO().Framerate))) + " fps";

	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 31.f);
	ImGui::Separator();
	ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 28.f);
	static float offset = 0.0f;
	float maxOffset = ImGui::GetContentRegionAvail().x + 100.f;
	offset += 0.8f;
	if (offset >= maxOffset) {
		offset = 0.0f;
	}
	ImGui::SetCursorPosX(offset - 50.f);
	ImGui::Text(fps.c_str());
}

void UltraInjector::DisplaySelectProcessUserInterface()
{
	if (ImGui::BeginTabBar("##ProcessTypes")) {
		if (ImGui::BeginTabItem("Windows")) {
			static std::string processSearch = "";
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::InputTextWithHint("##ProcessSearch", "Process that you want to search...", &processSearch);

			if (ImGui::BeginListBox("##ProcessListBox", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 50.f })) {
				std::vector<Process> processes = Injector.GetActiveWindows();

				for (int i = 0; i < processes.size(); i++) {
					Process process = processes[i];
					std::string displayProcess = process.WindowTitle;

					if (displayProcess.empty() == true) {
						displayProcess = process.ProcessName;
					}

					if (processSearch.empty() == true || Utils::ToLower(displayProcess).find(Utils::ToLower(processSearch)) != std::string::npos) {
						if (ImGui::Selectable(std::string(displayProcess + "##_" + std::to_string(i)).c_str(), (process == SelectedProcess))) {
							SelectedProcess = process;

							FileManager.SetCacheData(InjectorCacheData(SelectedProcess.ProcessName, SelectedDLLProfile.Path.string()));

							if (process == SelectedProcess) { // Handle double clicks
								CurrentDisplay = UserInterfaceDisplay::UID_Main;
							}
						}
					}
				}

				ImGui::EndListBox();
			}

			ImGui::Separator();

			if (ImGui::Button("Select", ImGui::GetContentRegionAvail())) {
				CurrentDisplay = UserInterfaceDisplay::UID_Main;
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Processes")) {
			static std::string processSearch = "";
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::InputTextWithHint("##ProcessSearch", "Process that you want to search...", &processSearch);

			if (ImGui::BeginListBox("##ProcessListBox", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 50.f })) {
				std::vector<Process> processes = Injector.GetActiveProcesses();

				for (int i = 0; i < processes.size(); i++) {
					Process process = processes[i];
					std::string displayProcess = process.ProcessName;

					if (displayProcess.empty() == true) {
						continue;
					}

					if (processSearch.empty() == true || Utils::ToLower(displayProcess).find(Utils::ToLower(processSearch)) != std::string::npos) {
						if (ImGui::Selectable(std::string(displayProcess + "##_" + std::to_string(i)).c_str(), (process == SelectedProcess))) {
							SelectedProcess = process;

							FileManager.SetCacheData(InjectorCacheData(SelectedProcess.ProcessName, SelectedDLLProfile.Path.string()));

							if (process == SelectedProcess) { // Handle double clicks
								CurrentDisplay = UserInterfaceDisplay::UID_Main;
							}
						}
					}
				}

				ImGui::EndListBox();
			}

			ImGui::Separator();

			if (ImGui::Button("Select", ImGui::GetContentRegionAvail())) {
				CurrentDisplay = UserInterfaceDisplay::UID_Main;
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

void UltraInjector::DisplaySelectDLLUserInterface()
{
	if (ImGui::BeginChild("##Profiles", {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y-120.f})) {
		if (ImGui::BeginListBox("##ProfilesListBox", ImGui::GetContentRegionAvail())) {
			for (auto profile : FileManager.GetProfiles()) {
				if (ImGui::Selectable(profile.Name.c_str(), SelectedDLLProfile == profile)) {
					SelectedDLLProfile = profile;

					FileManager.SetCacheData(InjectorCacheData(SelectedProcess.ProcessName, SelectedDLLProfile.Path.string()));

					if (profile == SelectedDLLProfile) { // Handle double clicks
						CurrentDisplay = UserInterfaceDisplay::UID_Main;
					}
				}
			}
			ImGui::EndListBox();
		}
		ImGui::EndChild();
	}

	ImGui::Separator();

	if (ImGui::Button("Select DLL", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y/2-6.f })) {
		CurrentDisplay = UserInterfaceDisplay::UID_Main;
	}

	if (ImGui::Button("Add DLL Profile", { ImGui::GetContentRegionAvail().x / 2 - 5.f, ImGui::GetContentRegionAvail().y })) {
		// Open a file dialog to select a DLL file
		OPENFILENAMEA ofn;
		char szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "DLL Files\0*.dll\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn) == TRUE) {
			std::filesystem::path selectedPath = ofn.lpstrFile;
			std::string fileName = selectedPath.filename().string();
			SelectedDLLProfile = DLLProfile(selectedPath, fileName);
			FileManager.AddProfile(SelectedDLLProfile);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove Selected DLL Profile", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y })) {
		if (SelectedDLLProfile.Path.empty() == false) {
			FileManager.RemoveProfile(SelectedDLLProfile);
			SelectedDLLProfile = DLLProfile();
		}
	}
}

void UltraInjector::Initialize()
{
	Injector.Initialize();
	FileManager.Initialize();

	InjectorCacheData cacheData = FileManager.GetCacheData();

	// PID of 1 so program will can automatically relocate the process
	SelectedProcess = Process(1, "", cacheData.ProcessName);
	
	for (const auto& profile : FileManager.GetProfiles()) {
		if (profile.Path == cacheData.DLLPath) {
			SelectedDLLProfile = profile;
			break;
		}
	}

	UserInterface window;
	window.SetImGuiRenderExecution([&]() {
		if (CurrentDisplay == UserInterfaceDisplay::UID_Main) {
			DisplayMainUserInterface();
		}
		else if (CurrentDisplay == UserInterfaceDisplay::UID_SelectProcess) {
			DisplaySelectProcessUserInterface();
		}
		else if (CurrentDisplay == UserInterfaceDisplay::UID_SelectDLL) {
			DisplaySelectDLLUserInterface();
		}
	});
	window.Initialize(); // This is a method which will run until the program exits

	// By the time this method calls, the program exited itself or the user has exited the program
	window.Destroy();
}
