#pragma once

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

/* Constants */

const std::string INJECTOR_FOLDER = "UltraInjector";
const std::string PROFILES_FILENAME = "Profiles.json";
const std::string CACHED_FILENAME = "InjectorCache.json";

/* The info for DLLs the user has stored that will be used for injection */
struct DLLProfile {
	/* The path to the DLL */
	std::filesystem::path Path;

	/* The name of the DLL */
	std::string Name;

	nlohmann::json ToJson();

	/* Constructors */

	DLLProfile() : Path(""), Name("") {}
	DLLProfile(std::filesystem::path Path, std::string Name) : Path(Path), Name(Name) {}

	/* Operators */

	bool operator==(const DLLProfile& Other) const {
		return Path == Other.Path && Name == Other.Name;
	}

	bool operator!=(const DLLProfile& Other) const {
		return !(*this == Other);
	}
};

/* Cached data for injector for quick injecting */
struct InjectorCacheData {
	/* The process that was recently used by the user */
	std::string ProcessName;

	/* The DLL that was recently used by the user */
	std::string DLLPath;

	/* Constructor */

	InjectorCacheData() : ProcessName(""), DLLPath("") {}
	InjectorCacheData(std::string ProcessName, std::string DLLPath) : ProcessName(ProcessName), DLLPath(DLLPath) {}

	/* Operators */

	bool operator==(const InjectorCacheData& Other) const {
		return ProcessName == Other.ProcessName && DLLPath == Other.DLLPath;
	}

	bool operator!=(const InjectorCacheData& Other) const {
		return !(*this == Other);
	}

	void operator=(const InjectorCacheData& Other) {
		ProcessName = Other.ProcessName;
		DLLPath = Other.DLLPath;
	}
};

class FileManager
{
private:
	/* The path to the injector data */
	std::filesystem::path FolderPath;

	/* The current cached DLL profiles */
	std::vector<DLLProfile> CurrentProfiles;

	/* Recently loaded cache data */
	InjectorCacheData LoadedCacheData;
public:
	/* Default constructor and destructor */

	FileManager() = default;
	~FileManager() = default;
private:
	/* Gets the path to injector data folder */
	std::filesystem::path GetFolderPath();

	/* Initializes the folder for the injector */
	void InitializeFolder();

	/* Loads the current profiles to the CurrentProfiles field */
	void LoadProfiles();

	/* Loads the cache data into the LoadedCacheData field */
	void LoadCacheData();
public:
	/* Initializes DLL profiles */
	void Initialize();

	/* Saves the cached profiles to file */
	void SaveProfiles();

	/**
	 * @brief Gets the current profiles
	 * @return The current profiles as a vector of the wrapper struct
	 */
	std::vector<DLLProfile> GetProfiles();

	/**
	 * @brief Adds a new profile to the current profiles
	 * @param Profile The profile to add
	 */
	void AddProfile(DLLProfile Profile);

	/**
	 * @brief Removes a profile from the current profiles
	 * @param Profile The profile to remove
	 */
	void RemoveProfile(DLLProfile Profile);

	/**
	 * @brief Gets the loaded cache data
	 * @return The loaded cache data
	 */
	InjectorCacheData GetCacheData();

	/**
	 * @brief Sets the cache data (in file)
	 * @param Data - The data to set
	 */
	void SetCacheData(InjectorCacheData Data);
};

