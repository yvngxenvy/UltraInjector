#include "Injector.hpp"

bool Process::IsRunning()
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcessID);
    if (hProcess == NULL) {
        return false;
    }

    DWORD exitCode;
    if (GetExitCodeProcess(hProcess, &exitCode)) {
        CloseHandle(hProcess);
        return exitCode == STILL_ACTIVE;
    }

    CloseHandle(hProcess);
    return false;
}

HWND Process::GetHWND() {
    struct EnumData {
        DWORD processID;
        HWND hwnd;
    } data{ ProcessID, NULL };

    EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
        EnumData& data = *reinterpret_cast<EnumData*>(lParam);
        DWORD windowProcessID;
        GetWindowThreadProcessId(hWnd, &windowProcessID);
        if (windowProcessID == data.processID) {
            data.hwnd = hWnd;
            return FALSE; // Stop enumeration once found
        }
        return TRUE; // Continue enumeration
    }, reinterpret_cast<LPARAM>(&data));

    return data.hwnd;
}

HANDLE Process::CreateHandle()
{
    return OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
}

void Injector::CacheProcesses() {
	// Make a temp vec instead of clearing the current one to fix imgui stutters
    std::vector<Process> newCachedProcesses = {};
	// CachedProcesses.clear();

    DWORD processIds[1024], cbNeeded;

    if (!EnumProcesses(processIds, sizeof(processIds), &cbNeeded)) {
        return;
    }

    DWORD processCount = cbNeeded / sizeof(DWORD);

    for (DWORD i = 0; i < processCount; ++i) {
        if (processIds[i] != 0) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);
            if (hProcess) {
                HMODULE hMod;
                DWORD cbNeeded;
                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                    char processName[MAX_PATH] = "<unknown>";
                    GetModuleBaseNameA(hProcess, hMod, processName, sizeof(processName) / sizeof(char));

                    Process process(processIds[i], "", processName);
                    char windowTitle[MAX_PATH * 2] = "<unknown>";

                    GetWindowTextA(process.GetHWND(), windowTitle, MAX_PATH * 2);
                    process.WindowTitle = windowTitle;
                    newCachedProcesses.push_back(process);
					//CachedProcesses.push_back(process);
                }
                CloseHandle(hProcess);
            }
        }
    }

	CachedProcesses = newCachedProcesses;
}

void Injector::InitializePeriodic()
{
	std::thread([this]() {
		while (true) {
			CacheProcesses();
			std::this_thread::sleep_for(std::chrono::milliseconds(PERIODIC_INTERVAL));
		}
	}).detach();
}

std::vector<Process> Injector::GetActiveProcesses()
{
    return CachedProcesses;
}

std::vector<Process> Injector::GetActiveWindows()
{
    std::vector<Process> processes = {};
	for (Process process : CachedProcesses) {
        if (process.GetHWND() == nullptr)
            continue;

		if (IsWindowVisible(process.GetHWND())) {
			processes.push_back(process);
		}
	}
	return processes;
}

void Injector::Initialize()
{
	CacheProcesses();
	InitializePeriodic();
}
