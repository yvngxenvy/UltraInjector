#include "FileManager.hpp"

nlohmann::json DLLProfile::ToJson()
{
	nlohmann::json ret;
	ret["Path"] = Path.string();
	ret["Name"] = Name;
	return ret;
}

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

void FileManager::LoadCacheData()
{
	std::ifstream file(FolderPath / CACHED_FILENAME);
	if (file.is_open() == false) {
		return;
	}
	nlohmann::json json;
	try {
		file >> json;
		InjectorCacheData cacheData(json["ProcessName"].get<std::string>(), json["DLLPath"].get<std::string>());
		LoadedCacheData = cacheData;
	}
	catch (nlohmann::json::parse_error& e) {
		// Handle parse error (e.g., log the error, notify the user, etc.)
		return;
	}
	// Process the JSON data if valid
}

void FileManager::Initialize()
{
	InitializeFolder();
	LoadProfiles();
	LoadCacheData();
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

std::vector<DLLProfile> FileManager::GetProfiles()
{
	return CurrentProfiles;
}

void FileManager::AddProfile(DLLProfile Profile)
{
	CurrentProfiles.push_back(Profile);
	SaveProfiles();
}

void FileManager::RemoveProfile(DLLProfile Profile)
{
	CurrentProfiles.erase(std::remove(CurrentProfiles.begin(), CurrentProfiles.end(), Profile), CurrentProfiles.end());
	SaveProfiles();
}

InjectorCacheData FileManager::GetCacheData()
{
	return LoadedCacheData;
}

void FileManager::SetCacheData(InjectorCacheData Data)
{
	LoadedCacheData = Data;
	std::ofstream file(FolderPath / CACHED_FILENAME);
	if (file.is_open() == false) {
		return;
	}
	nlohmann::json json;
	json["ProcessName"] = Data.ProcessName;
	json["DLLPath"] = Data.DLLPath;
	file << json.dump(4);
	file.close();
}
