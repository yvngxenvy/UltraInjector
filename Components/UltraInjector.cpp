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
						if (ImGui::Selectable(std::string(displayProcess + "##_" + std::to_string(i)).c_str(), (process.ProcessID == SelectedProcess.ProcessID))) {
							SelectedProcess = process;
						}
					}
				}

				ImGui::EndListBox();
			}

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
						if (ImGui::Selectable(std::string(displayProcess + "##_" + std::to_string(i)).c_str(), (process.ProcessID == SelectedProcess.ProcessID))) {
							SelectedProcess = process;
						}
					}
				}

				ImGui::EndListBox();
			}

			if (ImGui::Button("Select", ImGui::GetContentRegionAvail())) {
				CurrentDisplay = UserInterfaceDisplay::UID_Main;
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

void UltraInjector::Initialize()
{
	// TODO: Make injector a field later on. dont want to now cuz it looks cool
	Injector.Initialize();

	UserInterface window;
	window.SetImGuiRenderExecution([&]() {
		if (CurrentDisplay == UserInterfaceDisplay::UID_Main) {
			DisplayMainUserInterface();
		}
		else if (CurrentDisplay == UserInterfaceDisplay::UID_SelectProcess) {
			DisplaySelectProcessUserInterface();
		}
	});
	window.Initialize(); // This is a method which will run until the program exits

	// By the time this method calls, the program exited itself or the user has exited the program
	window.Destroy();
}
