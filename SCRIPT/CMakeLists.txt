

add_library(ScriptImpl SHARED
    ScriptImplementation.cpp
)


target_link_libraries(ScriptImpl PRIVATE
    2025ENGINE
    flecs
    reactphysics3d
    glm
    glad
    OliUI
)
