#include "NativeCPPScriptManager.h"

NativeCPPScriptManager::NativeCPPScriptManager()
{
}

NativeCPPScriptManager::NativeCPPScriptManager(std::string dllBuildPath_)
	:
	dllBuildPath{dllBuildPath_}
{
}

NativeCPPScriptManager::~NativeCPPScriptManager()
{
}

bool NativeCPPScriptManager::LoadDLL()
{
	try {
		lastWrite = std::filesystem::last_write_time(dllBuildPath);
	}
	catch (...) {
		std::cerr << "Failed to stat script DLL: " << dllBuildPath << '\n';
		return false;
	}

	std::string loadPath = MakeTempCopy(dllBuildPath);

	dllHandle = LoadLibraryFile(loadPath);
	if (!dllHandle) {
		std::cerr << "Failed to load DLL: " << loadPath << '\n';
		return false;
	}

	createScriptFunction = reinterpret_cast<CreateScriptFunc>
		(GetSymbol(dllHandle, "CreateScript"));
	DestroyScriptFunction = reinterpret_cast<DestroyScriptFunc>
		(GetSymbol(dllHandle, "DestroyScript"));

	if (!createScriptFunction || !DestroyScriptFunction) {
		std::cerr << "DLL does not exports CreateScript/DestroyScript\n";
		UnloadLibraryFile(dllHandle);
		return false;
	}

	scriptInstance.reset(createScriptFunction());
	if (!scriptInstance) {
		std::cerr << "CreateScript returned null\n";
		return false;
	}

	scriptInstance->SetupDependencies(ecsWorld);

	std::cout << "[ENGINE] &g_World: " << ecsWorld.get() << "\n";

	scriptInstance->OnIntialize();
	for (int i = 0; i < scriptInstance->v1.size(); i++)
	{
		std::cout << scriptInstance->v1[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "Loaded script successfully.\n";
	return true;
}

void NativeCPPScriptManager::LoadDependencies(std::shared_ptr<ECSWorld> ecsWorld_)
{
	ecsWorld = ecsWorld_;
}

LibHandle NativeCPPScriptManager::LoadLibraryFile(const std::string& path)
{
#ifdef _WIN32
	return LoadLibraryA(path.c_str());
#else
	//return dlopen(path.c_str(), RTLD_NOW);
	// Implemented Later.
#endif
}

void NativeCPPScriptManager::UnloadLibraryFile(LibHandle h)
{
	if (!h) return;
#ifdef _WIN32
	FreeLibrary(h);
#else
	//dlclose(h);
	// Implemented Later.
#endif
}

void* NativeCPPScriptManager::GetSymbol(LibHandle h, const char* name)
{
	if (!h) return nullptr;
#ifdef _WIN32
	return reinterpret_cast<void*>(GetProcAddress(h, name));
#else
	//return dlsym(h, name);
	//Implemented Later.
#endif
}

std::string NativeCPPScriptManager::MakeTempCopy(const std::string& srcPath)
{
	std::filesystem::path src(srcPath);
	auto tmp = src.parent_path() / (src.stem().string() + "_hotcopy" + src.extension().string());
	try {
		std::filesystem::copy_file(src, tmp, std::filesystem::copy_options::overwrite_existing);
		return tmp.string();
	}
	catch (...) {
		return srcPath; // fallback to original
	}
}

