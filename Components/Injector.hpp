#pragma once
#define WINDOWS_LEAN_AND_MEAN

#include <Windows.h>
#include <Psapi.h>
#include <cstdint>
#include <string>
#include <vector>
#include <thread>
#include <filesystem>

/* Constants */

const int32_t PERIODIC_INTERVAL = 250;

/* Struct for active process */
struct Process {
	/* The process id of the process*/
	int32_t ProcessID;

	/* The window title as it appears in the OS. */
	std::string WindowTitle;
	
	/* The process name (with .exe in it). e.g. Notepad.exe, RocketLeague.exe, RobloxPlayerBeta.exe, etc */
	std::string ProcessName;

	/**
	 * @brief Finds a module in the process by the module name
	 * @param ModuleName - The name of the module to find
	 * @return The handle to the module if found, NULL otherwise
	 */
	HMODULE FindModule(std::string ModuleName);

	/**
	 * @brief Checks if the process is running or not
	 * @return True if the process is running, false otherwise
	 */
	bool IsRunning();

	/**
	 * @brief Returns the current HWND. Be cautious how many times you call this method, as there is no caching involved. Relies on the ProcessName
	 * @return The HWND of the process
	 */
	HWND GetHWND();

	/**
	 * @brief Creates a handle to the process by opening the process with all flags. Relies on the ProcessID
	 * @return The handle to the process
	 */
	HANDLE CreateHandle();

	/* Constructors */

	Process() : ProcessID(0), WindowTitle(""), ProcessName("") {}
	Process(int32_t ProcessID, std::string WindowTitle, std::string ProcessName) : ProcessID(ProcessID), WindowTitle(WindowTitle), ProcessName(ProcessName) {}

	/* Operators */

	bool operator==(const Process& other) const {
		return ProcessID == other.ProcessID;
	}

	bool operator!=(const Process& other) const {
		return ProcessID != other.ProcessID;
	}
	
	void operator=(const Process& other) {
		ProcessID = other.ProcessID;
		WindowTitle = other.WindowTitle;
		ProcessName = other.ProcessName;
	}
};

class Injector
{
private:
	/* The vector of cached processes */
	std::vector<Process> CachedProcesses;	
private:
	/* Caches the cached vector field with the currently active processes */
	void CacheProcesses();
	
	/* Initializes the periodic loop that actively calls cache processes */
	void InitializePeriodic();
public:
	/* Default constructor and destructor */

	Injector() = default;
	~Injector() = default;
public:
	/**
	 * @brief This will return all processes (even if hidden) running on the OS. Fetches from cache
	 * @return A vector of all active processes
	 */
	std::vector<Process> GetActiveProcesses();

	/**
	 * @brief This will return all windows (not hidden) running on the OS. Fetches from cache
	 * @return A vector of all active processes
	 */
	std::vector<Process> GetActiveWindows();

	/**
	 * @brief Injects a DLL into a process
	 * @param Process - The process to inject the DLL into
	 * @param DLLPath - The path to the DLL to inject
	 * @return True if the DLL was injected successfully, false otherwise
	 */
	bool InjectDLL(Process Process, const std::filesystem::path DLLPath);

	/**
	 * @brief Uninjects a DLL into a process
	 * @param Process - The process to uninject the DLL from
	 * @param DLLPath - The path to the DLL to uninject
	 * @return True if the DLL was uninjected successfully, false otherwise
	 */
	bool UninjectDLL(Process Process, const std::filesystem::path DLLPath);

	/**
	 * @brief Finds a process from process name if it is lost
	 * @param Process - The process used as a reference to find the process
	 * @return The new process found with updated info
	 */
	Process RelocateProcess(Process Process);

	/* Initializes the periodic loop */
	void Initialize();
};

