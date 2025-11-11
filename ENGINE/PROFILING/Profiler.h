#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif


#ifdef TRACY_ENABLE
	#include "Tracy/Tracy.hpp"
	#include "Tracy/TracyOpenGL.hpp"

	#define PROFILE_SCOPE() ZoneScoped
	#define PROFILE_SCOPE_N(name) ZoneScopedN(name)
	#define PROFILE_FRAME() FrameMark
	#define PROFILE_GPU_SCOPE(name) TracyGpuZone(name)
#else
	#define PROFILE_SCOPE()
	#define PROFILE_SCOPE_N(name)
	#define PROFILE_FRAME()
	#define PROFILE_GPU_SCOPE(name)
#endif
