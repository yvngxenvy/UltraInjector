#pragma once

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

/* Constants */

const std::string INJECTOR_FOLDER = "UltraInjector";
const std::string PROFILES_FILENAME = "Profiles.json";

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

class FileManager
{
private:
	/* The path to the injector data */
	std::filesystem::path FolderPath;

	/* The current cached DLL profiles */
	std::vector<DLLProfile> CurrentProfiles;
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
public:
	/* Initializes DLL profiles */
	void Initialize();

	/* Saves the cached profiles to file */
	void SaveProfiles();
};

