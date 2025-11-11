#ifdef ENGINE_EXPORTS
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#include <iostream>

class ENGINE_API GlobalInformation
{
public:
    GlobalInformation();
    ~GlobalInformation();
};
