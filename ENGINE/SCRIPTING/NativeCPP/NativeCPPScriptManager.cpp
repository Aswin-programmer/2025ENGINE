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

void NativeCPPScriptManager::PrintLastError(const std::string& prefix)
{
	DWORD err = GetLastError();
	LPVOID msg;

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&msg, 0, NULL);

	std::cerr << prefix << " | Error Code = " << err
		<< " | Message: " << (msg ? (char*)msg : "Unknown error")
		<< "\n";

	if (msg) LocalFree(msg);
}

bool NativeCPPScriptManager::LoadDLL()
{
	try {
		lastWrite = std::filesystem::last_write_time(dllBuildPath);
	}
	catch (std::exception& e) {
		std::cerr << "[ERROR] Exception: " << e.what() << "\n";
		std::cerr << "Failed to stat script DLL: " << dllBuildPath << '\n';
		return false;
	}

	std::string loadPath = MakeTempCopy(dllBuildPath);

	dllHandle = LoadLibraryFile(loadPath);
	if (!dllHandle)
	{
		PrintLastError("Failed to load DLL: " + loadPath);
		return false;
	}

	createScriptFunction = reinterpret_cast<CreateScriptFunc>
		(GetSymbol(dllHandle, "CreateScript"));
	DestroyScriptFunction = reinterpret_cast<DestroyScriptFunc>
		(GetSymbol(dllHandle, "DestroyScript"));

	if (!createScriptFunction || !DestroyScriptFunction) {
		std::cerr << "DLL does not exports CreateScript/DestroyScript\n";
		UnloadLibraryFile(dllHandle);
		dllHandle = nullptr;
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

	std::cout << "Loaded script successfully.\n";
	return true;
}

bool NativeCPPScriptManager::ReloadIfUpdated()
{
	namespace fs = std::filesystem;
	try{
		auto current = fs::last_write_time(dllBuildPath);
		if(current != lastWrite)
		{
			std::cout<<"Detected updated DLL. Reloading..."<<std::endl;
			UnloadDLL();
			return LoadDLL();
		}
	}
	catch(...){
		std::cout<<"Something went wrong with the reloading!."<<std::endl;
	}
	return false;
}

bool NativeCPPScriptManager::UnloadDLL()
{
    if(!scriptInstance)
	{
		return false;
	}

	scriptInstance->OnShutDown();

	if(DestroyScriptFunction)
	{
		DestroyScriptFunction(scriptInstance.release());
		scriptInstance = nullptr;
	}
	else
    {
        delete scriptInstance.release();
        scriptInstance = nullptr;  
        std::cout << "Error in Unloading the Script!\n";
        return false;
    }

    UnloadLibraryFile(dllHandle);
    dllHandle = nullptr;          

    return true;
}

void NativeCPPScriptManager::UpdateScript()
{
	ReloadIfUpdated();
	if(scriptInstance)
	{
		scriptInstance->OnUpdate();
	}
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

	// Ensure Windows releases the DLL completely (crucial for hot reload)
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
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

