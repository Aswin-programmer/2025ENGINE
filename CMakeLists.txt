cmake_minimum_required(VERSION 3.16)

# Handle debug symbols on MSVC properly
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  if (MSVC)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
        "$<IF:$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue,ProgramDatabase>")
  endif()
endif()

project(2025ENGINE_ROOT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
add_subdirectory(DEPENDENCIES/FLECS)

# Add dependencies (each one provides a target)
add_subdirectory(DEPENDENCIES/GLFW)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
add_subdirectory(DEPENDENCIES/GLAD)
add_subdirectory(DEPENDENCIES/GLM)

# Disable all KTX test & tool executables
set(KTX_FEATURE_TESTS OFF CACHE BOOL "" FORCE)
set(KTX_FEATURE_LOADTEST_APPS OFF CACHE BOOL "" FORCE)
set(KTX_FEATURE_TOOLS OFF CACHE BOOL "" FORCE)
set(KTX_FEATURE_TOOLS_CTS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(DEPENDENCIES/KTX)

add_subdirectory(DEPENDENCIES/TINYGLTF)
add_subdirectory(DEPENDENCIES/REACTPHYSICS3D)
add_subdirectory(DEPENDENCIES/GLYPHGL)
add_subdirectory(DEPENDENCIES/OLIUI)

# ---- control cereal here ----
set(SKIP_PERFORMANCE_COMPARISON ON CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SANDBOX OFF CACHE BOOL "" FORCE)
set(BUILD_DOC OFF CACHE BOOL "" FORCE)
set(JUST_INSTALL_CEREAL ON CACHE BOOL "" FORCE)
add_subdirectory(DEPENDENCIES/CEREAL)

add_subdirectory(SCRIPT)

# Engine library (shared)
add_subdirectory(ENGINE)

# Editor / executable
add_subdirectory(APP)

