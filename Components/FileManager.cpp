#include "FileManager.hpp"

std::filesystem::path FileManager::GetFolderPath()
{
	return FolderPath;
}

void FileManager::InitializeFolder()
{
	FolderPath = std::filesystem::temp_directory_path().parent_path().parent_path() / INJECTOR_FOLDER;
	if (std::filesystem::exists(FolderPath) == false) {
		std::filesystem::create_directory(FolderPath);
	}
}

void FileManager::LoadProfiles()
{
	CurrentProfiles.clear();

	std::ifstream file(FolderPath / PROFILES_FILENAME);
	if (file.is_open() == false) {
		return;
	}

	nlohmann::json json;
	try {
		file >> json;

		for (auto profile : json) {
			DLLProfile dllProfile(profile["Path"].get<std::string>(), profile["Name"].get<std::string>());
			CurrentProfiles.push_back(dllProfile);
		}
	} catch (nlohmann::json::parse_error& e) {
		// Handle parse error (e.g., log the error, notify the user, etc.)
		return;
	}

	// Process the JSON data if valid
}

void FileManager::Initialize()
{
	InitializeFolder();
	LoadProfiles();
}

void FileManager::SaveProfiles()
{
	std::ofstream file(FolderPath / PROFILES_FILENAME);
	if (file.is_open() == false) {
		return;
	}

	nlohmann::json json;
	for (auto profile : CurrentProfiles) {
		json.push_back(profile.ToJson());
	}
	file << json.dump(4);
	file.close();
}

nlohmann::json DLLProfile::ToJson()
{
	nlohmann::json ret;
	ret["Path"] = Path.string();
	ret["Name"] = Name;
	return ret;
}
