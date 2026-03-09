

add_library(ScriptImpl SHARED
    ScriptImplementation.cpp
    Random.h Random.cpp
    Timer.h Timer.cpp
)


target_link_libraries(ScriptImpl PRIVATE
    2025ENGINE
    flecs
    reactphysics3d
    glm
    glad
    OliUI
)
