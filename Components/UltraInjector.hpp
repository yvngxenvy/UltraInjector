#pragma once

#include "UserInterface.hpp"
#include "Injector.hpp"
#include "FileManager.hpp"

/* Enum of what to display on the current window */
enum class UserInterfaceDisplay : uint8_t {
	UID_Main = 0,
	UID_SelectProcess = 1,
	UID_SelectDLL = 2
};

class UltraInjector
{
private:
	/* Main injector object */
	Injector Injector;

	/* Main file manager object */
	FileManager FileManager;

	/* The current process that is selected */
	Process SelectedProcess;

	/* The current DLL profile that is selected */
	DLLProfile SelectedDLLProfile;

	/* The current display that is being shown */
	UserInterfaceDisplay CurrentDisplay = UserInterfaceDisplay::UID_Main;
private:
	/* Handles the ImGui code for the main window */
	void DisplayMainUserInterface();

	/* Handles the ImGui code for the select process window */
	void DisplaySelectProcessUserInterface();

	/* Handles the ImGui code for the select DLL window */
	void DisplaySelectDLLUserInterface();
public:
	/* Default constructer and and destructor */

	UltraInjector() = default;
	~UltraInjector() = default;
public:
	/* Initializes the injector and user interface */
	void Initialize();
};

