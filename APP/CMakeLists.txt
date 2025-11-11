# --- Executable ---
add_executable(2025ENGINE_EXE
    "2025ENGINE.cpp"
)

target_link_libraries(2025ENGINE_EXE PRIVATE 2025ENGINE)

target_include_directories(2025ENGINE_EXE PRIVATE
    "${CMAKE_SOURCE_DIR}/ENGINE"
)

# --- Copy the engine DLL after build ---
add_custom_command(TARGET 2025ENGINE_EXE POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:2025ENGINE>"
        "$<TARGET_FILE_DIR:2025ENGINE_EXE>/"
    COMMENT "Copying 2025ENGINE.dll next to 2025ENGINE_EXE"
)

# Copy flecs.dll if it exists
add_custom_command(TARGET 2025ENGINE_EXE POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/DEPENDENCIES/FLECS/flecs.dll"
        "$<TARGET_FILE_DIR:2025ENGINE_EXE>/"
    COMMENT "Copying flecs.dll next to 2025ENGINE_EXE"
)

# Copy flecs.dll if it exists
add_custom_command(TARGET 2025ENGINE_EXE POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/DEPENDENCIES/GLFW/src/glfw3.dll"
        "$<TARGET_FILE_DIR:2025ENGINE_EXE>/"
    COMMENT "Copying flecs.dll next to 2025ENGINE_EXE"
)


target_compile_definitions(2025ENGINE_EXE PRIVATE 
	RESOURCES_PATH="${CMAKE_SOURCE_DIR}/ASSETS/"
)

target_compile_definitions(2025ENGINE_EXE PRIVATE 
	SCRIPT_PATH="${CMAKE_BINARY_DIR}/SCRIPT/"
)

# target_compile_definitions(GetIntoGameDevOPENGL PRIVATE 
# 	RESOURCES_PATH="ASSETS/"
# )
